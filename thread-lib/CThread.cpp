#include "CThread.h"

CThread::CThread() : m_pId(0){}

CThread::~CThread()
{
	m_pId = 0;
}

void* CThread::RunThreadFunc(void* pObj)
{
	CThread* pThread = (CThread*)pObj;
	pThread->Run();
	return NULL;
}

int CThread::Detach(pthread_t pid)
{
	int rc = pthread_detach(pid);
	if (rc != 0)
	{
		Log("call pthread_detach pid = %ld, error = %d, info = %s", pid, rc, strerror(rc));  // 运行日志
	}
	return rc;
}

int CThread::Join(pthread_t pid, void** retVal)
{
	int rc = pthread_join(pid, retVal);	////回收线程 等待线程结束获取线程返回值
	if (rc != 0)
	{
		Log("call pthread_join pid = %ld, error = %d, info = %s", pid, rc, strerror(rc));  // 运行日志
	}
	return rc;
}

int CThread::Cancel(pthread_t pid)
{
	int rc = pthread_cancel(pid);	//结束一个线程
	if (rc != 0)
	{
		Log("call pthread_cancel pid = %ld, error = %d, info = %s", pid, rc, strerror(rc));  // 运行日志
	}
	return rc;
}


int CThread::Start()
{
	int rc = pthread_create(&m_pId, NULL, CThread::RunThreadFunc, (void*)this);	//创建线程
	if (rc != 0)
	{
		Log("call pthread_create error = %d, info = %s", rc, strerror(rc));  // 运行日志
	}
	return rc;
}

int CThread::Yield()
{
	return pthread_yield();
}

void CThread::Stop()
{
	pthread_exit(NULL);	//结束线程，返回NULL
}

// 线程主动分离自己，必须在run函数调用才能生效
int CThread::Detach()
{	
	int rc = pthread_detach(pthread_self());	//分离线程，主线程不必等他结束；
	if (rc != 0)
	{
		Log("call pthread_detach error = %d, info = %s", rc, strerror(rc));  // 运行日志
	}
	return rc;
}
	
pthread_t CThread::GetId()
{
	return pthread_self();
}

const char* CThread::GetName()
{
	return "";
}

void CThread::Run(){}
	


// if(pthread_equal(thread0, pthread_self()))//判断当前调用进程是否为thread0 是返回true