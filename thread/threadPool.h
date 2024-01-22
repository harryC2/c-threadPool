/*
    此文件封装了threadPool类，用来管理线程池：busy_thread与idel_thread
    对外接口有 assign委派任务，操作busy_thread\idel_thread 线程安全
*/
#pragma once
#include <set>
#include "workthread.h"
#include "mutex.h"
#include "conditionVariable.h"

namespace web_rpc{
namespace thread_poll{

class ThreadPool
{
private:
    ThreadPool(/* args P*/);
    ThreadPool(ThreadPool&& other) = delete;
    ~ThreadPool();
public:
   void stop(){
        // busyList pushback
    mBusyMutex.lock();
    for (auto& item : vec_busy_threadPool)
    {
      item->stop();
    }
    mBusyMutex.unLock();

    mIdleMutex.lock();
    for (auto& item : vec_idle_threadPool)
    {
      item->stop();
    }
    mIdleMutex.unLock();

   }
  static ThreadPool& GetInstance()
  {
     static ThreadPool stThreadPool;
     return stThreadPool;
  }
public:
    int getIdelListSize();
    void init(int nCount);
    Thread* getIdelThread();
    void assign(Task* task);
    void moveToBusyList(Thread* pThread);
    // 效率问题，工作线程 完成工作后，立即放入到空闲队列中
    void moveToIdelList(Thread* pThread);

private:
// 多线程调用  经常头部删除与添加 用set或者list集合
// 在任务分发线程（获取数量，委派任务）；工作线程中将空闲线程移动到繁忙线程中
  std::set<Thread*> vec_idle_threadPool; 
  std::set<Thread*> vec_busy_threadPool; 
  int m_nCount{0};
  Mutex mIdleMutex;
  Mutex mBusyMutex;
  ConditionVariable m_idleCon;
  ConditionVariable m_busyCon;
};

}
}