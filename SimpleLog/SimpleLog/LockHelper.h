#pragma once
#include <Windows.h>

class CCriticalSection
{
public:
    CCriticalSection()
    {
        ::InitializeCriticalSection(&m_cs);
    }

    ~CCriticalSection()
    {
        ::DeleteCriticalSection(&m_cs);
    }

public:
    void Lock()
    {
        ::EnterCriticalSection(&m_cs);
    }

    void Unlock()
    {
        ::LeaveCriticalSection(&m_cs);
    }

private:
    CRITICAL_SECTION    m_cs;
};

template<typename LockType>
class CLockGuard
{
public:
    CLockGuard(LockType &lock)
        : m_lock(lock)
    {
        m_lock.Lock();
    }

    ~CLockGuard()
    {
        m_lock.Unlock();
    }
private:
    LockType    &m_lock;
};