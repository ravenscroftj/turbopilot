#ifndef __TURBOPILOT_SERVER_H
#define __TURBOPILOT_SERVER_H


#include <spdlog/spdlog.h>

#include "turbopilot/model.hpp"

#include "crow_all.h"

crow::response handle_openai_request(TurbopilotModel *model, const crow::request& req);

crow::response handle_hf_request(TurbopilotModel *model, const crow::request& req);

class TBPLogger : public crow::ILogHandler {
 public:
  TBPLogger() {}
  void log(std::string message, crow::LogLevel crow_level) {
    // "message" doesn't contain the timestamp and loglevel
    // prefix the default logger does and it doesn't end
    // in a newline.

    spdlog::level::level_enum level = spdlog::level::info;

    switch(crow_level){
        case crow::LogLevel::Critical:
            level = spdlog::level::critical;
            break;

        case crow::LogLevel::Error:
            level = spdlog::level::err;
            break;

        case crow::LogLevel::Warning:
            level = spdlog::level::warn;
            break;

        case crow::LogLevel::Info:
            level = spdlog::level::info;
            break;

        case crow::LogLevel::Debug:
            level = spdlog::level::debug;
            break;

        default:
            // if case is not a known value, assume the worst
            level = spdlog::level::critical;
    }

    spdlog::log(level, message);
  }
};


#endif // __TURBOPILOT_SERVER_H

