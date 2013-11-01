#pragma once

class CAsyncLogging;
class CLogThread
{
public:
    CLogThread(void);
    ~CLogThread(void);

    BOOL Start(CAsyncLogging *logging);
    BOOL NotifyLogging();

private:
    BOOL Quit();
    BOOL Log();
    virtual unsigned int Run();
    static unsigned int __stdcall threadProc(void *pArg);

private:
    HANDLE          m_hThread;
    CAsyncLogging*  m_logging;
    HANDLE          m_hLogNotifyEvent;
    BOOL            m_bQuit;
};

