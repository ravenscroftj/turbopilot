# TurboPilot

TurboPilot is a self-hosted [copilot](https://github.com/features/copilot) clone which uses the library behind [llama.cpp](https://github.com/comex/llama.cpp) to run huge [6 Billion Parameter Salesforce Codegen models](https://github.com/salesforce/CodeGen) in 2GiB of RAM. It is heavily based and inspired by on the [fauxpilot](https://github.com/fauxpilot/fauxpilot) project.

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

Start by downloading either the [2B](https://huggingface.co/moyix/codegen-2B-multi-gptj) or [6B](https://huggingface.co/moyix/codegen-6B-multi-gptj) GPT-J versions of CodeGen.

## Convert The Model

```bash
python convert-codegen-to-ggml.py ./codegen-6B-multi-gptj 0
```

## Quantize the model

```bash
./bin/codegen-quantize ../../codegen-6B-multi-gptj/ggml-model-f32.bin ../../codegen-6B-multi-gptj/ggml-model-quant.bin 2
```

## Run the model

```bash
./bin/codegen -t 6 -m ../../codegen-6B-multi-gptj/ggml-model-quant.bin -p "def main("
```