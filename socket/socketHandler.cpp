#include "socketHandler.h"
using namespace web_rpc::socket;



void SocketHandler::attach(Socket * socket)
{
    AutoMutex lock(&m_mutex);
    m_epoll->add(socket->get_sockfd(), (void *)socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::detach(Socket * socket)
{
    AutoMutex lock(&m_mutex);
    m_epoll->del(socket->get_sockfd(), (void *)socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::remove(Socket * socket)
{
    socket->close();
    m_sockpool.release(socket);
}


void SocketHandler::handle(int max_connections, int wait_time)
{
    // 创建epoll管理类 模型
    m_epoll = new EventPoller(false);
    // 创建epoll文件描述符及事件集合
    m_epoll->create(max_connections);
    // 将当前服务端socke监听新链接请求 挂到epoll管理中// 描述符，类地址存放于时间ptr中，监听事件信号集合
    m_epoll->add(m_server->get_sockfd(), m_server, (EPOLLIN | EPOLLHUP | EPOLLERR));
    // 当前监听socket*的，创建保存连接请求回来，新建的套接字
    m_sockpool.init(max_connections);

    debug("epoll wait time: %dms", wait_time);
    while (true)
    {
        int num = m_epoll->wait(wait_time);
        if (num == 0)
        {
            // debug("no events");
            continue;
        }
        for (int i = 0; i < num; i++)
        {
            // epoll  wait  将有多少触发就将触发ptr events赋值在m_events[] 数组中
            if (m_server == static_cast<Socket *>(m_epoll->m_events[i].data.ptr))
            {
                debug("socket accept event");
                int soctfd = m_server->accept();
                Socket * socket = m_sockpool.allocate();
                if (socket == NULL)
                {
                    error("socket pool is empty");
                    break;
                }
                socket->set_sockfd(soctfd);
                // 设置非阻塞模式
                socket->set_non_blocking();
                // 将当前返回的新连接 文件描述符挂到epoll监听中 add()
                attach(socket);
            }
            else
            {
                Socket * socket = static_cast<Socket *>(m_epoll->m_events[i].data.ptr);
                if (m_epoll->m_events[i].events & EPOLLHUP)
                {
                    error("socket %d closed by peer.", socket->get_sockfd());
                    detach(socket);
                    remove(socket);
                }
                else if (m_epoll->m_events[i].events & EPOLLERR)
			    {
                    error("socket %d error.", socket->get_sockfd());
                    detach(socket);
                    remove(socket);
			    }
                else if (m_epoll->m_events[i].events & EPOLLIN)
                {
                    debug("socket read event");
                    detach(socket);
                    Task * task = TaskFactory::createTask(socket);
                    TaskDispatcher::getInstance().assign(task);
                }
            }
        }
    }
}

SocketHandler::SocketHandler(/* args */)
{
}

SocketHandler::~SocketHandler()
{
}
void SocketHandler::listen(const string & ip, int port)
{
    m_server = new ServerSocket(ip, port);
}
