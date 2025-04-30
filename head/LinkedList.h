//
// Created by sa on 25-4-26.
//
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
using namespace std;

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
            std::cout << " (empty)";
            return;
        }
        char* base = (char*)head->data;
        for (auto* cur = head; cur; cur = cur->next) {
            // 求偏移
            size_t offset = (char*)cur->data - base;
            std::cout << ' ' << offset;
        }
        std::cout << "" << endl;
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

#endif // LINKEDLIST_H
