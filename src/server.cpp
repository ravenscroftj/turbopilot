
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
crow::response serve_response(TurbopilotModel *model, const crow::request& req){

        crow::json::rvalue data = crow::json::load(req.body);

        if(!data.has("prompt") && !data.has("input_ids")){
            crow::response res;
            res.code = 400;
            res.set_header("Content-Type", "application/json");
            res.body = "{\"message\":\"you must specify a prompt or input_ids\"}";
            return res;
        }




        // tokenize the prompt
        // std::vector<gpt_vocab::id> embd_inp;
        
        // if (data.has("prompt")) {
        //     std::string prompt = data["prompt"].s();
        //     embd_inp = ::gpt_tokenize(vocab, prompt);
        // }
        //  else {
        //     crow::json::rvalue input_ids = data["input_ids"];
        //     for (auto id : input_ids.lo()) {
        //         embd_inp.push_back(id.i());
        //     }
        // }


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


        // crow::json::wvalue usage = {
        //     {"completion_tokens", n_past},
        //     // {"prompt_tokens", static_cast<std::uint64_t>(embd_inp.size())},
        //     {"prompt_tokens", 0},
        //     {"total_tokens", static_cast<std::uint64_t>(n_past - embd_inp.size())}
        // };


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