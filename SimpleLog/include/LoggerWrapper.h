#pragma once
#include "Logger.h"
#include <sstream>

using std::stringstream;

class CLoggerWrapper
{
public:
    CLoggerWrapper(const char* pszFileName, int nLine, CLogger::LogLevel level)
        : m_logger(pszFileName, nLine, level)
    {
        m_logger.LogStart();
    }
    ~CLoggerWrapper()
    {
        m_logger.SetLogContent(m_logStream.str().c_str());
        m_logger.LogFinish();
    }
    stringstream& LogStart()
    {
        return m_logStream;
    }

private:
    CLogger              m_logger;
    stringstream         m_logStream;
};

#define LOG_DEBUG if (CLogger::LEVEL_DEBUG >= CLogger::m_baseLevel)\
    CLoggerWrapper(__FILE__, __LINE__, CLogger::LEVEL_DEBUG).LogStart()

#define LOG_INFO if (CLogger::LEVEL_INFO >= CLogger::m_baseLevel)\
    CLoggerWrapper(__FILE__, __LINE__, CLogger::LEVEL_INFO).LogStart()

#define LOG_ERROR if (CLogger::LEVEL_ERROR >= CLogger::m_baseLevel)\
    CLoggerWrapper(__FILE__, __LINE__, CLogger::LEVEL_ERROR).LogStart()