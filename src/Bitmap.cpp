#include <cstdint>
#include <iostream>
#include "Bitmap.h"
using namespace std;

Bitmap::Bitmap() : bits(nullptr), bCount(0), BCount(0) {}

Bitmap::Bitmap(size_t bCount):bCount(bCount),BCount((bCount+7)/8) {
    bits=new uint8_t[BCount]();
}

Bitmap::~Bitmap() {
    delete[] bits;
}

void Bitmap::set(size_t index,bool v) {
    if (index >= bCount) {
        cerr << "Bitmap::set 越界: index=" << index << "，bCount=" << bCount << endl;
        return;
    }
    // Bitmap::set 越界: index=4611686018427387890，bCount=8
    // Bitmap::set 越界: index=4611686018427387890，bCount=8

    //除8得字节，位移操作得某一位然后或进去
    if (v) {
        // 设1
        bits[index / 8] |= (1 << (index % 8));
    } else {
        bits[index / 8] &= ~(1 << (index % 8));
    }
}

bool Bitmap::test(size_t index) {
    return bits[index/8]&(1<<index%8);
}

size_t Bitmap::size() const {
    return bCount;
}

void Bitmap::print() {
    for(size_t i=0;i<bCount;i++) {
        if(i%8==0) cout<<endl;
        cout<<(test(i)?1:0);
    }
    cout<<endl;
}
