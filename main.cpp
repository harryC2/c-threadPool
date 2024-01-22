#include <iostream>
#include "thread/task.h"
#include "thread/taskdispatcher.h"
#include<chrono>
#include<thread>
#include "utility\logger.h"

using namespace std;
using namespace web_rpc::thread_poll;
using namespace web_rpc::utility;

class TaskPrintf :  public Task
{
private:
    /* data */
public:
    TaskPrintf(/* args */){};
    ~TaskPrintf(){};

    void run() override{
        info("run task %d",*static_cast<int*>(m_data));
    };
    virtual void destory() override
    {
        info("destory task %d",*static_cast<int*>(m_data));
        delete static_cast<int*>(m_data);
        m_data = nullptr;
        delete this;
    };
};

void printNum(int i)
{
    cout<<i<<endl;
}

int main()
{
    Logger::instance()->initLogger();
    TaskDispatcher::getInstance().initTaskDispatcher();

    for(int i = 0 ; i < 12 ; i ++)
    {
        TaskPrintf* pTask =  new TaskPrintf();
        pTask->set_data(new int(i));
        TaskDispatcher::getInstance().assign(pTask);
    }
    std::string input;
    {
        int a =0;
     std::this_thread::sleep_for(std::chrono::microseconds(1));
     info("主线程退出");
    }
    return 1;
}