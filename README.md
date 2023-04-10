# TurboPilot

TurboPilot is a self-hosted [copilot](https://github.com/features/copilot) clone which uses the library behind [llama.cpp](https://github.com/comex/llama.cpp) to run the [6 Billion Parameter Salesforce Codegen model](https://github.com/salesforce/CodeGen) in 4GiB of RAM. It is heavily based and inspired by on the [fauxpilot](https://github.com/fauxpilot/fauxpilot) project.

***NB: This is a proof of concept right now rather than a stable tool. Autocompletion is quite slow in this version of the project. Feel free to play with it, but your mileage may vary.***

![a screen recording of turbopilot running through fauxpilot plugin](assets/screenrecording.gif)

## Getting Started

```bash
git clone https://github.com/ravenscroftj/turbopilot
git submodule init
cd ggml
mkdir build
cd build
cmake ..
make codegen codegen-quantize
```

## Getting The Models

### Direct Download

You can download the pre-converted, pre-quantized models from [Google Drive](https://drive.google.com/drive/folders/1wFy1Y0pqoK23ZeMWWCp8evxWOJQVdaGh?usp=sharing). I've made the `multi` flavour models with 2B and 6B parameters available - these models are pre-trained on  `C`, `C++`, `Go`, `Java`, `JavaScript`, and `Python`

### Convert The Models Yourself

Start by downloading either the [2B](https://huggingface.co/moyix/codegen-2B-multi-gptj) or [6B](https://huggingface.co/moyix/codegen-6B-multi-gptj) GPT-J versions of CodeGen.

You could also experiment with the other sizes of model such as [16B](https://huggingface.co/moyix/codegen-16B-multi-gptj) if you want or try the mono models ([2B](https://huggingface.co/moyix/codegen-2B-mono-gptj), [6B](https://huggingface.co/moyix/codegen-6B-mono-gptj), [16B](https://huggingface.co/moyix/codegen-16B-mono-gptj)) which are fine-tuned on python only but which outperform the `multi` models in some cases (see [the original paper](https://arxiv.org/pdf/2203.13474.pdf) for details).

You will also need to place [vocab.json](https://huggingface.co/Salesforce/codegen-2B-multi/raw/main/vocab.json) and [added_tokens.json](https://huggingface.co/Salesforce/codegen-2B-multi/blob/main/added_tokens.json) in the directory along with the model to make the conversion script work. This is a temporary limitation that I'll remove at some point.

You can directly `git clone` from huggingface URLS above. To save time you can disable LFS on first checkout and selectively pull the files you need (you only need the `.bin` files for conversion. The large `.zst` files are not needed). Here is an example:

```bash
GIT_LFS_SKIP_SMUDGE=1 git clone https://huggingface.co/moyix/codegen-16B-multi-gptj
git config lfs.fetchexclude "*.zst"
git lfs fetch
git lfs checkout *.bin
```

#### Install Python Dependencies

The `convert-codegen-to-ggml.py` requires Python 3 - I used `3.10`. Install the dependencies with `pip install -r requirements.txt`.

#### Convert The Model

```bash
python convert-codegen-to-ggml.py ./codegen-6B-multi-gptj 0
```

#### Quantize the Model

```bash
./bin/codegen-quantize ./codegen-6B-multi-gptj/ggml-model-f32.bin ./codegen-6B-multi-gptj/ggml-model-quant.bin 2
```


## Acknowledgements

- This project would not have been possible without [Georgi Gerganov's work on GGML and llama.cpp](https://github.com/ggerganov/ggml)
- It was completely inspired by [fauxpilot](https://github.com/fauxpilot/fauxpilot) which I did experiment with for a little while but wanted to try to make the models work without a GPU
- The frontend of the project is powered by [Venthe's vscode-fauxpilot plugin](https://github.com/Venthe/vscode-fauxpilot)
- The project uses the [Salesforce Codegen](https://github.com/salesforce/CodeGen) models.
- Thanks to [Moyix](https://huggingface.co/moyix) for his work on converting the Salesforce models to run in a GPT-J architecture. Not only does this [confer some speed benefits](https://gist.github.com/moyix/7896575befbe1b99162ccfec8d135566) but it also made it much easier for me to port the models to GGML using the [existing gpt-j example code](https://github.com/ggerganov/ggml/tree/master/examples/gpt-j)
- The model server uses [CrowCPP](https://crowcpp.org/master/) to serve suggestions.
- Check out the [original scientific paper](https://arxiv.org/pdf/2203.13474.pdf) for CodeGen for more info.