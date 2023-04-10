# TurboPilot

TurboPilot is a self-hosted [copilot](https://github.com/features/copilot) clone which uses the library behind [llama.cpp](https://github.com/comex/llama.cpp) to run the [6 Billion Parameter Salesforce Codegen model](https://github.com/salesforce/CodeGen) in 4GiB of RAM. It is heavily based and inspired by on the [fauxpilot](https://github.com/fauxpilot/fauxpilot) project.

***NB: This is a proof of concept right now rather than a stable tool. Autocompletion is quite slow in this version of the project. Feel free to play with it, but your mileage may vary.***

![a screen recording of turbopilot running through fauxpilot plugin](assets/screenrecording.gif)

## Getting Started

The easiest way to try the project out is to grab the pre-processed models and then run the server in docker.

### Getting The Models

#### Direct Download

You can download the pre-converted, pre-quantized models from [Google Drive](https://drive.google.com/drive/folders/1wFy1Y0pqoK23ZeMWWCp8evxWOJQVdaGh?usp=sharing). I've made the `multi` flavour models with 2B and 6B parameters available - these models are pre-trained on  `C`, `C++`, `Go`, `Java`, `JavaScript`, and `Python`

#### Convert The Models Yourself

Follow [this guide](https://github.com/ravenscroftj/turbopilot/wiki/Converting-and-Quantizing-The-Models) if you want to experiment with quantizing the models yourself.

### Running TurboPilot Server

Download the [latest binary](https://github.com/ravenscroftj/turbopilot/releases) and extract it to the root project folder. If a binary is not provided for your OS or you'd prefer to build it yourself follow the [build instructions](BUILD.md)

Run:

```bash
./codegen-serve -m ./models/codegen-6B-multi-ggml-4bit-quant.bin
```

The application should start a server on port `18080`







## Acknowledgements

- This project would not have been possible without [Georgi Gerganov's work on GGML and llama.cpp](https://github.com/ggerganov/ggml)
- It was completely inspired by [fauxpilot](https://github.com/fauxpilot/fauxpilot) which I did experiment with for a little while but wanted to try to make the models work without a GPU
- The frontend of the project is powered by [Venthe's vscode-fauxpilot plugin](https://github.com/Venthe/vscode-fauxpilot)
- The project uses the [Salesforce Codegen](https://github.com/salesforce/CodeGen) models.
- Thanks to [Moyix](https://huggingface.co/moyix) for his work on converting the Salesforce models to run in a GPT-J architecture. Not only does this [confer some speed benefits](https://gist.github.com/moyix/7896575befbe1b99162ccfec8d135566) but it also made it much easier for me to port the models to GGML using the [existing gpt-j example code](https://github.com/ggerganov/ggml/tree/master/examples/gpt-j)
- The model server uses [CrowCPP](https://crowcpp.org/master/) to serve suggestions.
- Check out the [original scientific paper](https://arxiv.org/pdf/2203.13474.pdf) for CodeGen for more info.