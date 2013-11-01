#include "StdAfx.h"
#include "AsyncLogging.h"

using namespace std;

CAsyncLogging::CAsyncLogging(void)
{
    m_logThread.Start(this);
}


CAsyncLogging::~CAsyncLogging(void)
{
}

void CAsyncLogging::GetLogs(CLogList &logs)
{
    logs.clear();
    string log;
    while( m_logQueue.Pop(log) )
    {
        logs.push_back(log);
    }
}

void CAsyncLogging::AppendLog( const std::string &log )
{
    m_logQueue.Push(log);
}

bool CAsyncLogging::GetLog( string &log )
{
    return m_logQueue.Pop(log);
}
