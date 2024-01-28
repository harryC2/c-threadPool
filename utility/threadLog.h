/*
   此文件封装了一个线程执行日记缓冲区 i\o读写操作； 主线程记录运行时间及日志将字符串抛给
   此日志线程，日志线程进行读写缓冲区并实施刷新日志文件
*/
#pragma once
#include "logger.h"
#include "../thread/thread.h"
#include <string>
#include "pthread.h"
#include <list>


namespace web_rpc{
namespace utility{
using namespace web_rpc::thread_poll;

class ThreadLogger : public Thread
{
public:
    ThreadLogger(/* args */);
    virtual ~ThreadLogger();
protected:
 static void clearUp(void* pVoid);
public:   
    // 线程中执行的代码
    void init()
    {
      // 打开文件 创建输入输出流
      loggerHandle.open("./logTest.txt");
    }
    void postLogMes(StLogMessage* pLogMessage)
    {
         m_mutex.lock();
         m_listlog.push_back(pLogMessage);
         m_mutex.unLock();
         m_con.signal();
    }
    virtual void run() override
    {
        // unsigned long mask;
        // memset(&mask, 0xFF, sizeof(unsigned long));
        // if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL))
        // {
        //     error("thread manager pthread_sigmask failed!");
        //     return;
        // }

        while (true)
        {
            m_mutex.lock();
            while (m_listlog.empty())
            {
                m_con.wait(&m_mutex);
            }
            StLogMessage *pLogMessage = m_listlog.front();
            m_listlog.pop_front();
            m_mutex.unLock();
            loggerHandle.writeToFile(pLogMessage);
            delete pLogMessage;
            pLogMessage = nullptr;
        }
    };
private:
    std::list<StLogMessage*> m_listlog;
    LoggerHandle loggerHandle;
};

ThreadLogger::ThreadLogger(/* args */)
{
}

ThreadLogger::~ThreadLogger()
{
}
}
}