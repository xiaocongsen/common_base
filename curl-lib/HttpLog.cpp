#include "HttpLog.h"

void HttpLog::SendLog(const char *MsgGrade,const char *TimeStr,const char *FileName,const char *ApiName,const char*MsgContent)
{
    char buff[1024];
    snprintf(buff,1024,m_DataStr,m_ModuleID,MsgGrade,TimeStr,FileName,ApiName,MsgContent);
    Log("%s\n",buff);
    m_Http->SendPostData(buff);
    m_Http->SendExec();
} 

static HttpLog *pLog = NULL;
void HttpLog::InitLogObj(int ModuleID, const char *DataStr)
{
	static HttpLog logObj; // 全局唯一
    logObj.InitLog(ModuleID, DataStr);
	pLog = &logObj;
}

bool HttpLog::InitLog(int ModuleID, const char *DataStr)
{
    m_DataStr = DataStr;
    m_ModuleID = ModuleID;
    return m_Http->initPostCurl(CCfg::getValue("LogUil"),NULL);
}

void HttpLog::WriteLog(const char *MsgGrade, const char *FileName, const char *ApiName, const char *MsgContent)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    char timeStr[32];
    struct tm *p;
    p=gmtime(&tv.tv_sec);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H:%M:%SZ", p);
    Log("%s\n",timeStr);
    pthread_mutex_lock(&(pLog->mutex));
    pLog->SendLog(MsgGrade,timeStr,FileName,ApiName,MsgContent);
    pthread_mutex_unlock(&(pLog->mutex));
}
