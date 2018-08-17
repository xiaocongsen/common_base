#ifndef HTTP_BASE_H
#define HTTP_BASE_H


#include <curl/multi.h>
#include <iostream>
#include <string.h>
using namespace std;
class httpBase
{
public:
	httpBase(){}
    ~httpBase()
    {
        curl_easy_cleanup(m_EasyHandle);
        m_EasyHandle = NULL;
        curl_global_cleanup();
    }
    bool initPostCurl(const char *uil,void *write_callback);
    bool initGetCurl(const char *uil,void *write_callback);
    void SendPostData(const char *nData);
    bool SendExec();
    void SetBasicData(const char *data);

// static char data[8192];
// static int curIndex;

// static size_t write_callback(char *ptr,size_t size, size_t nmemb, void *userdata);

private:
    CURL *  m_EasyHandle;
};

#endif //HTTP_BASE_H
