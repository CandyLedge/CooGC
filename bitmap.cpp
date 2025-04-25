#include <cstdint>
#include <iostream>
using namespace std;
struct Bitmap {
    uint8_t* bits;
    size_t bCount;
    size_t BCount;

    Bitmap() : bits(nullptr), bCount(0), BCount(0) {}
    //+7/8刚好向上取整
    Bitmap(size_t bCount):bCount(bCount),BCount((bCount+7)/8) {
        bits=new uint8_t[BCount]();
    }

    ~Bitmap() {
        delete[] bits;
    }

    void set(size_t index,bool v) {
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

    bool test(size_t index) {
        return bits[index/8]&(1<<index%8);
    }

    size_t size() const {
        return bCount;
    }

    void print() {
        for(size_t i=0;i<bCount;i++) {
            if(i%8==0) cout<<endl;
            cout<<(test(i)?1:0);
        }
        cout<<endl;
    }
};

// int main() {
//     Bitmap bitmap(20); // 创建一个包含 20 位的位图

//     bitmap.set(3, true);  // 设置第 3 位为 1
//     bitmap.set(15, true); // 设置第 15 位为 1
//     bitmap.set(3, false); // 设置第 3 位为 0
//
//     // 打印位图的当前状态
//     bitmap.print(); // 输出：00000000000000010000
//
//     // 再设置一些位
//     bitmap.set(7, true);  // 设置第 7 位为 1
//     bitmap.set(9, true);  // 设置第 9 位为 1
//     bitmap.set(16, true); // 设置第 16 位为 1
//
//     // 打印位图的当前状态
//     bitmap.print(); // 输出：01000010010000010000
//
//     return 0;
// }
