# Build TurboPilot

TurboPilot is a C++ program that uses the [GGML](https://github.com/ggerganov/ggml) project to parse and run language models.

### Dependencies

To build turbopilot you will need CMake, Libboost, a C++ toolchain and GNU Make.

#### Ubuntu

On Ubuntu you can install these things with:

```bash
sudo apt-get update
sudo apt-get install libboost-dev cmake build-essential
```

#### MacOS

If you use [brew](https://brew.sh/) you can simply add these dependencies by running:

```bash
brew install cmake boost
```

### Checkout Submodules

Make sure the ggml subproject is checked out with `git submodule init` and `git submodule update`

### Prepare and Build

Configure cmake to build the project with the following:

```bash
mkdir ggml/build
cd ggml/build
cmake ..
```

If you are running on linux you can optionally compile a static build with `cmake -D CMAKE_EXE_LINKER_FLAGS="-static" ..` which should make your binary portable across different flavours of the OS.

From here you can now build the components that make up turbopilot:

```bash
make codegen codegen-quantize codegen-serve
```

Where:

- *codegen* is a command line tool for testing out prompts in a lightweight way (a lot like llama.cpp)
- *codegen-serve* is the actual REST server that can be used to connect to VSCode
- *codegen-quantize* is the tool for quantizing models exported by the conversion script. For more details see [Converting and Quantizing The Models](https://github.com/ravenscroftj/turbopilot/wiki/Converting-and-Quantizing-The-Models).
