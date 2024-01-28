#include "../head.h"

#ifdef CLIENT
#include "client.h"
using namespace web_rpc::webrpc;

#include "../utility/logger.h"
using namespace web_rpc::utility;


Client::Client()
{
   // System * sys = Singleton<System>::instance();
    //sys->init();

    // init logger
   // Logger::instance()->open(sys->get_root_path() + "/log/client.log");
}

Client::~Client()
{
    m_socket.close();
}

bool Client::connect(const string & ip, int port)
{
    m_ip = ip;
    m_port = port;
    return m_socket.connect(ip, port);
}
#endif 