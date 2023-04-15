# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.0.4] 2023-04-14

- Added multi-threaded server support which should prevent health checks aimed at `GET /` from failing during prediction.
- Separated autocomplete lambda into a separate C++ function so that it can be bound to `/v1/completions`, `/v1/engines/copilot-codex/completions` and `/v1/engines/codegen/completions`
- Removed `model` from completion input as required param which stops the official copilot plugin from freaking out
- Integrate latest changes from upstream ggml including some fixes for ARM NEON processor
- Added Mac builds as part of CI
- Support for fork of [vscode-fauxpilot](https://github.com/ravenscroftj/vscode-fauxpilot) with a progress indicator is now available ([PR](https://github.com/Venthe/vscode-fauxpilot/pull/26) is open upstream, please react/vote for it).


## [0.0.3] 2023-04-13

- Added 350M parameter codegen model to Google Drive folder
- Added multi-arch docker images so that users can now directly run on Apple silicon and even raspberry pi
- Now support pre-tokenized inputs passed into the API from a Python tokenizer (thanks to @thakkarparth007 for their PR - https://github.com/ravenscroftj/ggml/pull/2)


## [0.0.2] - 2023-04-12

- Project now builds on Mac OS (Thanks to @Dimitrije-V for their PR https://github.com/ravenscroftj/ggml/pull/1 and @dabdine for contributing some clearer Mac build instructions)
- Fix inability to load vocab.json on converting the 16B model due to encoding of the file not being set by @swanserquack in #5
- Improve performance of model by incorporating changes to GGML library from @ggerganov


## [0.0.1] 2023-04-10

- Turbopilot is born!