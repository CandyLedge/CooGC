#include <iostream>
#include "head/VPool.h"
#include "head/LinkedList.h"
#include "head/BlockHead.h"
#include "head/Bitmap.h"

using namespace std;

int main() {
    cout << "\033[32m---CooCG   <(^_^<)----\033[0m" << endl;
    cout << "=========================" << endl;

    VPool pool;
    cout << sizeof(ListNode<BlockHead>) << " " << sizeof(BlockHead) << " "
         << sizeof(Bitmap) << " " << sizeof(VPool) << endl;

    // 测试分配和释放
    void* ptr1 = pool.allocate(64);
    cout << "Allocated 64 bytes\n";

    pool.del(ptr1);
    cout << "Deallocated 64 bytes\n";

    return 0;
}