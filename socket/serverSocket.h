#pragma once

#include "socket.h"
#include <string>

namespace web_rpc{
namespace socket{

class ServerSocket : public Socket
{
private:
    /* data */
public:
    ServerSocket(/* args */);
    virtual ~ServerSocket();
    ServerSocket(const std::string & ip,int port);



};



}
}
