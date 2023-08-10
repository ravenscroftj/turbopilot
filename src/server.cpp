
#include "turbopilot/server.hpp"
#include "turbopilot/model.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.



/**
 * This function serves requests for autocompletion from crow
 * 
*/
crow::response handle_hf_request(TurbopilotModel *model, const crow::request& req){

        crow::json::rvalue data = crow::json::load(req.body);

        if(!data.has("inputs")){
            crow::response res;
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.body = "{\"message\":\"you must specify inputs field or\"}";
            return res;
        }

        // std::string suffix = data["suffix"].s();
        int maxTokens = 200;
        if(data.has("max_tokens")){
            maxTokens = data["max_tokens"].i();
        }


        auto result = model->predict(data["inputs"].s(), maxTokens, false);

        crow::json::wvalue response = {
            {"generated_text", result.str()},
        };
        

        
        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = response.dump(); //ss.str();
        return res;
}

/**
 * This function serves requests for autocompletion from crow
 * 
*/
crow::response handle_openai_request(TurbopilotModel *model, const crow::request& req){

        crow::json::rvalue data = crow::json::load(req.body);

        if(!data.has("prompt") && !data.has("input_ids")){
            crow::response res;
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.body = "{\"message\":\"you must specify a prompt or input_ids\"}";
            return res;
        }

        // std::string suffix = data["suffix"].s();
        int maxTokens = 200;
        if(data.has("max_tokens")){
            maxTokens = data["max_tokens"].i();
        }


        auto result = model->predict(data["prompt"].s(), maxTokens, false);


        boost::uuids::uuid uuid = boost::uuids::random_generator()();


        // Generate a mock response based on the input parameters
        crow::json::wvalue choice = {
            {"text", result.str()},
            {"index",0},
            {"finish_reason", "length"},
            {"logprobs", nullptr}
        };
        crow::json::wvalue::list choices = {choice};


        crow::json::wvalue usage = {
            {"completion_tokens", 0},
            // {"prompt_tokens", static_cast<std::uint64_t>(embd_inp.size())},
            {"prompt_tokens", 0},
            {"total_tokens", 0}
        };

        crow::json::wvalue response = {
            {"id", boost::lexical_cast<std::string>(uuid)},
            {"model", "codegen"},
            {"object","text_completion"},
            {"created", static_cast<std::int64_t>(std::time(nullptr))},
            {"choices", choices },
            {"usage", usage}
        };

        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");

        res.body = response.dump(); //ss.str();
        return res;
}