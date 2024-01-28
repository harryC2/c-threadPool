#pragma once
#include "socket.h"
#include "../thread/mutex.h"
#include "eventPoller.h"
#include "serverSocket.h"
#include "../utility/objectPool.h"
#include "../thread/taskdispatcher.h"
using namespace web_rpc::thread_poll;

#include "../utility/logger.h"
using namespace web_rpc::utility;

#include "../task/taskFactory.h"
using namespace web_rpc::task;

namespace web_rpc{
namespace socket{
class SocketHandler
{
private:
    SocketHandler(/* args */);
    SocketHandler(SocketHandler&& other) = delete;
    SocketHandler& operator= (SocketHandler&& other) = delete;
    ~SocketHandler();
public:
    static SocketHandler& getInstance()
    {
        static SocketHandler socketHandle;
        return socketHandle;
    }

    void listen(const string& ip,int port);
    void attach(Socket* socket);
    void detach(Socket* socker);
    void remove(Socket* socker);
    void handle(int max_connections,int wait_time);
private:
    EventPoller * m_epoll{nullptr};// epoll 类多线程访问
    Socket* m_server{nullptr};// socket 类
    Mutex m_mutex; // 多线程操作epoll互斥锁
    ObjectPool<Socket> m_sockpool; // 当前epoll监听事件的 socket集合
};

}
}
