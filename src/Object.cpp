#include <iostream>
#include <VPool.h>
using namespace std;

VPool pool;
class Object {
public:
    void* operator new(const size_t size) {
        void* ptr = pool.allocate(size);
        if (!ptr) throw bad_alloc();  // 如果没有足够内存，抛出异常
        return ptr;
    }

    // 重载 operator delete：模拟删除时不回收内存
    void operator delete(void* ptr) {

        cout << "object delete" << endl;

    }
    Object() {
        cout<<"object is create"<<endl;
    }
    ~Object() {
        cout<<"object was free"<<endl;
    }
};