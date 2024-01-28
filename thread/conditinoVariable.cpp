#include "conditionVariable.h"

using namespace web_rpc::thread_poll;


ConditionVariable::ConditionVariable()
{
    pthread_cond_init(&m_cond,nullptr);
}

ConditionVariable::~ConditionVariable(){

    pthread_cond_destroy(&m_cond);
}

int ConditionVariable::wait(Mutex* mutex)
{
// 这里需要一个锁 来实现条件变量，当wait条件不满足时候，他将释放这个锁并阻塞等待
// 当条件满足时，他将获取这个锁，并向下执行
  
 return pthread_cond_wait(&m_cond,&mutex->m_mutex);

}

int ConditionVariable::signal()
{
    //唤醒一个等待的条件变量线程
 return pthread_cond_signal(&m_cond);
}

int ConditionVariable::boradcast()
{
    //唤醒所有等待的条件变量线程，性能更差
    return pthread_cond_broadcast(&m_cond);
}

