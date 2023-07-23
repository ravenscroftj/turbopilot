#include <iostream>
#include <cstdio>
#include <ggml/ggml.h>

#include <spdlog/spdlog.h>

#include <argparse/argparse.hpp>
#include "turbopilot/model.hpp"

#include "turbopilot/gptj.hpp"

int main(int argc, char **argv)
{

    argparse::ArgumentParser program("turbopilot");

    program.add_argument("-f", "--model-file")
        .help("Path to the model that turbopilot should serve")
        .required();

    program.add_argument("-t", "--model-type")
        .help("The type of model to load. Can be codegen/gpt-j or starcoder architectures.")
        .default_value("codegen");

    program.add_argument("-p", "--port")
        .help("The tcp port that turbopilot should listen on")
        .default_value("18080");

    program.add_argument("-r", "--random-seed")
        .help("Set the random seed for RNG functions")
        .default_value(-1)
        .scan<'i', int>();


    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    ggml_time_init();

    const int64_t t_main_start_us = ggml_time_us();


    TurbopilotModel *model = NULL;

    auto model_type = program.get<std::string>("--model-type");

    ModelConfig config{};
    std::mt19937 rng(program.get<int>("--random-seed"));

    if(model_type.compare("codegen") == 0) {
        spdlog::info("Initializing GPT-J type model for '{}' model", model_type);
        model = new GPTJModel(config, rng);
    }else{
        spdlog::error("Invalid model type: {}", model_type);
    }

    spdlog::info("Attempt to load model from {}", program.get<std::string>("--model-type"));
    int64_t t_load_us = 0;
    const int64_t t_start_us = ggml_time_us();
    auto loaded = model->load_model(program.get<std::string>("--model-file"));

    if(!loaded){
        spdlog::error("Failed to load model");
        return -1;
    }

    t_load_us = ggml_time_us() - t_start_us;

    spdlog::info("Loaded model in {:0.2f}ms", t_load_us/1000.0f);

    auto result = model->predict("test", 100);

    spdlog::info("output: {}", result.str());

    free(model);
}