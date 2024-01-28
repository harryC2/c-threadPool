#pragma once

#include "../socket/socket.h"
using namespace web_rpc::socket;

#include "../thread/task.h"
using namespace web_rpc::thread_poll;

#include "workTask.h"
using namespace web_rpc::task;



namespace web_rpc{
namespace task{

class TaskFactory
{
public:
    static Task* createTask(Socket* pSocket)
    {
         Task* ptask =  new WorkTask(pSocket);
         return ptask;
    };
};


}
}
