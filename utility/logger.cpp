#include "logger.h"
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <stdexcept>
#include "threadLog.h"
using namespace web_rpc::utility;

void Logger::log(Level level, const char* file, int line, const char* format, ...)
{
    if (m_level > level)
    {
        return;
    }
    // 主线程当前日志执行时间
    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);

    //写入时间
    int timeLen = 0;
    const char * fmt = "%s %s %s:%d ";
    timeLen = snprintf(NULL, 0, fmt, timestamp, s_level[level], file, line);
    if (timeLen <= 0)
    {
        return;
    }

    char timeLog[timeLen+1] ={0};
    //char *buffer = new char[timeLen + 1];
    snprintf(timeLog, timeLen+1, fmt, timestamp, s_level[level], file, line);
    // buffer[len] = 0;
    // m_fout << buffer;
    // delete buffer;
    // m_len += len;

    va_list arg_ptr;
    va_start(arg_ptr, format);
    int contentLen = vsnprintf(NULL, 0, format, arg_ptr);
    va_end(arg_ptr);
    if (contentLen <= 0)
    {
        return;
    }
    char content[contentLen+1] = {0};
    va_start(arg_ptr, format);
    vsnprintf(content, contentLen+1, format, arg_ptr);
    va_end(arg_ptr);
    StLogMessage* pstLogMessage = new StLogMessage(timeLen + contentLen + 2);
    memset(pstLogMessage->m_pStrLogMes,0,timeLen + contentLen + 2);
    memcpy(pstLogMessage->m_pStrLogMes,timeLog,timeLen);
    pstLogMessage->m_pStrLogMes[timeLen-1] = ' ';
    memcpy(&pstLogMessage->m_pStrLogMes[timeLen],content,contentLen);
    pstLogMessage->m_pStrLogMes[timeLen + contentLen ] = '\n';
    loggerThread->postLogMes(pstLogMessage);
}
const char* Logger::s_level[LEVEL_COUNT] = 
{
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

void Logger::initLogger(){
    // 设置日志级别
    level(Level::DEBUG);
    loggerThread = new ThreadLogger();
    loggerThread->start();
};
LoggerHandle::LoggerHandle() : m_max(0), m_len(0)
{
}

LoggerHandle::~LoggerHandle()
{
    close();
}

void LoggerHandle::open(const string &filename)
{
    m_filename = filename;
    //打开文件以追加的方式
    m_fout.open(filename.c_str(), ios::app);
    if (m_fout.fail())
    {
        throw std::logic_error("open log file failed: " + filename);
    }
    //把文件的写指针移到文件结尾并往后移动0个单位
    m_fout.seekp(0, ios::end);
    /*tellg() 和tellp()
      这两个成员函数不用传入参数，返回pos_type 类型的值(根据ANSI-C++ 标准) ，
      就是一个整数，代表当前get 流 指针的位置(用tellg) 或put 流指针的位置(用
      tellp).而且不要对tellg 或tellp 的返回值进行修改。
    */
    m_len = m_fout.tellp();
}

void LoggerHandle::close()
{
    m_fout.close();
}

void LoggerHandle::writeToFile(StLogMessage* pLogMessage)
{
    if (m_fout.fail())
    {
        throw std::logic_error("open log file failed: " + m_filename);
    }
    m_fout << pLogMessage->m_pStrLogMes;
    m_len += pLogMessage->m_nlen;
    /*
    其作用是将输出流中的数据立即刷新到文件或
    者设备中，以确保数据已经被写入。需要注意
    的是，如果不调用 flush() 方法，数据可能会被缓存并延迟写入，
    从而导致程序在错误的时间点出现异常或者输出数据不完整的情况。
    因此，在需要确保数据已经被写入时，
    应该使用 flush() 方法来刷新输出流。*/
    m_fout.flush();
    
    if (m_max > 0 && m_len >= m_max)
    {
        rotate();
    }
}

void LoggerHandle::max(int bytes)
{
    m_max = bytes;
}

void LoggerHandle::rotate()
{
    close();
    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H-%M-%S", ptm);
    string filename = m_filename + timestamp;
    if (rename(m_filename.c_str(), filename.c_str()) != 0)
    {
        throw std::logic_error("rename log file failed: " + string(strerror(errno)));
    }
    open(m_filename);
}


