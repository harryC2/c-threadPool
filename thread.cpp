#include "thread.h"

using namespace web_rpc::thread_poll;


Thread::Thread()
{
}

Thread::~Thread()
{
}
void Thread::start(){
    // 创建线程
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);// 设置线程作用域为系统级别，
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//设置线程分离状态，以便运行结束后能够自动释放资源
    pthread_create(&m_id,&attr,threadFunc,(void*)this);
    pthread_attr_destroy(&attr);

}

void* Thread::threadFunc(void* ptr)
{
    Thread *thread = (Thread *)ptr;
    thread->run();
    return ptr;
}
void Thread::stop()
{
   pthread_exit(PTHREAD_CANCELED);
}
void Thread::setTask(Task* task)
{    
    m_mutex.lock();
    m_task = task;
    m_con.signal();
    m_mutex.unLock();
};
Task* Thread::getTask()
{
    AutoMutex lock(&m_mutex);
    return m_task;
};