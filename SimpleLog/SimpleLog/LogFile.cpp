#include "StdAfx.h"
#include "LogFile.h"
#include <assert.h>
#include <algorithm>

using namespace std;
CLogFile::CLogFile(void)
    : m_pFile(NULL)
    , m_length(0)
{
    strncpy(m_logFlag, "_log_buf_flag_", sizeof(m_logFlag)/sizeof(m_logFlag[0]));
    memset(m_logBuf, 0, sizeof(m_logBuf));
}

CLogFile::~CLogFile(void)
{
}

void CLogFile::Open( const std::string &filepath )
{
    assert(NULL == m_pFile);
    m_pFile = fopen(filepath.c_str(), "a+");
    assert(NULL != m_pFile);
    memset(m_logBuf, 0, sizeof(m_logBuf));
    setvbuf(m_pFile, m_logBuf, _IOFBF, sizeof(m_logBuf));

    m_length = GetFileLength(m_pFile);
}

void CLogFile::Close()
{
    if(m_pFile != NULL)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

void CLogFile::Write( const std::string &log )
{
    if(NULL == m_pFile)
    {
        return;
    }

    fwrite(log.c_str(), log.length(), 1, m_pFile);
    
    m_length += log.length() + count(log.begin(), log.end(), '\n');
}

long CLogFile::GetFileLength( FILE *pFile )
{
    if(NULL == pFile)
    {
        return 0;
    }

    long oldPos = ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    long fileSize = ftell(pFile);
    fseek(pFile, oldPos, SEEK_SET);

    return fileSize;
}

void CLogFile::Flush()
{
    if(NULL == m_pFile)
    {
        return;
    }

    fflush(m_pFile);
}

long CLogFile::GetLength()
{
    return m_length;
}
