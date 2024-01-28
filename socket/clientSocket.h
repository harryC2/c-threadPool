/*
    此文件是ClientSocket, 继承socket  实现链接服务端ip 和端口的作用
*/
#pragma once
#ifdef CLIENT
#include "socket.h"
#include <string.h>

namespace web_rpc{
namespace socket{

class ClientSocket : public Socket
{
public:
    ClientSocket();
    ClientSocket(const std::string &ip, int port);
    virtual ~ClientSocket();
};

}
}
#endif 
