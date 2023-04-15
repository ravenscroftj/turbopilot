# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

- Added multi-threaded server support which should prevent health checks aimed at `GET /` from failing during prediction.


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