#pragma once

#include <string>
#include <fstream>
using namespace std;

namespace web_rpc {
namespace utility {

    enum Level
    {
        DEBUG = 0,
        INFO,
        WARN,
        ERRORE,
        FATAL,
        LEVEL_COUNT
    };

#define debug(format, ...) \
    Logger::instance()->log(Level::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define info(format, ...) \
    Logger::instance()->log(Level::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define warn(format, ...) \
    Logger::instance()->log(Level::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define error(format, ...) \
    Logger::instance()->log(Level::ERRORE, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define fatal(format, ...) \
    Logger::instance()->log(Level::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

/*
Logger  提供给主线程生产 任务日志
*/
struct StLogMessage
{
    StLogMessage(int nLen){
        m_pStrLogMes = new char[nLen];
        m_nlen = nLen;
    }
    ~StLogMessage()
    {
        if(m_pStrLogMes != nullptr)
        {
            delete[] m_pStrLogMes;
            m_pStrLogMes = nullptr;
        }
    }
    char* m_pStrLogMes{nullptr};
    int m_nlen{0};
};

class ThreadLogger;
class Logger
{
public:
  static Logger* instance()
  {
    static Logger logger;
    return &logger;
  };
  void level(int level){
    m_level = level;
  };
  
  void initLogger();
private:
  Logger(){};
  ~Logger(){};
  Logger(Logger&& other) = delete;
  Logger& operator = (Logger&& other) = delete;
public:
  // 生成logMessage
  void log(Level level, const char* file, int line, const char* format, ...);
private:
  // 日志线程
  ThreadLogger* loggerThread = nullptr; 
  int m_level{Level::DEBUG};
  static const char* s_level[LEVEL_COUNT];
};


class LoggerHandle
{
public:
    LoggerHandle();
    ~LoggerHandle();
    void open(const string &filename);
    void close();
    void max(int bytes);
    void rotate();
    void writeToFile(StLogMessage* pLogMessage);
private:
    string m_filename;
    ofstream m_fout;
    int m_max;
    int m_len;
};

}}
