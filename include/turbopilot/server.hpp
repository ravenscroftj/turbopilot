#ifndef __TURBOPILOT_SERVER_H
#define __TURBOPILOT_SERVER_H


#include "turbopilot/model.hpp"

#include "crow_all.h"

crow::response serve_response(TurbopilotModel *model, const crow::request& req);



#endif // __TURBOPILOT_SERVER_H

