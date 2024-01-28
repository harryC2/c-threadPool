 /*
    此文件定义了Server类，是服务类完成一些服务绑定
*/
#pragma once
#include <string>
using namespace std;
#include "../serialize/dataStream.h"
using namespace web_rpc::serialize;

#include "functionHandler.h"

namespace web_rpc{
namespace webrpc{

class Server
{
public:
   static Server& getInstance()
   {
    static Server server;
    return server;
   }
private:
    Server(Server&& other) =delete;
    Server& operator= (const Server&& other) =delete;
    Server();
    ~Server();

public:
    void listen(const std::string& ip,int port);
    void start();
    void set_threads(int threads);
    void set_connects(int connects);
    void set_wait_time(int wait_time);

    //std::string (*)(const std::string &name)  函数指针类型 ， 返回值string  c参数 const string 
    // inline void web_rpc::webrpc::Server::
    // bind<std::string (*)(const std::string &name)>
    // (const std::string &funcName, std::string (*func)(const std::string &name))

    template<class F>
    void bind(const string& funcName,F func)
    {
        m_handler.bind(funcName, func);
    }
    void call(const string& funcName,DataStream& in,DataStream& out)
    {
        m_handler.call(funcName, in, out);
    }
private:
    string m_ip;
    int m_port;
    int m_threads;
    int m_connects;
    int m_wait_time;
    FunctionHandler m_handler;
};



}
}