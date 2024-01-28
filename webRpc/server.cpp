#include "server.h"
using namespace web_rpc::webrpc;

#include "../utility/logger.h"
using namespace web_rpc::utility;

#include "../thread/taskdispatcher.h"
using namespace web_rpc::thread_poll;

#include "../socket/socketHandler.h"
using namespace web_rpc::socket;

Server::Server()
{
    // System * sys = Singleton<System>::instance();
    // sys->init();

    // // init logger
    // Logger::instance()->open(sys->get_root_path() + "/log/server.log");

    // // init inifile
    // IniFile * ini = Singleton<IniFile>::instance();
    // ini->load(sys->get_root_path() + "/config/server.ini");

    // m_ip = (string)(*ini)["server"]["ip"];
    // m_port = (*ini)["server"]["port"];
    // m_threads = (*ini)["server"]["threads"];
    // m_connects = (*ini)["server"]["max_conn"];
    // m_wait_time = (*ini)["server"]["wait_time"];
}

Server::~Server()
{
}

void Server::listen(const string & ip, int port)
{
    m_ip = ip;
    m_port = port;
}

void Server::start()
{
    // init the thread pool and task queue
    Logger::instance()->initLogger();
    TaskDispatcher::getInstance().initTaskDispatcher();
    SocketHandler::getInstance().listen("127.0.0.1",8080);
    SocketHandler::getInstance().handle(8,0);

}

void Server::set_threads(int threads)
{
    m_threads = threads;
}

void Server::set_connects(int connects)
{
    m_connects = connects;
}

void Server::set_wait_time(int wait_time)
{
    m_wait_time = wait_time;
}

