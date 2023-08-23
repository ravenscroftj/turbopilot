#include <iostream>
#include <cstdio>
#include <ggml/ggml.h>

#include <spdlog/spdlog.h>

#include <turbopilot/crow_all.h>

#include <argparse/argparse.hpp>

#include "turbopilot/model.hpp"
#include "turbopilot/starcoder.hpp"
#include "turbopilot/gptj.hpp"
#include "turbopilot/gptneox.hpp"
#include "turbopilot/server.hpp"


#define TURBOPILOT_VERSION "1.1.0"

int main(int argc, char **argv)
{

    argparse::ArgumentParser program("turbopilot", TURBOPILOT_VERSION);

    program.add_argument("--debug")
        .default_value(false)
        .help("Output verbose logs and timings")
        .implicit_value(true);

    program.add_argument("-f", "--model-file")
        .help("Path to the model that turbopilot should serve")
        .required();

    program.add_argument("-m", "--model-type")
        .help("The type of model to load. Can be codegen,starcoder,wizardcoder,stablecode")
        .default_value("codegen");

    program.add_argument("-t", "--threads")
        .help("The number of CPU threads turbopilot is allowed to use. Defaults to 4")
        .default_value(4)
        .scan<'i', int>();

    program.add_argument("-p", "--port")
        .help("The tcp port that turbopilot should listen on")
        .default_value(18080)
        .scan<'i', int>();

    program.add_argument("-r", "--random-seed")
        .help("Set the random seed for RNG functions")
        .default_value(-1)
        .scan<'i', int>();

    program.add_argument("--temperature")
        .help("Set the generation temperature")
        .default_value(0.2f)
        .scan<'g', float>();

    program.add_argument("--top-p")
        .help("Set the generation top_p")
        .default_value(0.1f)
        .scan<'g', float>();


    program.add_argument("prompt").remaining();



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

    if(program.get<bool>("--debug")){
        spdlog::set_level(spdlog::level::level_enum::debug);
        spdlog::debug("debug logging enabled");
    }

    ggml_time_init();

    const int64_t t_main_start_us = ggml_time_us();


    TurbopilotModel *model = NULL;

    auto model_type = program.get<std::string>("--model-type");

    ModelConfig config{};
    std::mt19937 rng(program.get<int>("--random-seed"));

    config.n_threads = program.get<int>("--threads");
    config.temp = program.get<float>("--temperature");
    config.top_p = program.get<float>("--top-p");

    if(model_type.compare("codegen") == 0) {
        spdlog::info("Initializing GPT-J type model for '{}' model", model_type);
        model = new GPTJModel(config, rng);
    }else if(model_type.compare("starcoder") == 0 || model_type.compare("wizardcoder") == 0){
        spdlog::info("Initializing Starcoder/Wizardcoder type model for '{}' model type", model_type);
        model = new StarcoderModel(config, rng);
    }else if(model_type.compare("stablecode") == 0){
        spdlog::info("Initializing StableLM type model for '{}' model type", model_type);
        model = new GPTNEOXModel(config, rng);
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


    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    CROW_ROUTE(app, "/copilot_internal/v2/token")([](){
        //return "Hello world";

        crow::json::wvalue response = {{"token","1"}, {"expires_at", static_cast<std::uint64_t>(2600000000)}, {"refresh_in",900}};

        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = response.dump();
        return res;
    });

    //huggingface code compatible endpoint
    CROW_ROUTE(app, "/api/generate").methods(crow::HTTPMethod::Post)
    ([&model](const crow::request& req) {
        return handle_hf_request(model, req);
    });

    CROW_ROUTE(app, "/v1/completions").methods(crow::HTTPMethod::Post)
    ([&model](const crow::request& req) {
        return handle_openai_request(model, req);
    });

    CROW_ROUTE(app, "/v1/engines/codegen/completions").methods(crow::HTTPMethod::Post)
    ([&model](const crow::request& req) {
        return handle_openai_request(model, req);
    });


    CROW_ROUTE(app, "/v1/engines/copilot-codex/completions").methods(crow::HTTPMethod::Post)
    ([&model](const crow::request& req) {
        return handle_openai_request(model, req);
    });
    

    app.port(program.get<int>("--port")).multithreaded().run();

    free(model);
}