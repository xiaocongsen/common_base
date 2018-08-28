#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include <stdlib.h>
#include <memory>
#include <queue>
#include <vector>
#include <assert.h>

template <typename T>
class ObjectPool
{
  public:
    //初始化块大小如果能预先估算出块大小则性能达到最大
    //即:只分配一次内存只释放一次内存
    ObjectPool(unsigned int ChunkSize = 32);

    virtual ~ObjectPool();

    //设置块大小
    void SetChunkSize(unsigned int ChunkSize);

    //模拟new关键字分配内存并调用构造函数
    T *New();

    //模拟delete关键字释放内存调用析构函数
    void Delete(T *pT);

  private:
    //分配块大小
    void AllocateChunk(unsigned int ChunkSize);

    //释放块
    void ReleaseChunk();

  private:
    //空闲池
    std::queue<T *> mFreePool;

    //块池
    std::vector<T *> mChunkPool;

    //当前块大小
    unsigned int mChunkSize;
};

template <typename T>
ObjectPool<T>::ObjectPool(unsigned int ChunkSize)
    : mChunkSize(ChunkSize)
{
}

template <typename T>
ObjectPool<T>::~ObjectPool()
{
    ReleaseChunk();
}

template <typename T>
void ObjectPool<T>::SetChunkSize(unsigned int ChunkSize)
{
    if (0 == ChunkSize)
    {
        assert(false);
        return;
    }
    mChunkSize = ChunkSize;
    if (mFreePool.empty())
    {
        return;
    }
    if (mFreePool.size() < mChunkSize)
    {
        AllocateChunk(mChunkSize - mFreePool.size());
    }
}

template <typename T>
T *ObjectPool<T>::New()
{
    if (mFreePool.empty())
    {
        AllocateChunk(mChunkSize);
    }
    if (mFreePool.empty())
    {
        assert(false);
        return 0;
    }
    T *pT = mFreePool.front();
    mFreePool.pop();
    new (pT) T();
    return pT;
}

template <typename T>
void ObjectPool<T>::Delete(T *pT)
{
    if (0 == pT)
    {
        assert(false);
        return;
    }
    //pT->~T();
    mFreePool.push(pT);
}

template <typename T>
void ObjectPool<T>::AllocateChunk(unsigned int ChunkSize)
{
    if (0 == ChunkSize)
    {
        assert(false);
        return;
    }
    T *pT = reinterpret_cast<T *>(malloc(ChunkSize * sizeof(T)));
    if (0 == pT)
    {
        assert(false);
        return;
    }

    for (unsigned int i = 0; i < ChunkSize; ++i)
    {
        mFreePool.push(pT + i);
    }
    mChunkPool.push_back(pT);
}

template <typename T>
void ObjectPool<T>::ReleaseChunk()
{
    for (unsigned int i = 0; i < mChunkPool.size(); ++i)
    {
        free(mChunkPool[i]);
        mChunkPool[i] = 0;
    }
    mChunkPool.clear();
}
#endif //OBJECT_POOL_HPP