#include "StdAfx.h"
#include "LogThread.h"
#include <process.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include "AsyncLogging.h"
#include "SyncLogging.h"

extern CSyncLogging g_SyncLogging;

using namespace std;

CLogThread::CLogThread(void)
    : m_hThread(NULL)
    , m_logging(NULL)
    , m_hLogNotifyEvent(NULL)
    , m_bQuit(FALSE)
{
}

CLogThread::~CLogThread(void)
{
    Quit();
    CloseHandle(m_hLogNotifyEvent);
}

unsigned int __stdcall CLogThread::threadProc(void *pArg)
{
    CLogThread *pThread = (CLogThread*)pArg;
    return pThread->Run();
}

BOOL CLogThread::Start(CAsyncLogging *logging)
{
    if(m_hThread != 0)
    {
        return TRUE;
    }

    m_logging = logging;
    m_bQuit = FALSE;

    m_hLogNotifyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(NULL == m_hLogNotifyEvent)
    {
        return FALSE;
    }
    
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, CLogThread::threadProc, this, 0, NULL);
    if(m_hThread == NULL)
    {
        return FALSE;
    }

    return TRUE;
}

unsigned int CLogThread::Run()
{
    while (!m_bQuit)
    {
        if(!Log())
        {
            return -1;
        }
    }

    return 0;
}


BOOL CLogThread::Quit()
{
    if(NULL != m_hThread)
    {
        return TRUE;
    }

    m_bQuit = TRUE;
    
    DWORD dwWaitResult = WaitForSingleObject(m_hThread, 30*1000);
    if(dwWaitResult != WAIT_OBJECT_0)
    {
        if(WAIT_FAILED == dwWaitResult)
        {
            assert(false);
            return FALSE;
        }
        else if(WAIT_TIMEOUT == dwWaitResult)
        {
            // 30*1000ms内线程还未退出，强制结束线程（不应该发生）
            TerminateThread(m_hThread, -1);
            assert(false);
        }
    }

    CloseHandle(m_hThread);
    m_hThread = NULL;

    return TRUE;
}

BOOL CLogThread::NotifyLogging()
{
    return SetEvent(m_hLogNotifyEvent);
}

BOOL CLogThread::Log()
{
    DWORD dwWaitResult = WaitForSingleObject(m_hLogNotifyEvent, 1000);
    if(dwWaitResult == WAIT_FAILED)
    {
        assert(false);
        return FALSE;
    }

    string log;
    while( m_logging->GetLog(log) )
    {
        g_SyncLogging.Append(log.c_str());
    }
    return TRUE;
}
