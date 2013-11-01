#include "StdAfx.h"
#include "Logger.h"
#include "SyncLogging.h"
#include "Utils.h"
#include "AsyncLogging.h"

using namespace std;

CSyncLogging g_SyncLogging;
CAsyncLogging g_AsyncLogging;

CLogger::LogLevel CLogger::m_baseLevel = CLogger::LEVEL_DEBUG;
BOOL CLogger::m_bAsync = FALSE;


CLogger::CLogger( const char* pszFileName, int nLine, LogLevel level )
    : m_nLine(nLine)
    , m_level(level)
{
    strncpy(szFilepath, pszFileName, sizeof(szFilepath)/sizeof(szFilepath[0]));
}

CLogger::~CLogger(void)
{
    LogFinish();

    if(m_bAsync)
    {
        g_AsyncLogging.AppendLog(m_stream.str());
    }
    else
    {
        g_SyncLogging.Append(m_stream.str().c_str());
    }
}

stringstream& CLogger::LogStart()
{
    char buf[32] = {0};
    m_stream << "[" << Utils::GetFormatTimeStamp(buf, sizeof(buf)) 
        << " pid=" << Utils::GetPid() << " tid=" << Utils::GetTid()<<"]" 
        << " [" << GetLogLevelName(m_level) <<"] ";
    return m_stream;
}

void CLogger::LogFinish()
{
    m_stream << " - " << Utils::GetBaseName(szFilepath) << "(" << m_nLine << ")\n";
}

const char* CLogger::GetLogLevelName( LogLevel level )
{
    static const char* aryLoglevelName[] = { "DEBUG", "INFO", "ERROR"};
    if (level < 0 || level >= NUM_LOG_LEVELS)
    {
        return "UNKOWN";
    }
    return aryLoglevelName[level];
}