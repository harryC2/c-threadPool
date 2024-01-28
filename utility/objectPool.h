/*
   模板类，实现对std::list 线程安全的template<class T> 模板对象池的创建，读写操作
*/
#pragma once
#include<list>
#include "../thread/mutex.h"


using namespace web_rpc::thread_poll;
namespace web_rpc{
namespace utility{

template<class T>
class ObjectPool
{
public:
    ObjectPool(/* args */);
    ~ObjectPool();

    void init(int max);
    // 从池子中取出一个T 
    T* allocate();
    // 往池子中添加一个T
    void release(T* p);

private:
    std::list<T*> m_list;
    Mutex m_mutex;
};
template<class T>
T* ObjectPool<T>::allocate()
{
    AutoMutex lock(&m_mutex);
    if (m_list.size() == 0)
    {
        return nullptr;
    }
    T *p = m_list.front();
    m_list.pop_front();
    return p;
};
template<class T>
void ObjectPool<T>::release(T* p)
{
    AutoMutex lock(&m_mutex);
    m_list.push_back(p);
};
template<class T>
void ObjectPool<T>::init(int max)
{
    AutoMutex lock(&m_mutex);
    for (int i = 0; i < max; i++)
    {
        m_list.push_back(new T());
    };
};
template<class T>
ObjectPool<T>::ObjectPool(/* args */)
{
};
template<class T>
ObjectPool<T>::~ObjectPool()
{
};




}
}


