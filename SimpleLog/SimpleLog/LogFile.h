#pragma once

#include <string>

class CLogFile
{
public:
    CLogFile(void);
    ~CLogFile(void);

    
    void Open(const std::string &filepath);
    void Close();
    void Write(const std::string &log);
    void Flush();
    long GetLength();
private:
    long GetFileLength(FILE *pFile);

private:
    FILE*   m_pFile;
    char    m_logFlag[16];
    char    m_logBuf[64*1024];
    long    m_length;
};

