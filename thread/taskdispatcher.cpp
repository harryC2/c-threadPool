#include "taskdispatcher.h"
#include "signal.h"
#include <signal.h>
#include <string.h>
#include "..\utility\logger.h"

using namespace web_rpc::thread_poll;
using namespace web_rpc::utility;
TaskDispatcher::TaskDispatcher(/* args */)
{
}

TaskDispatcher::~TaskDispatcher()
{
}

void TaskDispatcher::initTaskDispatcher()
{
    ThreadPool::GetInstance().init(8);
    debug("task dispatcher init %d threadPool number",8);
    start();
}
void TaskDispatcher::assign(Task *task)
{
     debug("task dispatcher assign task");
    // 主线程调用、与任务分发线程调用
    AutoMutex lock(&m_mutex);
    m_listTask.push_back(task);
    m_con.signal();

}

void TaskDispatcher::run()
{

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
        }
    }
};
