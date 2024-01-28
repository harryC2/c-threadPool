#include "workthread.h"
#include "threadPool.h"
#include <string.h>
#include "../utility/logger.h"
#include <iostream>
#include "pthread.h"
using namespace web_rpc::thread_poll;
using namespace web_rpc::utility;


WorkerThread::WorkerThread()
{

}

WorkerThread::~WorkerThread()
{

}

void WorkerThread::clearUp(void* pVoid)
{
    // log执行清理函数
    info("clearup %d",static_cast<Thread*>(pVoid));
    std::cout<<"clearup"<<std::endl;
}

void WorkerThread::run()
{
    // 在程序执行过程中，操作系统或者其他进程可以向当前进程发送各种类型的信号
    // sigint 终端型号、 sigterm终止信号、 sigsegv中断信号、 多线程程序，每个线程都有
    // 各自的屏蔽信号集，每个线程都设置一个屏蔽集
    // sigset_t mask;
    // if(0 != sigfillset(&mask))
    // {
    //     // error log worker thread sigfillset faile!
    // }
    // if(0 != pthread_sigmask(SIG_SETMASK,&mask,NULL))
    // {
    //     // error log worker thread pthread_sigmask faile!
    // }
    // unsigned long mask;
    // memset(&mask, 0xFF, sizeof(unsigned long));
    // if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL))
    // {
    //     error("thread manager pthread_sigmask failed!");
    //     return;
    // }

    pthread_cleanup_push(clearUp, this);
    // 线程运行函数
    while (true)
    {
     // 这里要等待任务条件变量阻塞
      m_mutex.lock();
      while(m_task == nullptr)
      {
        m_con.wait(&m_mutex);
      }
      m_mutex.unLock();
     int rc = 0;
     int old_state = 0;
     rc = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_state);// 设置不可取消线程
     m_task->run();
     m_task->destory();
     m_task = nullptr;
    ThreadPool::GetInstance().moveToIdelList(this);
    rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state);// 设置取消线程
    pthread_testcancel(); // cancel-point 是否被请求取消 执行清理动作
    }
    pthread_cleanup_pop(1);
}