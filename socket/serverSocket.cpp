#include"serverSocket.h"

using namespace web_rpc::socket;

ServerSocket::ServerSocket(const std::string & ip,int port): Socket(ip, port)
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0)
    {
        error("create server socket error: errno=%d errstr=%s", errno, strerror(errno));
        return;
    }
    // 非阻塞模式
    set_non_blocking();
    // 设置接收数据大小
    set_recv_buffer(10 * 1024);
    // 设置发送数据大小
    set_send_buffer(10 * 1024);
    // 关闭套接字得等待数据发送完
    set_linger(true, 0);
    // 设置保活
    set_keep_alive();
    // 设置地址重用
    set_reuse_addr();
    // 绑定ip
    bind(ip, port);
    // 监听最大数量1024
    listen(1024);

}
ServerSocket::ServerSocket(/* args */): Socket()
{
}

ServerSocket::~ServerSocket()
{
    close();
}

