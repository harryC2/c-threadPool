#include "dataStream.h"

using namespace web_rpc::serialize;

void DataStream::show()
{
    int size = m_buf.size();
    std::cout << "data size = " << size << std::endl;
    int i = 0;
    while (i < size)
    {
        switch ((DataType)(m_buf[i]))
        {
        case DataType::BOOL:
            if ((int)m_buf[++i] == 0)
            {
                std::cout << "false";
            }
            else
            {
                std::cout << "true";
            }

            break;
        case DataType::CHAR:
        {
            char a;
            std::memcpy(&a, &m_buf[++i], sizeof(char));
            std::cout << a;
            i += sizeof(char);
        }
        break;
        case DataType::INT32:
        {
            int32_t a;
            std::memcpy(&a, &m_buf[++i], sizeof(int32_t));
            std::cout << a;
            i += sizeof(int32_t);
        }
        break;
        case DataType::INT64:
        {
            int64_t a;
            std::memcpy(&a, &m_buf[++i], sizeof(int64_t));
            std::cout << a;
            i += sizeof(int64_t);
        }
        break;
        case DataType::FLOAT:
        {
            float a;
            std::memcpy(&a, &m_buf[++i], sizeof(float));
            std::cout << a;
            i += sizeof(float);
        }
        break;
        case DataType::DOUBLE:
        {
            double a;
            std::memcpy(&a, &m_buf[++i], sizeof(double));
            std::cout << a;
            i += sizeof(double);
        }
        break;
        case DataType::STRING:
        {
            if ((DataType)m_buf[++i] == DataType::INT32)
            {
                int len = *((int *)&m_buf[++i]);
                i += 4;
                std::cout << std::string(&m_buf[i], len);
                i += len;
            }
            // int32_t len;
            // std::memcpy(&len, &m_buf[++i], sizeof(int32_t));
            // std::cout << "string len = " << len;
            // i += sizeof(int32_t);
            // char *pChar = new char[len + 1];
            // std::memset(pChar, 0, len + 1);
            // std::memcpy(pChar, &m_buf[++i], len);
            // std::cout << " string  data = " << pChar;
            // i += len;
        }
        break;
        default:
            break;
        }

        std::cout<<std::endl;
    }
}

// 读 将字节流数据读出来
bool DataStream::read(char * data, int len)
{
    std::memcpy(data,(char*)&m_buf[m_len],len);
    m_len+=len;
    return true;
}
void DataStream::read(bool &b)
{
    if ((DataType)m_buf[m_len++] == DataType::BOOL)
    {
        b = *((bool *)&m_buf[m_len++]);
    }
    else
    {
        return;
    }
    m_buf.data();
};
void DataStream::read(char &value)
{
    if ((DataType)m_buf[m_len++] == DataType::CHAR)
    {
        value = ((char)m_buf[m_len++]);
    }
    else
    {
        return;
    }
}
void DataStream::read(int32_t &value)
{
    if ((DataType)m_buf[m_len++] == DataType::INT32)
    {
        value = *((int32_t *)&m_buf[m_len]);
        m_len += 4;
    }
    else
    {
        return;
    }
}
void DataStream::read(int64_t &value)
{
    if ((DataType)m_buf[m_len++] == DataType::INT64)
    {
        value = *((int64_t *)&m_buf[m_len]);
        m_len += sizeof(int64_t);
    }
    else
    {
        return;
    }
}
void DataStream::read(float &value)
{
    if ((DataType)m_buf[m_len++] == DataType::FLOAT)
    {
        value = *((int64_t *)&m_buf[m_len]);
        m_len += sizeof(int64_t);
    }
    else
    {
        return;
    }
}
void DataStream::read(double &value)
{
    if ((DataType)m_buf[m_len++] == DataType::DOUBLE)
    {
        value = *((double *)&m_buf[m_len]);
        m_len += sizeof(double);
    }
    else
    {
        return;
    }
}
void DataStream::read(char *value)
{
    if ((DataType)m_buf[m_len++] == DataType::STRING)
    {
        int len{0};
        read(len);
        value = new char[len + 1];
        std::memcpy(value, &m_buf[m_len], len);
        m_len += len;
    }
    else
    {
        return;
    }
}
void DataStream::read(std::string &value)
{
    if ((DataType)m_buf[m_len++] == DataType::STRING)
    {
        int len{0};
        read(len);
        value.assign(&m_buf[m_len], len);
        m_len += len;
    }
    else
    {
        return;
    }
}
void DataStream::write(bool value)
{
    char type = DataType::BOOL;
    write((char *)&type, sizeof(char));
    write((char *)&value, sizeof(char));
}
void DataStream::write(char value)
{
    char type = DataType::CHAR;
    write((char *)&type, sizeof(char));
    write((char *)&value, sizeof(char));
}

void DataStream::write(int32_t value)
{
    char type = DataType::INT32;
    write((char *)&type, sizeof(char));
    write((char *)&value, sizeof(int32_t));
}
void DataStream::write(int64_t value)
{
    char type = DataType::INT64;
    write((char *)&type, sizeof(char));
    write((char *)&value, sizeof(int64_t));
}

void DataStream::write(float value)
{
    char type = DataType::FLOAT;
    write((char *)&type, sizeof(char));
    write((char *)&value, sizeof(float));
}
void DataStream::write(double value)
{
    char type = DataType::STRING;
    write((char *)&type, sizeof(char));
    write((char *)&value, sizeof(float));
}
void DataStream::write(const std::string &value)
{
    char type = DataType::STRING;
    write((char *)&type, sizeof(char));
    int32_t len = value.size();
    write(len);
    write(value.data(), len);
}
void DataStream::write(const char *value)
{
    char type = DataType::STRING;
    write((char *)&type, sizeof(char));
    int len = strlen(value);
    write(len); // T  L  V 编码；  type, 长度 ，value
    write(value, len);
}

void DataStream::DataStreamReserve(int len)
{
    int size = m_buf.size();
    int cap = m_buf.capacity();
    if (size + len > cap)
    {
        while (size + len > cap)
        {
            if (cap == 0)
            {
                cap = 1;
            }
            else
            {
                cap *= 2;
            }
        }
        m_buf.reserve(cap);
    }
}

int DataStream::size() const
{
    return m_buf.size();
}
const char * DataStream::data() const
{
    return m_buf.data();
}


void DataStream::write(const char *data, int len)
{
    // 扩容 如果vector 容量不够的话先扩容再写至，可以防止底层多次内存移动
    DataStreamReserve(len);
    int size = m_buf.size();
    m_buf.resize(size + len);
    std::memcpy(&m_buf[size], data, len); // 内存拷贝，可以提高移动效率
}