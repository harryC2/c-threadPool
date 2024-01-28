/*
  本文件创建目的是,利用 实现一个互斥锁、
  及自动解锁、包含Mutex\autoMutex两个类
*/
#pragma once
#include "pthread.h"

namespace web_rpc{
namespace thread_poll{

class Mutex
{
  friend class ConditionVariable;
public:
    Mutex(/* args */);
    ~Mutex();
    int lock();
    int try_lock();
    int unLock();

private:
pthread_mutex_t m_mutex;
};

class AutoMutex{
public:
  explicit AutoMutex(Mutex* mutex):m_mutex(mutex)
  {
    m_mutex->lock();
  };
  ~AutoMutex()
  {
    m_mutex->unLock();
  };
private:
Mutex* m_mutex{nullptr};
};


}
}