#ifndef HTTP_LOG_H
#define HTTP_LOG_H
#include "httpBase.h"
#include "CCfg.h"

class HttpLog
{
  private:
    // 禁止拷贝、赋值
    HttpLog(const HttpLog &);
    HttpLog &operator=(const HttpLog &);
    HttpLog()
    {
        m_Http = new httpBase;
        pthread_mutex_init(&mutex, NULL);
    }

  public:
    ~HttpLog() {}
    static void InitLogObj(int ModuleID, const char *DataStr);

    static void WriteLog(const char *MsgGrade, const char *FileName, const char *ApiName, const char *MsgContent);
    bool InitLog(int ModuleID, const char *DataStr);
    void SendLog(const char *MsgGrade,const char *TimeStr, const char *FileName, const char *ApiName, const char *MsgContent);
    
    pthread_mutex_t mutex;
  private:
    
    int m_ModuleID;
    httpBase *m_Http;
    const char *m_DataStr;
};

#endif //HTTP_LOG_H