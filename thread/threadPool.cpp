#include "threadPool.h"
#include "../utility/logger.h"
using namespace web_rpc::thread_poll;
using namespace web_rpc::utility;

ThreadPool::ThreadPool(/* argsP*/)
{
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::init(int nCount)
{
    m_nCount = nCount;
    AutoMutex lock(&mIdleMutex);
    for (int i = 0; i < nCount; i++)
    {
        Thread *pThread = new WorkerThread();
        // info log 记录 当前创建的线程地址，到时候退出的时候clearUp记录退出线程地址是否一致
        info("ThreadPool init  thread number %d,addr = %d ",i,pThread);
        pThread->start();
        vec_idle_threadPool.insert(pThread);
    }
};
int ThreadPool::getIdelListSize()
{
    AutoMutex lock(&mIdleMutex);
    return vec_idle_threadPool.size();
}
Thread* ThreadPool::getIdelThread()
{
    // 这里阻塞获取idelThread 空闲线程
    AutoMutex lock(&mIdleMutex);
    while (vec_idle_threadPool.size() == 0)
    {
        m_idleCon.wait(&mIdleMutex);
    };
    return *vec_idle_threadPool.begin();
};
void ThreadPool::assign(Task *task)
{
    if (task == nullptr)
    {
        // debug log : assign a null task to threadpool
        return;
    }
    Thread *pThread = getIdelThread();
    if (pThread != nullptr)
    {
        moveToBusyList(pThread);
        pThread->setTask(task);
    }
    else
    {
            // error log  threadPool is busy
    }
}
void ThreadPool::moveToBusyList(Thread *pThread)
{
    // idel list pop
    if (pThread == nullptr)
    {
        return;
    }

    // busyList pushback
    mBusyMutex.lock();
    while (m_nCount == vec_busy_threadPool.size())
    {
        // 说明有重复的需要过滤掉
        m_busyCon.wait(&mBusyMutex);
    }
    vec_busy_threadPool.insert(pThread);
    mBusyMutex.unLock();

    mIdleMutex.lock();
    auto iter = vec_idle_threadPool.find(pThread);
    if (iter != vec_idle_threadPool.end())
    {
        vec_idle_threadPool.erase(iter);
    }
    mIdleMutex.unLock();
}
// 效率问题，工作线程 完成工作后，立即放入到空闲队列中
void ThreadPool::moveToIdelList(Thread *pThread)
{
    // idel list pop
    if (pThread == nullptr)
    {
        return;
    }
    // busyList pushback
    mIdleMutex.lock();
    vec_idle_threadPool.insert(pThread);
    m_idleCon.signal();
    mIdleMutex.unLock();

    mBusyMutex.lock();
    auto iter = vec_busy_threadPool.find(pThread);
    if (iter != vec_busy_threadPool.end())
    {
        vec_busy_threadPool.erase(iter);
    }
    m_busyCon.signal();
    mBusyMutex.unLock();
}