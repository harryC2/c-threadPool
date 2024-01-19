/*
    此文件封装了基础线程的创建，虚接口run;
*/
#pragma once
#include<pthread.h>
#include "task.h"
#include "conditionVariable.h"
#include "mutex.h"

namespace web_rpc{
namespace thread_poll{

class Thread
{
public:
   Thread();
   virtual ~Thread();
public:
  void start();
  void stop();
  virtual void run() = 0;

  void setTask(Task* task);
  Task* getTask();
protected:
  static void* threadFunc(void* arg);
protected:
  pthread_t m_id{NULL};
  Task* m_task{nullptr};
  ConditionVariable m_con;
  Mutex m_mutex;
};

}
}
