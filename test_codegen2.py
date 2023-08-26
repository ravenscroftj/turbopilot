#%%
from transformers import AutoTokenizer, AutoModelForCausalLM
tokenizer = AutoTokenizer.from_pretrained("Salesforce/codegen2-1B")
model = AutoModelForCausalLM.from_pretrained("Salesforce/codegen2-1B", trust_remote_code=True, revision="main")


#%%
model = model.to(device="cuda")

#%%
text = """
import os

def post_to_pastebin"""
input_ids = tokenizer(text, return_tensors="pt").to("cuda").input_ids
generated_ids = model.generate(input_ids, max_length=512)
print(tokenizer.decode(generated_ids[0], skip_special_tokens=True))

# %%

def format_model_input(prefix, suffix):
  return prefix + "<mask_1>" + suffix + "<|endoftext|>" + "<sep>" + "<mask_1>"


prefix = """
import os

def post_to_pastebin"""
suffix = "result = post_to_pastebin(content)"
text = format_model_input(prefix, suffix)
input_ids = tokenizer(text, return_tensors="pt").to("cuda").input_ids
generated_ids = model.generate(input_ids, max_length=128)
print(tokenizer.decode(generated_ids[0], skip_special_tokens=False))
# %%
def main():
  text = """

  print(tokenizer.decode(generated_ids[0], skip_special_tokens=True))

if __name__ == '__main__':
    main()

  print(tokenizer.decode(generated_ids[0], skip_special_tokens=True))
  # %%

  import os

  def post_to_pastebin"""
  input_ids = tokenizer(text, return_tensors="pt").to("cuda").input_ids
  generated_ids = model.generate(input_ids, max_length=512)


 print(tokenizer.decode(generated_ids[0], skip_special_tokens=True))

# %%

def post_to_pastebin(content):
  input_ids = tokenizer(content, return_tensors="pt").to("cuda").input_ids
  generated_ids = model.generate(input_ids, max_length=512)
  return tokenizer.decode(generated_ids[0], skip_special_tokens=True)




  