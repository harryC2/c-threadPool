
/*
    此文件作用是socket类 实现bind listen connect cloase accept recv send 等
*/
#pragma once
#include <iostream>

#include <sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<string>
#include"string.h"
#include "../utility/logger.h"

using namespace std;
using namespace web_rpc::utility;

namespace web_rpc{
namespace socket{

class Socket
{
private:
    /* data */
public:
    Socket(/* args */);
    Socket(const string& ip, int port);
    virtual ~Socket();

    bool bind(const string& ip,int port);
    bool listen(int backLog);
    bool connect(const string& ip,int port);
    bool close();

    int accept();
    int recv(char* buf,int len);
    int send(const char* buf,int len);

    bool set_non_blocking();
    bool set_send_buffer(int size);
    bool set_recv_buffer(int size);
    bool set_linger(bool active,int seconds);
    bool set_keep_alive();
    bool set_reuse_addr();
    bool set_reuse_port();
    void set_sockfd(int fd)
    {
        m_sockfd = fd;
    };
    int  get_sockfd()
    {
        return m_sockfd;
    };
protected:
  string m_ip;
  int m_port{0};
  int m_sockfd{0};

};



}
}


