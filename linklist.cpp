#include <cstdint>
#include <iostream>
using namespace std;
// 双向链表节点
template<typename T>
struct ListNode {
    T* data;
    ListNode* prev;
    ListNode* next;

    ListNode(T* d = nullptr)
        : data(d), prev(nullptr), next(nullptr) {}
};

template<typename T>
class LinkedList {
public:
    LinkedList() : head(nullptr) {}

    // insert head
    void push(T* value) {
        auto* node = new ListNode<T>(value);
        node->next = head;
        if (head) head->prev = node;
        head = node;
    }


    void remove(T* value) {
        for (auto* cur = head; cur; cur = cur->next) {
            if (cur->data == value) {
                if (cur->prev) cur->prev->next = cur->next;
                else head = cur->next;
                if (cur->next) cur->next->prev = cur->prev;
                delete cur;
                return;
            }
        }
    }


    T* pop() {
        if (!head) return nullptr;
        auto* node = head;
        head = head->next;
        if (head) head->prev = nullptr;
        T* val = node->data;
        delete node;
        return val;
    }

    bool empty()  { return head == nullptr; }

    void debugPrint() const {
        std::cout << "Free blocks:";
        if (!head) {
            std::cout << " (empty)\n";
            return;
        }
        char* base = (char*)head->data;
        for (auto* cur = head; cur; cur = cur->next) {
            // 求偏移
            size_t offset = (char*)cur->data - base;
            std::cout << ' ' << offset;
        }
        std::cout << '\n';
    }


    ~LinkedList() {
        while (head) {
            auto* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

private:
    ListNode<T>* head;
};



// int main() {
//     // 模拟“内存池”基地址
//     static char fakePool[1024];
//     void* poolBase = fakePool;
//
//     LinkedList<BlockHeader> list;
//
//     BlockHeader b1{4,false}, b2{5,false}, b3{6,false};
//
//     // 插入测试
//     list.push(&b1);
//     list.push(&b2);
//     list.push(&b3);
//     std::cout << "After push_front b1, b2, b3:\n";
//     list.debugPrint();
//
//     // pop_front 应移除 b3
//     BlockHeader* p = list.pop();
//     assert(p == &b3);
//     std::cout << "After pop_front (b3 removed):\n";
//     list.debugPrint();
//
//     // remove b1
//     list.remove(&b1);
//     std::cout << "After remove b1:\n";
//     list.debugPrint();
//
//     // 清空链表
//     list.pop();  // 移除 b2
//     std::cout << "After removing all, empty? "
//               << (list.empty() ? "yes" : "no") << std::endl;
//
//     return 0;
// }
