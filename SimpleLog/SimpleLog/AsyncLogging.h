#pragma once
#include "LogThread.h"
#include <list>
#include <string>
#include <Windows.h>
#include "NoLockQueue.h"
#include "LockHelper.h"

class CAsyncLogging
{
    friend class CLogThread;
    typedef std::list<std::string> CLogList;
public:
    CAsyncLogging(void);
    ~CAsyncLogging(void);
    
    void AppendLog(const std::string &log);

private:
    void GetLogs(CLogList &logs);
    bool GetLog(std::string &log);
private:
    CNoLockQueue<std::string>       m_logQueue;
    CLogThread                      m_logThread;
    CCriticalSection                m_cs;
};

