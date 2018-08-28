
/* author : limingfan
 * date : 2014.10.21
 * description : 线程管理API封装
 */
 
#ifndef CTHREAD_H
#define CTHREAD_H

#include <pthread.h>
#include <string.h>
#include "stdafx.h"

class CThread
{
public:
	CThread();
	virtual ~CThread();

public:
    // 线程管理api
	static int Detach(pthread_t pid);  // 分离线程
	static int Join(pthread_t pid, void** retVal);    // 等待线程结束
	static int Cancel(pthread_t pid);    // 取消停止线程
	

public:
	int Start();         // 创建线程并执行run函数调用
	
	// 以下函数只能在线程运行函数run内调用
	int Yield();         // 挂起线程，只能在线程内主动调用
	int Detach();        // 线程主动分离自己，必须在run函数调用才能生效
	void Stop();         // 停止线程，只能在线程内主动调用
	virtual void Run();  // 线程实现者重写run
	
public:
	pthread_t GetId();   // 线程id
	virtual const char* GetName();  // 线程名称

private:
    // 禁止拷贝、赋值
    CThread(const CThread&);
    CThread& operator =(const CThread&);
	
private:
    pthread_t m_pId;


    static void* RunThreadFunc(void* pObj);
};


#endif // CTHREAD_H



