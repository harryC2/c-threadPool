#include "socket/clientSocket.h"
using namespace web_rpc::socket;

#include <iostream>
using namespace std;
#include "thread/task.h"
#include "thread/taskdispatcher.h"
using namespace web_rpc::thread_poll;
#include<chrono>
#include<thread>
#include "utility/logger.h"
using namespace web_rpc::utility;
#include"serialize/DataStream.h"
using namespace web_rpc::serialize;

class Request : public Serializable
{
public:
    Request() {}
    Request(const string & name) : m_name(name) {}
    ~Request() {}

    const string & name() const
    {
        return m_name;
    }

    SERIALIZE(m_name)

private:
    string m_name;
};


class Response : public Serializable
{
public:
    Response() {}
    Response(const string & name) : m_name(name) {}
    ~Response() {}

    const string & name() const
    {
        return m_name;
    }

    SERIALIZE(m_name)

private:
    string m_name;
};


void testClient()
{
    Client client;
    if( client.connect("192.168.200.100", 8080))
    {
        auto total = client.call<int>("sum", 1, 2);
            //std::cout << total << std::endl;
    }
}

class TaskClient :  public Task
{
private:
    /* data */
public:
    TaskClient(/* args */){};
    ~TaskClient(){};

    void run() override{
        testClient();
    };
    virtual void destory() override
    {
        delete this;
    };
};
#include <chrono>

int main()
{
    Logger::instance()->initLogger();
    TaskDispatcher::getInstance().initTaskDispatcher(50);
    Client::initSocket();
    // ms时间
    auto start = std::chrono::system_clock::now();
    auto duration = start.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    int b = 10000;
    TaskDispatcher::getInstance().setTaskNumber(b);
    TaskDispatcher::getInstance().setTaskStartTime(millis);
    for (int i = 0; i < b; i++)
    {
        TaskClient *pTask = new TaskClient();
        TaskDispatcher::getInstance().assign(pTask);
    }
    system("pause");
    return 1;
}