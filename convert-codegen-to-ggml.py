# Convert GPT-J-6B h5 transformer model to ggml format
#
# Load the model using GPTJForCausalLM.
# Iterate over all variables and write them to a binary file.
#
# For each variable, write the following:
#   - Number of dimensions (int)
#   - Name length (int)
#   - Dimensions (int[n_dims])
#   - Name (char[name_length])
#   - Data (float[n_dims])
#
# By default, the bigger matrices are converted to 16-bit floats.
# This can be disabled by adding the "use-f32" CLI argument.
#
# At the start of the ggml file we write the model parameters
# and vocabulary.
#

import sys
import struct
import json
import torch
import numpy as np
from accelerate import init_empty_weights

from transformers import AutoModelForCausalLM, AutoTokenizer

# ref: https://github.com/openai/gpt-2/blob/master/src/encoder.py
def bytes_to_unicode():
    """
    Returns list of utf-8 byte and a mapping to unicode strings. We specifically avoids mapping to whitespace/control
    characters the bpe code barfs on.
    The reversible bpe codes work on unicode strings. This means you need a large # of unicode characters in your vocab
    if you want to avoid UNKs. When you're at something like a 10B token dataset you end up needing around 5K for
    decent coverage. This is a significant percentage of your normal, say, 32K bpe vocab. To avoid that, we want lookup
    tables between utf-8 bytes and unicode strings.
    """
    bs = (
        list(range(ord("!"), ord("~") + 1)) + list(range(ord("¡"), ord("¬") + 1)) + list(range(ord("®"), ord("ÿ") + 1))
    )
    cs = bs[:]
    n = 0
    for b in range(2**8):
        if b not in bs:
            bs.append(b)
            cs.append(2**8 + n)
            n += 1
    cs = [chr(n) for n in cs]
    return dict(zip(bs, cs))

if len(sys.argv) < 3:
    print("Usage: convert-h5-to-ggml.py dir-model [use-f32]\n")
    print("  ftype == 0 -> float32")
    print("  ftype == 1 -> float16")
    sys.exit(1)

# output in the same directory as the model
dir_model = sys.argv[1]
fname_out = sys.argv[1] + "/ggml-model.bin"

with open(dir_model + "/vocab.json", "r", encoding="utf8") as f:
    encoder = json.load(f)

with open(dir_model + "/added_tokens.json", "r") as f:
    encoder_added = json.load(f)

with open(dir_model + "/config.json", "r") as f:
    hparams = json.load(f)

# possible data types
#   ftype == 0 -> float32
#   ftype == 1 -> float16
#
# map from ftype to string
ftype_str = ["f32", "f16"]

ftype = 1
if len(sys.argv) > 2:
    ftype = int(sys.argv[2])
    if ftype < 0 or ftype > 1:
        print("Invalid ftype: " + str(ftype))
        sys.exit(1)
    fname_out = sys.argv[1] + "/ggml-model-" + ftype_str[ftype] + ".bin"


model = AutoModelForCausalLM.from_pretrained(dir_model, low_cpu_mem_usage=True)
print (model)

tokenizer = AutoTokenizer.from_pretrained('Salesforce/codegen-350M-multi')

print(tokenizer)


# config = AutoConfig.from_pretrained(sys.argv[1])

# model = AutoModelForCausalLM.from_pretrained("./codegen-2B-multi", torch_dtype=torch.float16).to("cuda")

from accelerate import load_checkpoint_and_dispatch


list_vars = model.state_dict()
#print (list_vars)

fout = open(fname_out, "wb")

fout.write(struct.pack("i", 0x67676d6c)) # magic: ggml in hex
fout.write(struct.pack("i", hparams['vocab_size']))
fout.write(struct.pack("i", hparams["n_positions"]))
fout.write(struct.pack("i", hparams["n_embd"]))
fout.write(struct.pack("i", hparams["n_head"]))
fout.write(struct.pack("i", hparams["n_layer"]))
fout.write(struct.pack("i", hparams["rotary_dim"]))
fout.write(struct.pack("i", ftype))

byte_encoder = bytes_to_unicode()
byte_decoder = {v:k for k, v in byte_encoder.items()}

print(byte_encoder)

fout.write(struct.pack("i", hparams['vocab_size']))#len(encoder) + len(encoder_added)))

# replace key tokens in tokenizer

for word,idx in sorted(tokenizer.vocab.items(), key=lambda x: x[1]) :
    #text = word.encode("utf8") #
    text = bytearray([byte_decoder[c] for c in word if c in byte_decoder])

    if(len(text)) < 1:
        #print(f"'{word}'")
        #continue
        text = bytearray(word.encode('utf8'))
    # else:
    #     print(text)
    fout.write(struct.pack("i", len(text)))
    fout.write(text)

# for key in encoder:
#     #text = bytearray([byte_decoder[c] for c in key])
#     text = key.encode("utf8")
#     fout.write(struct.pack("i", len(text)))
#     fout.write(text)

# for key in encoder_added:
#     try:
#         #text = bytearray([byte_decoder[c] for c in key])
#         text = key.encode("utf8")
#     except Exception as e:
#             print(e)
#             print(key)
#             print(text)
#             sys.exit(1)
#     fout.write(struct.pack("i", len(text)))
#     fout.write(text)

empty_vocab = hparams['vocab_size'] - tokenizer.vocab_size

print(f"Fill empty vocab for {empty_vocab} slots")

for i in range( hparams['vocab_size'] - len(encoder) - len(encoder_added)):
    text = "<|endoftext|>".encode("utf8")
    fout.write(struct.pack("i", len(text)))
    fout.write(text)

for name in list_vars.keys():
    data = list_vars[name].squeeze().numpy()
    print("Processing variable: " + name + " with shape: ", data.shape)

    # we don't need these
    if name.endswith("attn.masked_bias") or name.endswith(".attn.bias"):
        print("  Skipping variable: " + name)
        continue

    n_dims = len(data.shape);

    # ftype == 0 -> float32, ftype == 1 -> float16
    ftype_cur = 0;
    if ftype != 0:
        if name[-7:] == ".weight" and n_dims == 2:
            print("  Converting to float16")
            data = data.astype(np.float16)
            ftype_cur = 1
        else:
            print("  Converting to float32")
            data = data.astype(np.float32)
            ftype_cur = 0
    else:
        if data.dtype != np.float32:
            print("  Converting to float32")
            data = data.astype(np.float32)
            ftype_cur = 0

    # for efficiency - transpose these matrices:
    # (note - with latest ggml this is no longer more efficient, so disabling it)
    #  "transformer.h.*.mlp.fc_in.weight"
    #  "transformer.h.*.attn.out_proj.weight"
    #  "transformer.h.*.attn.q_proj.weight"
    #  "transformer.h.*.attn.k_proj.weight"
    #  "transformer.h.*.attn.v_proj.weight"
    #if name.endswith(".mlp.fc_in.weight")     or \
    #   name.endswith(".attn.out_proj.weight") or \
    #   name.endswith(".attn.q_proj.weight")   or \
    #   name.endswith(".attn.k_proj.weight")   or \
    #   name.endswith(".attn.v_proj.weight"):
    #    print("  Transposing")
    #    data = data.transpose()

    # header
    str = name.encode('utf-8')

    fout.write(struct.pack("iii", n_dims, len(str), ftype_cur))
    for i in range(n_dims):
        fout.write(struct.pack("i", data.shape[n_dims - 1 - i]))
    fout.write(str);

    # data
    data.tofile(fout)

fout.close()

print("Done. Output file: " + fname_out)
print("")
