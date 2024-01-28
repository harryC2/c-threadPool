/*
  该文件实现一个需要序列化的类，继承Serializable，将定义的 SERIALIZE(成员变量)，实现该类某些成员变量的序列化与反系列化
*/

#pragma once
namespace web_rpc {
namespace serialize {

class DataStream;

class Serializable
{
public:
    virtual void serialize(DataStream & stream) const = 0;
    virtual bool unserialize(DataStream & stream) = 0;
};

#define SERIALIZE(...)                              \
    void serialize(DataStream & stream) const       \
    {                                               \
        char type = DataStream::CUSTOM;             \
        stream.write((char *)&type, sizeof(char));  \
        stream.write_args(__VA_ARGS__);             \
    }                                               \
                                                   \
    bool unserialize(DataStream & stream)           \
    {                                               \
        char type;                                  \
        stream.read(&type, sizeof(char));           \
        if (type != DataStream::CUSTOM)             \
        {                                           \
            return false;                           \
        }                                           \
        stream.read_args(__VA_ARGS__);              \
        return true;                                \
    }

}
}