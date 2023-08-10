#ifndef __TURBOPILOT_SERVER_H
#define __TURBOPILOT_SERVER_H


#include "turbopilot/model.hpp"

#include "crow_all.h"

crow::response handle_openai_request(TurbopilotModel *model, const crow::request& req);

crow::response handle_hf_request(TurbopilotModel *model, const crow::request& req);


#endif // __TURBOPILOT_SERVER_H

