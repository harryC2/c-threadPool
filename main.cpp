#include <iostream>
#include "task.h"
#include "taskdispatcher.h"
#include<chrono>
#include<thread>

using namespace std;
using namespace web_rpc::thread_poll;

class TaskPrintf :  public Task
{
private:
    /* data */
public:
    TaskPrintf(/* args */){};
    ~TaskPrintf(){};

    void run() override{
        std::cout<< "run task"<<endl;
        std::cout<< *static_cast<int*>(m_data)<<std::endl;
    };
    virtual void destory() override
    {
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
    TaskDispatcher::getInstance().init();

    for(int i = 0 ; i < 12 ; i ++)
    {
        TaskPrintf* pTask =  new TaskPrintf();
        pTask->set_data(new int(i));
        TaskDispatcher::getInstance().assign(pTask);
    }
    std::string input;
    while (true)
    {
        int a =0;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      //  std::cout<<"a"<<std::endl;
    }
    return 1;
}