#ifndef HPOINT_H
#define HPOINT_H

#include <cstdint>
#include "Object.h"
#include "SLog.h"

struct HashTools {
    static uintptr_t salt;
    static uintptr_t encode(void* p) { return reinterpret_cast<uintptr_t>(p) ^ salt; }
    static void*    decode(uintptr_t hp) { return reinterpret_cast<void*>(hp ^ salt); }
};
inline uintptr_t HashTools::salt = 120328123456789 ^ 99;

template <typename T>
class HPoint {
    uintptr_t hp = 0;
    bool weak = false;  // 标记是否为弱引用

    Object::ObjHeader* getHdr() const {
        return reinterpret_cast<Object::ObjHeader*>(HashTools::decode(hp));
    }

public:
    HPoint() = default;

    explicit HPoint(T* origin) {
        auto* header = reinterpret_cast<Object::ObjHeader*>(
            reinterpret_cast<char*>(origin) - sizeof(Object::ObjHeader));
        hp = HashTools::encode(header);
        header->refNum++;
        LOG("Created, ref=" << header->refNum);
    }

    HPoint(const HPoint& obj) : hp(obj.hp), weak(obj.weak) {
        if (!weak && hp) {
            getHdr()->refNum++;
            LOG("Copied, ref=" << getHdr()->refNum);
        }
    }

    HPoint(HPoint&& obj) noexcept : hp(obj.hp), weak(obj.weak) {
        obj.hp = 0;
        obj.weak = false;
        LOG("Move-constructed");
    }

    ~HPoint() {
        if (!hp || weak) return;  // 如果是弱或者没对象
        auto* header = getHdr();
        if (--header->refNum == 0) {
            T* obj = reinterpret_cast<T*>(
                reinterpret_cast<char*>(header) + sizeof(Object::ObjHeader));
            obj->~T();
            Object::operator delete(obj);
            LOG("Destroyed, ref=0, object deleted");
        } else {
            LOG("Destroyed, ref=" << header->refNum);
        }
    }

    HPoint& operator=(const HPoint& o) {
        if (this != &o) {
            // 增加新源的引用
            if (!o.weak && o.hp) {
                auto* newHdr = reinterpret_cast<Object::ObjHeader*>(
                    HashTools::decode(o.hp));
                newHdr->refNum++;
            }
            // 释放旧对象
            if (hp && !weak) {
                auto* oldHdr = getHdr();
                if (--oldHdr->refNum == 0) {
                    T* obj = reinterpret_cast<T*>(
                        reinterpret_cast<char*>(oldHdr) + sizeof(Object::ObjHeader));
                    obj->~T();
                    Object::operator delete(obj);
                    LOG("Copy-assign: old object deleted");
                }
            }
            hp = o.hp;
            weak = o.weak;
            LOG("Copy-assigned, ref=" << (hp ? getHdr()->refNum : 0));
        }
        return *this;
    }

    HPoint& operator=(HPoint&& o) noexcept {
        if (this != &o) {
            if (hp && !weak) {
                auto* oldHdr = getHdr();
                if (--oldHdr->refNum == 0) {
                    T* obj = reinterpret_cast<T*>(
                        reinterpret_cast<char*>(oldHdr) + sizeof(Object::ObjHeader));
                    obj->~T();
                    Object::operator delete(obj);
                    LOG("Move-assign: old object deleted");
                }
            }
            hp = o.hp;
            weak = o.weak;
            o.hp = 0;
            o.weak = false;
            LOG("Move-assigned");
        }
        return *this;
    }

    T* get() const {
        if (!hp) return nullptr;
        return reinterpret_cast<T*>(
            reinterpret_cast<char*>(getHdr()) + sizeof(Object::ObjHeader));
    }

    void reset() {
        if (!hp || weak) return;
        auto* hdr = getHdr();
        if (--hdr->refNum == 0) {
            T* obj = reinterpret_cast<T*>(
                reinterpret_cast<char*>(hdr) + sizeof(Object::ObjHeader));
            obj->~T();
            Object::operator delete(obj);
            LOG("Reset: ref=0, object deleted");
        } else {
            LOG("Reset: ref=" << hdr->refNum);
        }
        hp = 0;
    }

    static HPoint weakRef(T* origin) {
        HPoint w;
        w.weak = true;
        auto* header = reinterpret_cast<Object::ObjHeader*>(
            reinterpret_cast<char*>(origin) - sizeof(Object::ObjHeader));
        w.hp = HashTools::encode(header);
        LOG("WeakRef created");
        return w;
    }

    T& operator*() const { return *get(); }
    T* operator->() const { return get(); }
    bool valid() const { return hp != 0; }
};

#endif // HPOINT_H
