//
// Created by sa on 25-4-26.
//
#ifndef OBJECT_H
#define OBJECT_H

#include <cstddef>

class Object {
    struct ObjHeader {
        size_t size;
        int refNum=0;
        explicit ObjHeader(size_t s):size(s){}
    };
    ObjHeader* gethdr() {
        return reinterpret_cast<ObjHeader*>(
            reinterpret_cast<char*>(this)-sizeof(ObjHeader));
    }
    //突然明白友元怎么用了
    template<typename T>
    friend class HPoint;
public:
    void* operator new(const size_t size);
    void operator delete(void* ptr);
    Object();
    ~Object();
};

#endif // OBJECT_H