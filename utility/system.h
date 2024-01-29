#pragma once

#include <string>
using namespace std;

namespace web_rpc {
namespace utility {

class System
{

public:
    static System& instance()
    {
        static System system;
        return system;
    }
    System();
    ~System();

    void init();
    string get_root_path();

private:
    void core_dump();

private:
    string m_root_path;
};

}}
