#include "StdAfx.h"
#include "SyncLogging.h"
#include "Utils.h"

using namespace std;

static const int LOGFILE_MAXSIZE = 1024*1024*30;       //每个日志文件最大为30M
static const int LOGFILE_MAXNUM  = 8;                  //一个进程最多有8个日志文件

CSyncLogging::CSyncLogging(const char *pszFilePath/*=NULL*/)
    : m_fileNo(-1)
    , m_lastFlushStamp(time(NULL))
{
    if(!pszFilePath)
    {
        m_sPath = GetDefaultLogFilePath();
    }
    else
    {
        m_sPath = pszFilePath;
    }

    m_logFile.Open(m_sPath);
}

CSyncLogging::~CSyncLogging(void)
{
    m_logFile.Close();
}

string CSyncLogging::GetDefaultLogFilePath()
{
    char szPid[64] = {0};
    sprintf(szPid, "%d", Utils::GetPid());
    string sImagePath = Utils::GetProcessImageFilePath();
    string sLogFileName = string(Utils::GetBaseName(sImagePath.c_str())) + "_" + szPid + "@"\
        + Utils::GetHostName() + ".log";
    string sLogParentPath = Utils::GetParentPath(sImagePath.c_str()) + "log\\";

    if(!Utils::PathExists(sLogParentPath.c_str()))
    {
        Utils::MakeDirs(sLogParentPath.c_str());
    }
    string sPath = sLogParentPath + sLogFileName;
    return sPath;
}

void CSyncLogging::Append(const string& log)
{
    CLockGuard<CCriticalSection> guard(m_cs);
    
    if (m_logFile.GetLength() >= LOGFILE_MAXSIZE)
    {
        m_logFile.Close();
        m_logFile.Open(GetRotatedFilePath());
    }

    m_logFile.Write(log);
    FlushFile();
}

std::string CSyncLogging::GetRotatedFilePath()
{
    if( m_fileNo == LOGFILE_MAXNUM )
    {
        string sFirstFilePath = GetFilePath(0);
        remove(sFirstFilePath.c_str());
        for(int i=0; i<LOGFILE_MAXNUM; ++i)
        {
            rename(GetFilePath(i+1).c_str(), GetFilePath(i).c_str());
        }
    }
    else
    {
        m_fileNo ++;
    }
    return GetFilePath(m_fileNo);
}

std::string CSyncLogging::GetFilePath(int fileNo)
{
    char szFileNo[16] = {0};
    _snprintf(szFileNo, sizeof(szFileNo)/sizeof(szFileNo[0]), "_%02d", fileNo);

    string sParentPath = Utils::GetParentPath(m_sPath.c_str());
    string sBaseName = Utils::GetBaseName(m_sPath.c_str());
    size_t pos = sBaseName.find_last_of('.');
    if (string::npos != pos)
    {
        sBaseName.insert(pos, szFileNo);
    }
    else
    {
        sBaseName.append(szFileNo);
    }

    return sParentPath+sBaseName;
}

void CSyncLogging::FlushFile()
{
    time_t curStamp = time(NULL);
    if(curStamp-m_lastFlushStamp >= 1)
    {
        m_logFile.Flush();
        m_lastFlushStamp = curStamp;
    }
}