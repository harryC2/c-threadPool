#pragma once

#include "../thread/task.h"
using namespace web_rpc::thread_poll;

#include "../socket/socket.h"
using namespace web_rpc::socket;

namespace web_rpc{
namespace task{

struct MsgHead{
    char flag[8];
    uint32_t cmd;
    uint32_t len;
};
const uint32_t recv_buff_size = 1024;
class WorkTask : public Task
{
public:
  WorkTask(Socket* socket);
  virtual ~WorkTask();
  void run() override;
  void destory() override;
};

}
}