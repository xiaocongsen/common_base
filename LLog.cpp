#include "LLog.h"

void LLog::SendLog(const char *MsgGrade,const char *TimeStr,const char *FileName,const char *ApiName,const char*MsgContent)
{
    char buff[1024];
    snprintf(buff,1024,m_DataStr,m_ModuleID,MsgGrade,TimeStr,FileName,ApiName,MsgContent);
    printf("%s\n",buff);
    m_Http->SendPostData(buff);
    m_Http->SendExec();
} 

static LLog *pLog = NULL;
void LLog::InitLogObj(int ModuleID, const char *DataStr)
{
	static LLog logObj; // 全局唯一
    logObj.InitLog(ModuleID, DataStr);
	pLog = &logObj;
}

void LLog::WriteLog(const char *MsgGrade, const char *FileName, const char *ApiName, const char *MsgContent)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    char timeStr[32];
    struct tm *p;
    p=gmtime(&tv.tv_sec);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H:%M:%SZ", p);
    printf("%s\n",timeStr);
    pthread_mutex_lock(&(pLog->mutex));
    pLog->SendLog(MsgGrade,timeStr,FileName,ApiName,MsgContent);
    pthread_mutex_unlock(&(pLog->mutex));
}
