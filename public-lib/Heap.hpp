#ifndef HEAP_H
#define HEAP_H

#include <string.h>
#include "LLog.h"
template<class T> class Heap;

template<class T>
class HeapNode {
public:
    HeapNode() {
        heapIndex = -1;
        heap = NULL;
    }
    Heap<T>* heap;
    int heapIndex;

    void removeFromHeap()
    {
        if(heap)
            heap->del(heapIndex);
    }

    void setIndex(int index)
    {
        heapIndex = index;
    }
};

template<class T>
class Heap
{
public:
    Heap(int(*compare)(T*, T*),
         int capacity=1024*100, int step = 1024*20):
        compare(compare),
        data(nullptr),
        capacity(capacity),
        count(0),
        step(step)
    {
        data = new T*[this->capacity];
    }

    int (*compare)(T* o, T* p);

    void swap(int p, int i)
    {
        T* pp = data[p];
        T* pi = data[i];

        resetPostion(pp, i);
        resetPostion(pi, p);
    }
    void resetPostion(T* o, int index)
    {
        data[index] = o;
        o->setIndex(index);
    }

    void add(T* p)
    {
        TR("add node=%p to heap=%p\n", p, this);
        if(p->heap)
        {
            if(p->heap == this)
            {
                return;
            }

            TR("warning: add node %p to heap %p, but it is in %p\n", p, this, p->heap);
            p->heap->del(p->heapIndex);
        }

        extend();
        resetPostion(p, this->count);
        this->count++;
        addAdjust(this->count-1);
        p->heap = this;
    }
    T* del(int idx)
    {
        if(this->count == 0)
            return NULL;
        if(idx >= this->count)
            return NULL;
        if(idx < 0)
            return NULL;


        T* ret = data[idx];
        T* last = data[--this->count];

        // find the hole
        int hole = delAdjust(idx);

        // set last one --> hole
        resetPostion(last, hole);

        // adjust as ADD by hole
        addAdjust(hole);

        ret->heap = NULL;
        ret->heapIndex = -1;

        return ret;
    }

    void addAdjust(int i)
    {
        int p;
        while(i > 0)
        {
            p = (i-1)/2; // parent is (i-1) / 2
            // parent is large then child, just return
            int result = this->compare(data[p], data[i]);
            if(result > 0)
            {
                break;
            }

            swap(p, i);

            i = p;
        }
    }
    int delAdjust(int i)
    {
        while(1)
        {
            // left and right child
            int l = i*2+1;
            int r = i*2+2;
            // if left is NULL, then right is NULL too, mean i is the last
            if(l >= count)
                return i;

            // if right is NULL, mean left is the LAST
            if(r >= count)
            {
                swap(i, l);
               // heap->data[i] = heap->data[l];
                return l;
            }

            // if left and right both exist, select One replace i
            int res = compare(data[l], data[r]);
          //  int res = heap->cmp(heap->data[l], heap->data[r]);
            // left is max then right
            // Larget node to parent
            if(res > 0)
            {
                swap(i, l);
                //heap->data[i] = heap->data[l];
                i = l;
            }
            else
            {
                swap(i, r);
            //    heap->data[i] = heap->data[r];
                i = r;
            }
        }
        return -1;
    }

    void extend()
    {
        if(this->capacity == this->count)
        {
            this->capacity = this->count + this->step;
            T** tmp = data;
            this->data = new T*[this->capacity];
            memcpy(this->data, tmp, sizeof(T*) * this->count);
            delete [] tmp;
        }
    }

    T* get(int idx = 0)
    {
        if(idx < 0)
        {
            return nullptr;
        }

        if(this->count > idx)
        {
            return data[idx];
        }
        return nullptr;
    }

    bool isContain(T* obj)
    {
        if(get(obj->heapIndex) == obj)
            return true;
        return false;
    }

    T** data;
    int capacity;
    int count;
    int step;
};

#endif // HEAP_H

