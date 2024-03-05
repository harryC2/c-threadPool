#include "taskdispatcher.h"
#include "..\utility\logger.h"
#include "signal.h"
#include <signal.h>
#include <string.h>
#include<iostream>
#include<chrono>
using namespace web_rpc::thread_poll;
using namespace web_rpc::utility;
TaskDispatcher::TaskDispatcher(/* args */)
{
}

TaskDispatcher::~TaskDispatcher()
{
}

void TaskDispatcher::initTaskDispatcher(int num)
{
    ThreadPool::GetInstance().init(num);
    debug("task dispatcher init %d threadPool number",num);
    start();
}
void TaskDispatcher::assign(Task *task)
{
     debug("task dispatcher assign task");
    // 主线程调用、与任务分发线程调用
    m_mutex.lock();
    m_listTask.push_back(task);
    m_mutex.unLock();
    m_con.signal();
}
void TaskDispatcher::setTaskNumber(int taskNumber)
{
    m_taskNumber =taskNumber;
}
void TaskDispatcher::setTaskStartTime(int64_t startTime)
{
    m_startTime =startTime;
}
int64_t TaskDispatcher::getEndTaskTime()
{
    return m_endTime;
}
void TaskDispatcher::run()
{
    static int taskNumber = 0;
    // 线程派发任务呗
    // 这里有一个任务队列吧
    unsigned long  mask;
    memset(&mask, 0xFF, sizeof(unsigned long));
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL))
    {
        error("thread manager pthread_sigmask failed!");
        return;
    }

    while (true)
    {
        m_mutex.lock();
        while (m_listTask.empty())
        {
            m_con.wait(&m_mutex);
        }
        Task *pTask = m_listTask.front();
        m_listTask.pop_front();
        m_mutex.unLock();
        // 直接派任务，如果当前线程池任务繁忙，不存在idel线程
        //存在阻塞
        // 如何解决 ，如果繁忙就放回task队列中
        if(ThreadPool::GetInstance().getIdelListSize() == 0)
        {
            m_mutex.lock();
            m_listTask.push_front(pTask);
            m_mutex.unLock();
        }
        else
        {
           ThreadPool::GetInstance().assign(pTask);
           taskNumber++;
        }
        if (taskNumber == m_taskNumber)
        {
            auto start = std::chrono::system_clock::now();
            auto duration = start.time_since_epoch();
            m_endTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            float c = (m_endTime-m_startTime)/static_cast<float>(m_taskNumber);
             std::cout<<"起始时间："<<m_startTime<<"截止时间"<<"任务数量:"<<m_taskNumber<<"总耗时："<<m_endTime-m_startTime<<"平均时间"<<(c)<<std::endl;
        }
        
    }
};
