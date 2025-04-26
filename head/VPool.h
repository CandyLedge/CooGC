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
    void* base;
    LinkedList<BlockHead>** freeLists;
    Bitmap** bitmaps;

    int forSize(size_t s);
    size_t indexOf(BlockHead* blk, int lvl);
    void merge(BlockHead* blk);

public:
    VPool();
    ~VPool();

    void* allocate(size_t size);
    void del(void* p);
};

#endif // VPOOL_H