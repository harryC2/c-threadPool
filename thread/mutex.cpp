#include"mutex.h"

using namespace web_rpc::thread_poll;


Mutex::Mutex(/* args */)
{
    // 以下三种错误情况会自动检测，使使用时更加安全；
    // 设置一个错误检测属性，当同一个线程 锁已经锁上的锁，
    // 一个线程去释放已经释放的锁会发生错误
    // 一个线程去释放另一个线程锁定的锁

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&m_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    pthread_mutex_init(&m_mutex,nullptr);
}

Mutex::~Mutex()
{
      pthread_mutex_destroy(&m_mutex);
}
int  Mutex::lock()
{
 return pthread_mutex_lock(&m_mutex);
}
int Mutex::unLock()
{
 return pthread_mutex_unlock(&m_mutex);
}

int Mutex::try_lock()
{
    return pthread_mutex_trylock(&m_mutex);
}