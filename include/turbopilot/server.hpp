#ifndef __TURBOPILOT_SERVER_H
#define __TURBOPILOT_SERVER_H


#include "turbopilot/model.hpp"

#include <crow.h>

crow::response serve_response(TurbopilotModel *model, const crow::request& req);

extern "C"
crow::response server_response(const crow::request& req)
{
    return serve_response(&model, req);
}


#endif // __TURBOPILOT_SERVER_H

