/*
    该文件实现DataType 类，有         enum DataType
         {
            BOOL = 0,
            CHAR,
            INT32,
            INT64,
            FLOAT,
            DOUBLE,
            STRING,
            VECTOR,
            LIST,
            MAP,
            SET,
            CUSTOM
         };
         类型实现序列化与反系列化；
         序列化成字节流保存在 std::vector<char*> m_buf缓冲区中；且适当运用reseve() resize()扩容;
         调用write()序列化,read()反序列化；
*/
#pragma once
#include <vector>
#include<string>
#include<cstring>
#include<cstdint>
#include <iostream>
#include<typeinfo>
#include<fstream>
#include <list>
#include <map>
#include <set>

#include"serializable.h"
#include"stdarg.h"
static const char * constPath = "./data1.a";
const std::string pathStr = "./data.a";
namespace web_rpc{
namespace serialize {

    class DataStream
    {
        public:
         enum DataType
         {
            BOOL = 0,
            CHAR,
            INT32,
            INT64,
            FLOAT,
            DOUBLE,
            STRING,
            VECTOR,
            LIST,
            MAP,
            SET,
            CUSTOM
         };
    DataStream(){};
    ~DataStream(){};
    // operator <<
    template<class T>
    bool isRighteType(T type)
    {
       bool a =  typeid(int) == typeid(T);
       a = std::is_same<T,std::string>::value;
       //int* pTem = static_cast<int*>(type);
       // 这里可以判断那些类型可以序列化
       // 加constexr 可以编译器运行，而在提高代码运行期效率
    //    if(pTem != nullptr)
    //    {
    //     a = true;
    //     return a;  
    //    }
       a = std::is_same<T,int32_t>::value;
       return a;

    }
    template<class T>
     DataStream& operator <<(T&& t)
     {
       // isRighteType(std::forward<T>(t));
        write(std::forward<T>(t));
        return *this;
     };
     void writeFile(std::string& strPath)
     {
        
        if(strPath.empty())
        {
            strPath = constPath;    
        }
        std::ofstream filestream(strPath,std::ios::out|std::ios::binary);
        if(filestream.is_open())
        {
            filestream.write(reinterpret_cast<const char*>(m_buf.data()),m_buf.size());
            filestream.close();
        }
     }
    void readData(std::string& strPath)
    {
         if(strPath.empty())
        {
            strPath = constPath;    
        }
        std::ifstream filestream(strPath,std::ios::in|std::ios::binary);
        if(filestream.is_open())
        {
            filestream.seekg(0,std::ios::end);
            std::streampos fileSize = filestream.tellg();
            m_buf.clear();
            m_buf.resize(fileSize);
            filestream.seekg(0,std::ios::beg);
            filestream.read(m_buf.data(),fileSize);
            filestream.close();
        }
    }
    void show();
    void write(const char* data,int len);

    void write(bool b);
    void write(char value);
    void write(int32_t value);
    void write(int64_t value);
    void write(float value);
    void write(double value);
    void write(const char* value); 
    void write(const std::string& value); 
    template<class T>
    void write(const std::vector<T>& value);
    template <typename T>
    void write(const std::list<T> & value);
    template <typename K, typename V>
    void write(const std::map<K, V> & value);
    template <typename T>
    void write(const std::set<T> & value);
    template <typename T, typename ...Args>
    void write_args(const T & head, const Args&... args)
    {
         write(head);
         write_args(args...);// 循环将参数读出，write(head)//写第一个参数
         
    }
    void write(const Serializable& serializable )
    {
        serializable.serialize(*this);
    }
    void write_args(){};

    int size() const;
    const char * data() const;
    
    //读 将字节流数据读出来
    bool read(char * data, int len);
    void read(bool& b);
    void read(char& value);
    void read(int32_t& value);
    void read(int64_t& value);
    void read(float& value);
    void read(double& value);
    void read(char* value);
    void read(std::string& value);
    template <typename T>
    bool read(std::vector<T> & value);
    template <typename T>
    bool read(std::list<T> & value);
    template <typename K, typename V>
    bool read(std::map<K, V> &value);
    template <typename T>
    bool read(std::set<T> & value);
    template <typename T, typename ...Args>
    bool read_args(T & head, Args&... args);
    bool read_args(){return false;};
    bool read(Serializable& serializable )
    {
        return serializable.unserialize(*this);
    }

    template<class T>
     DataStream& operator >>(T&& t)
     {
        isRighteType(std::forward<T>(t));
        read(std::forward<T>(t));
        return *this;
     };
    private:
    void DataStreamReserve(int len);
      unsigned int m_len{0};
      std::vector<char> m_buf;
    };

    
    template<class T>
    void DataStream::write(const std::vector<T>& value)
    {
        char type = DataType::VECTOR;
        write(&type,sizeof(char));
        int len  = value.size();
        write(len);
        for(const auto& item : value)
        {
            write(item);
        }
    }
    template <typename T>
void DataStream::write(const std::list<T> & value)
{
    char type = DataType::LIST;
    write((char *)&type, sizeof(char));
    int len = value.size();
    write(len);
    for (auto it = value.begin(); it != value.end(); it++)
    {
        write((*it));
    }
}

template <typename K, typename V>
void DataStream::write(const std::map<K, V> & value)
{
    char type = DataType::MAP;
    write((char *)&type, sizeof(char));
    int len = value.size();
    write(len);
    for (auto it = value.begin(); it != value.end(); it++)
    {
        write(it->first);
        write(it->second);
    }
}

template <typename T>
void DataStream::write(const std::set<T> & value)
{
    char type = DataType::SET;
    write((char *)&type, sizeof(char));
    int len = value.size();
    write(len);
    for (auto it = value.begin(); it != value.end(); it++)
    {
        write(*it);
    }
}

template <typename K,typename V>
    bool DataStream::read(std::map<K, V> &valueMap)
    {
        valueMap.clear();
        if ((DataType)m_buf[m_len++] != DataType::MAP)
        {
            return false;
        }
        // 读取大小
        int len = -1;
        read(len);
       // value.reserve(len);
        for (int i = 0; i < len; i++)
        {
            K key;
            V value;
            read(key);
            read(value);
            valueMap[key] = value;
        }

        return true;
    }
template <typename T>
bool DataStream::read(std::vector<T> & value)
{
    value.clear();
    if (m_buf[m_len] != DataType::VECTOR)
    {
        return false;
    }
    ++m_len;
    int len;
    read(len);
    for (int i = 0; i < len; i++)
    {
        T v;
        read(v);
        value.push_back(v);
    }
    return true;
}

template <typename T>
bool DataStream::read(std::list<T> & value)
{
    value.clear();
    if (m_buf[m_len] != DataType::LIST)
    {
        return false;
    }
    ++m_len;
    int len;
    read(len);
    for (int i = 0; i < len; i++)
    {
        T v;
        read(v);
        value.push_back(v);
    }
    return true;
}
template <typename T>
bool DataStream::read(std::set<T> & value)
{
    value.clear();
    if (m_buf[m_len] != DataType::SET)
    {
        return false;
    }
    ++m_len;
    int len;
    read(len);
    for (int i = 0; i < len; i++)
    {
        T v;
        read(v);
        value.insert(v);
    }
    return true;
}
template <typename T, typename ...Args>
bool DataStream::read_args(T & head, Args&... args)
{
    read(head);
    return read_args(args...);
}
    }
}