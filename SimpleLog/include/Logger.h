#pragma once
#include <iosfwd>

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
    void LogStart();
    void SetLogContent(const char* logContent);
    void LogFinish();
private:
    const char* GetLogLevelName(LogLevel level);

public:
    static LogLevel      m_baseLevel;//log should be recorded only when m_level >= m_baseLevel
    static bool          m_bAsync;

private:
    std::stringstream*   m_stream;
    char                 szFilepath[512];
    int                  m_nLine;
    LogLevel             m_level;
};