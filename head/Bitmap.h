//
// Created by sa on 25-4-26.
//
#ifndef BITMAP_H
#define BITMAP_H

#include <cstdint>
#include <cstddef>
#include <iostream>
using namespace std;

struct Bitmap {
    uint8_t* bits;
    size_t bCount;
    size_t BCount;

    Bitmap();
    Bitmap(size_t bCount);
    ~Bitmap();

    void set(size_t index,bool v);
    bool test(size_t index);
    size_t size() const;
    void print();
};

#endif // BITMAP_H
