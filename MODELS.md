# Models Directory

## StableCode Instruct State-of-the-art for low Spec machines(Released 8th August 2023)

[StableCode](https://stability.ai/blog/stablecode-llm-generative-ai-coding) Instruct is a new model from [Stability.ai](https://stability.ai/) which provides reasonable autocomplete suggestions in approx 3GiB of RAM.

| Model Name          | RAM Requirement | Direct Download  | HF Project Link |
|---------------------|-----------------|-----------------|-----------------|
| StableCode   | ~3GiB        |   [:arrow_down:](https://huggingface.co/TheBloke/stablecode-instruct-alpha-3b-GGML/blob/main/stablecode-instruct-alpha-3b.ggmlv1.q4_0.bin)           |   [:hugs:](https://huggingface.co/TheBloke/stablecode-instruct-alpha-3b-GGML/)           |

To run in Turbopilot set model type `-m stablecode`

## "Coder" family models

WizardCoder, StarCoder and SantaCoder are current "state-of-the-art" autocomplete models 

### SantaCoder (Small Model, Reasonable on lower spec machines - Released 13/4/2023)

[SantaCoder](https://huggingface.co/bigcode/santacoder) is a smaller version of the StarCoder and WizardCoder family with only 1.1 Billion parameters. The model is trained with fill-in-the-middle objective allowing it to be used to auto-complete function parameters.

This model is primarily trained on Python, Java and Javscript.


| Model Name          | RAM Requirement | Direct Download  | HF Project Link |
|---------------------|-----------------|-----------------|-----------------|
| SantaCoder   | ~2GiB        |   [:arrow_down:](https://huggingface.co/mike-ravkine/gpt_bigcode-santacoder-GGML/resolve/main/santacoder-q4_0.bin)           |   [:hugs:](https://huggingface.co/mike-ravkine/gpt_bigcode-santacoder-GGML/)           |

To run in Turbopilot set model type `-m starcoder`


### WizardCoder 15B Best Autocomplete Performance, Compute-Hungry (Released 15/6/2023)

[WizardCoder](https://github.com/nlpxucan/WizardLM/tree/main/WizardCoder) is the current SOTA auto complete model, it is an updated version of StarCoder that achieves 57.1 pass@1 on HumanEval benchmarks (essentially in 57% of cases it correctly solves a given challenge. Read more about how this metric works in the scientific paper [here](https://arxiv.org/pdf/2107.03374.pdf) ).

Even when quantized, WizardCoder is a large model that takes up a significant amount of RAM.


| Model Name          | RAM Requirement | Direct Download  | HF Project Link |
|---------------------|-----------------|-----------------|-----------------|
| WizardCoder   | ~12GiB        |   [:arrow_down:](https://huggingface.co/TheBloke/WizardCoder-15B-1.0-GGML/resolve/main/WizardCoder-15B-1.0.ggmlv3.q4_0.bin)           |   [:hugs:](https://huggingface.co/TheBloke/WizardCoder-15B-1.0-GGML/)           |

To run in Turbopilot set model type `-m wizardcoder`


### StarCoder (Released 4/5/2023)

[StarCoder](https://huggingface.co/blog/starcoder) held the previous title of state-of-the-art coding model back in May 2023. It is still a reasonably good model by comparison but it is a similar size and has similar RAM and compute requirements to WizardCoder so you may be better off just running that. Links below provided for posterity.


| Model Name          | RAM Requirement | Direct Download  | HF Project Link |
|---------------------|-----------------|------------------|-----------------|
| StarCoder   | ~12GiB        | [:arrow_down:](https://huggingface.co/NeoDim/starcoder-GGML/resolve/main/starcoder-ggml-q4_0.bin)           |   [:hugs:](https://huggingface.co/NeoDim/starcoder-GGML/)           |
| StarCoder Plus   | ~12GiB        | [:arrow_down:](https://huggingface.co/TheBloke/starcoderplus-GGML/resolve/main/starcoderplus.ggmlv3.q4_0.bin)           |   [:hugs:](https://huggingface.co/TheBloke/starcoderplus-GGML/)           |

To run in Turbopilot set model type `-m starcoder`






## CodeGen 1.0

The CodeGen models were the first models supported by Turbopilot. They perform less well than the newer Wizardcoder/Starcoder/Santacoder variant models.


The `multi` flavour models can provide auto-complete suggestions for `C`, `C++`, `Go`, `Java`, `JavaScript`, and `Python`.

The `mono` flavour models can provide auto-complete suggestions for `Python` only (but the quality of Python-specific suggestions may be higher).

Pre-converted and pre-quantized models are available for download from here:

| Model Name          | RAM Requirement | Supported Languages       | Direct Download  | HF Project Link |
|---------------------|-----------------|---------------------------|-----------------|-----------------|
| CodeGen 350M multi   | ~800MiB        | `C`, `C++`, `Go`, `Java`, `JavaScript`, `Python`  |   [:arrow_down:](https://huggingface.co/ravenscroftj/CodeGen-350M-multi-ggml-quant/resolve/main/codegen-350M-multi-ggml-4bit-quant.bin)           |   [:hugs:](https://huggingface.co/ravenscroftj/CodeGen-350M-multi-ggml-quant)           |
| CodeGen 350M mono   | ~800MiB   | `Python`          |   [:arrow_down:](https://huggingface.co/Guglielmo/CodeGen-350M-mono-ggml-quant/resolve/main/ggml-model-quant.bin)           |   [:hugs:](https://huggingface.co/Guglielmo/CodeGen-350M-mono-ggml-quant)           |
| CodeGen 2B multi   | ~4GiB  | `C`, `C++`, `Go`, `Java`, `JavaScript`, `Python`          |   [:arrow_down:](https://huggingface.co/ravenscroftj/CodeGen-2B-multi-ggml-quant/resolve/main/codegen-2B-multi-ggml-4bit-quant_q4_0.bin)           |   [:hugs:](https://huggingface.co/ravenscroftj/CodeGen-2B-multi-ggml-quant)          |
| CodeGen 2B mono   | ~4GiB  | `Python`          |   [:arrow_down:](https://huggingface.co/Guglielmo/CodeGen-2B-mono-ggml-quant/resolve/main/ggml-model-quant.bin)           |   [:hugs:](https://huggingface.co/Guglielmo/CodeGen-2B-mono-ggml-quant/)          |
| CodeGen 6B multi   | ~8GiB  | `C`, `C++`, `Go`, `Java`, `JavaScript`, `Python`          |   [:arrow_down:](https://huggingface.co/ravenscroftj/CodeGen-6B-multi-ggml-quant/resolve/main/codegen-6B-multi-ggml-4bit-quant.bin)           |   [:hugs:](https://huggingface.co/ravenscroftj/CodeGen-6B-multi-ggml-quant)          |
| CodeGen 6B mono   | ~8GiB  | `Python`          |   [:arrow_down:](https://huggingface.co/Guglielmo/CodeGen-6B-mono-ggml-quant/resolve/main/ggml-model-quant.bin)           |   [:hugs:](https://huggingface.co/Guglielmo/CodeGen-6B-mono-ggml-quant/)          |
