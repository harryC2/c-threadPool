#include "task.h"

using namespace web_rpc::thread_poll;

void Task::set_data(void * data)
{
    AutoMutex lock(&m_mutex_data);
    m_data = data;
}
void* Task::get_data()
{
    AutoMutex lock(&m_mutex_data);
    return m_data;
}