#pragma once
#include <stdio.h>
#include <string>
#include "LockHelper.h"
#include "LogFile.h"

class CSyncLogging
{
public:
    CSyncLogging(const char *pszFilePath=NULL);
    ~CSyncLogging(void);

    void Append(const std::string& log);

private:
    std::string GetDefaultLogFilePath();
    std::string GetRotatedFilePath();
    std::string GetFilePath(int fileNo);
    void FlushFile();

private:
    CLogFile                m_logFile;
    std::string             m_sPath;
    int                     m_fileNo;
    CCriticalSection        m_cs;
    time_t                  m_lastFlushStamp;

};