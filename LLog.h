#ifndef L_LOG_H
#define L_LOG_H
#include "httpBase.h"
#include "CCfg.h"

class LLog
{
  private:
    // 禁止拷贝、赋值
    LLog(const LLog &);
    LLog &operator=(const LLog &);
    LLog()
    {
        m_Http = new httpBase;
        pthread_mutex_init(&mutex, NULL);
        // m_DataStr =  "{\"ModuleID\":222,\"ModuleName\":\"entruct\",\"MsgGrade\":\"%s\",\"MsgTime\":\"2018-07-19T12:23:00Z\",\"FileName\":\"%s\",\"ApiName\":\"%s\",\"Descript\":\"%s\",\"MsgContent\":\"%s\",\"Coder\":\"xiaocongsen\",\"IPAddress\":\"192.168.1.x\"}";
    }

  public:
    ~LLog() {}
    static void InitLogObj(int ModuleID, const char *DataStr);

    static void WriteLog(const char *MsgGrade, const char *FileName, const char *ApiName, const char *MsgContent);
    bool InitLog(int ModuleID, const char *DataStr)
    {
        m_DataStr = DataStr;
        m_ModuleID = ModuleID;
        return m_Http->initPostCurl(CCfg::getValue("LogUil"),NULL);
    }
    void SendLog(const char *MsgGrade,const char *TimeStr, const char *FileName, const char *ApiName, const char *MsgContent);
    
    pthread_mutex_t mutex;
  private:
    
    int m_ModuleID;
    httpBase *m_Http;
    const char *m_DataStr;
};

#endif //L_LOG_H