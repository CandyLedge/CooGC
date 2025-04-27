//
// Created by sa on 25-4-26.
//
#ifndef VPOOL_H
#define VPOOL_H

#include <cstddef>
#include "BlockHead.h"
#include "LinkedList.h"
#include "Bitmap.h"

class VPool {
    static VPool* alone;//唯一
    void* base;
    LinkedList<BlockHead>** freeLists;
    Bitmap** bitmaps;

    int forSize(size_t s);
    size_t indexOf(BlockHead* blk, int lvl);
    void merge(BlockHead* blk);

    VPool();//私有构造函数
    VPool(const VPool&)=delete;
    VPool& operator=(const VPool&)=delete;
public:
    // VPool();
    static VPool* getAlone();
    ~VPool();

    void* allocate(size_t size);
    void del(void* p);
};

#endif // VPOOL_H