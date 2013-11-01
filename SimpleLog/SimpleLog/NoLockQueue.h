#pragma once

#include <list>

template <typename T>
class CNoLockQueue
{
public:
    CNoLockQueue()
    {
        m_queue.push_back(T());
        m_curIt = m_queue.begin();
    }

    ~CNoLockQueue(){}

private:
    CNoLockQueue(const CNoLockQueue &);
    CNoLockQueue& operator=(const CNoLockQueue &);

public:
    // push完了之后至少有两个元素，一个是刚push的，另一个是*m_curIt
    unsigned long Push(const T &item)
    {
        m_queue.erase(m_queue.begin(), m_curIt);
        m_queue.push_back(item);

        return m_queue.size();
    }

    bool Pop(T &item)
    {
        std::list<T>::iterator it = m_curIt;
        if(++it == m_queue.end())
            return false;

        m_curIt ++;
        item = *m_curIt;
        return true;
    }

private:
    std::list<T>                m_queue;
    typename std::list<T>::iterator      m_curIt;
};
