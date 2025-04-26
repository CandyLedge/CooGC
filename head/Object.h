//
// Created by sa on 25-4-26.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <cstddef>
#include <new>
#include <iostream>
#include "VPool.h"

extern VPool pool;

class Object {
public:
    void* operator new(const size_t size);
    void operator delete(void* ptr);
    Object();
    ~Object();
};

#endif // OBJECT_H