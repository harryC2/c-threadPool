/*
    此文件是ClientSocket, 继承socket  实现链接服务端ip 和端口的作用
*/
#pragma once
#include<winsock2.h>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;


#include "../serialize/DataStream.h"
using namespace web_rpc::serialize;



namespace web_rpc{
namespace socket{

struct MsgHead {
    char flag[8];
    uint32_t cmd;
    uint32_t len;
};

const uint32_t send_buff_size = 1024;
const uint32_t recv_buff_size = 1024;
class Client
{
public:
    Client();
    ~Client();

    bool connect(const string & ip, int port);
    static bool initSocket();
    static void CleanSocket();
    template <typename R, typename ...Args>
    R call(const string & name, const Args&... args);

private:
    string m_ip;
    int m_port;
    SOCKET clientSocket;
};

template <typename R, typename ...Args>
R Client::call(const string & name, const Args&... args)
{
    DataStream in;
    in << name;
    in.write_args(args...);

    MsgHead head;
    strcpy(head.flag, "work");
    head.cmd = 0;
    head.len = in.size();

    char send_buff[send_buff_size];
    std::memcpy(send_buff, (char *)&head, sizeof(MsgHead));
    std::memcpy(send_buff + sizeof(MsgHead), in.data(), in.size());

    int len = sizeof(MsgHead) + in.size();

    ::send(clientSocket,send_buff,len,0);
    //clientSocket.send(send_buff, len);

    char recv_buff[recv_buff_size];
    //int recv_size = clientSocket.recv(recv_buff, recv_buff_size);
    int recv_size  = ::recv(clientSocket,recv_buff,recv_buff_size,0);
    DataStream out;
    out.write(recv_buff, recv_size);

    R v;
    out >> v;
    return v;
}


}
}
