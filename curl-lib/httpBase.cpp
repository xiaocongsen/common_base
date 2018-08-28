#include "httpBase.h"


bool httpBase::initPostCurl(const char *uil,void *write_callback)
{
    m_EasyHandle = curl_easy_init();
    if (m_EasyHandle)
    {
        curl_easy_setopt(m_EasyHandle, CURLOPT_URL, uil);
        curl_easy_setopt(m_EasyHandle, CURLOPT_POST, 1);
        if(write_callback != NULL)
            curl_easy_setopt(m_EasyHandle, CURLOPT_WRITEFUNCTION, write_callback);
        return true;
    }
    return false;
}

bool httpBase::initGetCurl(const char *uil,void *write_callback)
{
    m_EasyHandle = curl_easy_init();
    if (m_EasyHandle)
    {
        curl_easy_setopt(m_EasyHandle, CURLOPT_URL, uil);
        if(write_callback != NULL)
            curl_easy_setopt(m_EasyHandle, CURLOPT_WRITEFUNCTION, write_callback);
        return true;
    }
    return false;
}

void httpBase::SetBasicData(const char *data)
{
    curl_easy_setopt(m_EasyHandle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(m_EasyHandle, CURLOPT_USERPWD, data);
}

void httpBase::SendPostData(const char *nData)
{
    curl_easy_setopt(m_EasyHandle, CURLOPT_POSTFIELDS, nData);
}

bool httpBase::SendExec()
{
    if (m_EasyHandle)
    {
        CURLcode res = curl_easy_perform(m_EasyHandle);
        if (res != CURLE_OK)
        {
            Log("error SendPacket curl_easy_perform res = %d" , res);
            return false;
        }
    }
    return true;
}