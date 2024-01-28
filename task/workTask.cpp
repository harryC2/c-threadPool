#include "workTask.h"
using namespace web_rpc::task;

#include "../utility/logger.h"
using namespace web_rpc::utility;

#include "../serialize/dataStream.h"
using namespace web_rpc::serialize;

#include "../socket/socketHandler.h"
using namespace web_rpc::socket;

#include "../webRpc/server.h"
using namespace web_rpc::webrpc;

#include "string.h"
#include<cstring>

WorkTask::WorkTask(Socket* socket):Task(socket)
{
    
}
WorkTask::~WorkTask()
{
    
}
void WorkTask::run()
{
    // socke 要处理的任务
    debug("work task run");
    
    Socket* pSocket = static_cast<Socket*>(m_data);
    if(pSocket == nullptr)
    {
        return;
    }

    MsgHead msg_head;
    memset(&msg_head,0,sizeof(MsgHead));

    int len = pSocket->recv((char *)(&msg_head),sizeof(MsgHead));
    if(len == 0)
    {
        warn("socket cloasr by peer");
        SocketHandler::getInstance().remove(pSocket);
        return;
    }
    else if (len == -1 && errno == EAGAIN)
    {
        warn("socket recv len: %d, error msg: EAGAIN errno: %d", len, errno);
        SocketHandler::getInstance().attach(pSocket);
        return;
    }
   else if (len == -1 && errno == EWOULDBLOCK)
    {
        warn("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
        SocketHandler::getInstance().attach(pSocket);
        return;
    }
   else if (len == -1 && errno == EINTR)
    {
        warn("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
        SocketHandler::getInstance().attach(pSocket);
        return;
    }
    else if (len != sizeof(msg_head))
    {
        error("recv msg head error length: %d, errno: %d", len, errno);
        SocketHandler::getInstance().remove(pSocket);
        return;
    }
    info("recv msg head len: %d, flag: %s, cmd: %d, body len: %d", len, msg_head.flag, msg_head.cmd, msg_head.len);
    if (std::strncmp(msg_head.flag, "work", 4) != 0)
    {
        error("msg head flag error");
        SocketHandler::getInstance().remove(pSocket);
        return;
    }
    if (msg_head.len >= uint32_t(recv_buff_size))
    {
        error("recv msg body len: %d, large than recv_buff_size: %d", msg_head.len, recv_buff_size);
        SocketHandler::getInstance().remove(pSocket);
        return;
    }

    char buf[recv_buff_size];
    std::memset(&buf,0,recv_buff_size);
    len = pSocket->recv(buf, msg_head.len);
   if (len == -1 && errno == EAGAIN)
    {
        warn("socket recv len: %d, error msg: EAGAIN errno: %d", len, errno);
         SocketHandler::getInstance().remove(pSocket);
        return;
    }
    if (len == -1 && errno == EWOULDBLOCK)
    {
        warn("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
        SocketHandler::getInstance().remove(pSocket);
        return;
    }
    if (len == -1 && errno == EINTR)
    {
        warn("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
         SocketHandler::getInstance().remove(pSocket);
        return;
    }
    if (len != (int)(msg_head.len))
    {
        error("recv msg body error length: %d, body: %s, errno: %d", len, buf, errno);
        SocketHandler::getInstance().remove(pSocket);
        return;
    }
     info("recv msg body len: %d, msg data: %s", len, buf);
     DataStream in;
     in.write(buf,len);
     
     // 先写的名字，后写的参数； 参数在调用call的时候再取出
     std::string func;
     in>>func;
     DataStream out;
     Server::getInstance().call(func,in,out);
     pSocket->send(out.data(),out.size());    
     SocketHandler::getInstance().attach(pSocket);
}

void WorkTask::destory()
{
    if(m_data != nullptr)
    {
        delete this;
    }
    
}

