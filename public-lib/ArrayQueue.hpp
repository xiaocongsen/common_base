#ifndef ONE_TO_ONE_LOCKLESS_QUEUE_H
#define ONE_TO_ONE_LOCKLESS_QUEUE_H
//单线程无所队列 

#include <semaphore.h>
#include <atomic>

template <typename T>
class ArrayQueue
{
public:
    ArrayQueue(unsigned int nSize = 10000);
    ~ArrayQueue();
    void OneToOneLocklessPop(T& obj);
    void OneToOneLocklessPush(T& obj);
    void OneToManyLocklessPop(unsigned int &index ,T &obj);
private:
    sem_t m_Blank;      //缓冲区剩余大小
    sem_t m_Data;       //数据数量
    T *m_Buffer;        //缓冲区指针
    // int m_PopIndex;     
    std::atomic<unsigned int> m_PopIndex;   //Pop当前序号
    std::atomic<unsigned int> m_PushIndex;  //push当前序号
    // int m_PushIndex;    
    unsigned int BuffSize;//缓存区大小
    pthread_mutex_t mutex;
};

template <typename T>
ArrayQueue<T>::ArrayQueue(unsigned int nSize):m_PopIndex(0),m_PushIndex(0)
{
    BuffSize = nSize;
    pthread_mutex_init(&mutex, (const pthread_mutexattr_t*)0);//初始化锁；
    sem_init(&m_Blank, 0, BuffSize);
    sem_init(&m_Data, 0, 0);
    m_Buffer = new T[BuffSize];
    // m_PopIndex = 0;
    // m_PushIndex = 0;
}

template <typename T>
ArrayQueue<T>::~ArrayQueue()
{
    sem_destroy(&m_Blank); //销毁信号量；
    sem_destroy(&m_Data);  //销毁信号量；
    delete[] m_Buffer;
}

template <typename T>
void ArrayQueue<T>::OneToOneLocklessPop(T &ret)
{
    // static T ret;
    sem_wait(&m_Data);
    ret = m_Buffer[m_PopIndex++];
    m_PopIndex = m_PopIndex%BuffSize;
    sem_post(&m_Blank);
    // return ret;
}

template <typename T>
void ArrayQueue<T>::OneToOneLocklessPush(T &obj)
{
    sem_wait(&m_Blank);
    m_Buffer[m_PushIndex++] = obj;
    m_PushIndex = m_PushIndex%BuffSize;
    sem_post(&m_Data);
}

template <typename T>
void ArrayQueue<T>::OneToManyLocklessPop(unsigned int &index ,T &obj)
{
    sem_wait(&m_Data);
    index = m_PopIndex.fetch_add(1,std::memory_order_relaxed);
    obj = m_Buffer[index%BuffSize];
    sem_post(&m_Blank);
//  //---------------------------------
    // sem_wait(&m_Data);
    // pthread_mutex_lock(&mutex);//加锁
    // obj = m_Buffer[m_PopIndex++];
    // m_PopIndex %= BuffSize;
    // pthread_mutex_unlock(&mutex);//解锁
    // sem_post(&m_Blank);
}

#endif