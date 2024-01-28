#include <iostream>
#include<chrono>
#include<thread>
#include <algorithm>
#include "webRpc/server.h"
using namespace web_rpc::webrpc;

#include<string>
using namespace std;

#include "utility/logger.h"
using namespace web_rpc::utility;

#include "serialize/serializable.h"
using namespace web_rpc::serialize;

string hello(const string & name)
{
    return "hello, " + name;
}

int sum(int a, int b)
{
    return a + b;
}

class Request :public Serializable
{
public:
  Request(){};
  ~Request(){};
  Request(const string& name): m_name(name){};
  
  const string & name() const
  {
    return m_name;
  }
  SERIALIZE(m_name);
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

Response upper(const Request & req)
{
    string name = req.name();
    //transform(name.begin(),name.end(),::toupper);
    return Response(name+"replay news");// 这里涉及到多次拷贝，需要std::move 拷贝构造来实现
}




int main()
{
    Server::getInstance().bind("hello", hello);
    Server::getInstance().bind("sum", sum);
    Server::getInstance().bind("upper", upper);
    Server::getInstance().start();

   std::string input;
    {
     int a =0;
     std::this_thread::sleep_for(std::chrono::microseconds(1));
     info("主线程退出");
    }
    return 1;
}