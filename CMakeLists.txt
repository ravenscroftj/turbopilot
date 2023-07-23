cmake_minimum_required (VERSION 3.0)
project(turbopilot VERSION 0.1.0)


add_subdirectory(extern/ggml)
add_subdirectory(extern/argparse)
add_subdirectory(extern/spdlog)
add_subdirectory(src)