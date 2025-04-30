#include <iostream>
#include <Object.h>
#include <VPool.h>
using namespace std;

Object::Object() {
    cout<< "Object created\n";
}


Object::~Object() {
    cout << "Object destroyed\n";
}

void* Object::operator new(size_t size) {
    void *block=VPool::getAlone()->allocate(sizeof(ObjHeader)+size);
    new(block) ObjHeader(size);
    return reinterpret_cast<char*>(block)+sizeof(ObjHeader);
}
void Object::operator delete(void* ptr) {
   if (!ptr) return;
    void* block=reinterpret_cast<char*>(ptr)-sizeof(ObjHeader);
    ObjHeader* header=reinterpret_cast<ObjHeader*>(block);
    header->~ObjHeader();
    VPool::getAlone()->del(header);
}
