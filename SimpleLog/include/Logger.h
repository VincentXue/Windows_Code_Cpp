#pragma once

#include <Windows.h>
#include <sstream>

#ifdef SIMPLELOG_EXPORTS
#define SIMPLELOG_API __declspec(dllexport)
#else
#define SIMPLELOG_API __declspec(dllimport)
#endif


class SIMPLELOG_API CLogger
{
public:
    enum LogLevel
    {
        LEVEL_DEBUG,
        LEVEL_INFO,
        LEVEL_ERROR,
        NUM_LOG_LEVELS,
    };

public:
    CLogger(const char* pszCodeFile, int nLine, LogLevel level);
    ~CLogger(void);
    std::stringstream& LogStart();

private:
    void LogFinish();
    const char* GetLogLevelName(LogLevel level);

public:
    static LogLevel      m_baseLevel;
    static BOOL          m_bAsync;

private:
    std::stringstream    m_stream;
    char                 szFilepath[512];
    int                  m_nLine;
    LogLevel             m_level;
};

#define LOG_DEBUG if (CLogger::LEVEL_DEBUG >= CLogger::m_baseLevel)\
    (CLogger(__FILE__, __LINE__, CLogger::LEVEL_DEBUG).LogStart()

#define LOG_INFO if (CLogger::LEVEL_INFO >= CLogger::m_baseLevel)\
    CLogger(__FILE__, __LINE__, CLogger::LEVEL_INFO).LogStart()

#define LOG_ERROR if (CLogger::LEVEL_ERROR >= CLogger::m_baseLevel)\
    CLogger(__FILE__, __LINE__, CLogger::LEVEL_ERROR).LogStart()