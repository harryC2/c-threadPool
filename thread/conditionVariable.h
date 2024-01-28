/*
    本文件封装了一个条件信号变量，用来实现线程同步机制
    允许线程在某个条件满足之前等待，并在条件满足时被通知
*/
#pragma once
#include<pthread.h>
#include"mutex.h"
namespace web_rpc{
namespace thread_poll{

class ConditionVariable
{
public:
  ConditionVariable();
  ~ConditionVariable();

public:
    int wait(Mutex* mtuex);
    int signal();
    int boradcast();

private:
  pthread_cond_t m_cond;
};

}
}