/*
    本文件是设计一个多线程任务基类Task；
    给线程池的任务需要继承此类；实现了任务数据存储(线程安全)，
    任务运行，运行玩之后释放资源的接口；
*/
#pragma once
#include "mutex.h"

namespace web_rpc{
namespace thread_poll{
class Task{
public:
  Task(){};
  ~Task(){};

  void set_data(void* data);
  void* get_data();
  virtual void run() = 0;
  virtual void destory() = 0;

protected:
   void* m_data{nullptr};
   Mutex m_mutex_data;
};


}
}