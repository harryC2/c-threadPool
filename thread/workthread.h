/*
   此文件目的是继承thread 基类线程，封装一个工作线程，
    运行线程池委派到的任务
*/
#pragma once 
#include "thread.h"

namespace web_rpc{
namespace thread_poll{

class WorkerThread : public Thread
{
public: 
  WorkerThread();
  virtual ~WorkerThread();
public:
  void run() override;
protected:
 static void clearUp(void* pVoid);

};

}
}