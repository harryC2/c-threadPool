/*
   此文件封装了taskdispatcher.h类 单例，这个是一个任务分发线程
   接受主线程的任务，将任务分发给线程池去完成
*/


#pragma once 
#include "thread.h"
#include "task.h"
#include "threadPool.h"
#include <list>
#include "mutex.h"



namespace web_rpc{
namespace thread_poll{

class TaskDispatcher : public Thread
{
public:
   static TaskDispatcher& getInstance()
   {
    static TaskDispatcher stTaskDisapatcher;
    return stTaskDisapatcher;
   }
   void initTaskDispatcher();
   void assign(Task* task);
   virtual void run() override;

private:
    TaskDispatcher(/* args */);
    ~TaskDispatcher();

private:
    std::list<Task*> m_listTask;
};

}
}