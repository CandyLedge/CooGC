#include <iostream>
#include "linklist.cpp"
#include "bitmap.cpp"
using namespace std;

static int maxOrder = 10;
static int minOrder = 1;
static int poolSize = 1 << maxOrder;
static int levels = maxOrder - minOrder + 1;

struct BlockHead {
    int k;
    bool used;
};

class VPool {
    void* base;
    LinkedList<BlockHead>** freeLists;
    Bitmap** bitmaps;

    int forSize(size_t s) {
        size_t total = s + sizeof(BlockHead);
        int k = minOrder;
        size_t block = size_t(1) << k;
        while (block < total && k < maxOrder) {
            k++;
            block <<= 1;
        }
        return k - minOrder;
    }

    size_t indexOf(BlockHead* blk, int lvl) {
        size_t off = (char*)blk - (char*)base;
        // return off>>(minOrder+lvl);
        size_t index = off >> (minOrder + lvl);
        return index;
    }

    void merge(BlockHead* blk) {
        while (blk->k < maxOrder) {
            size_t size = size_t(1) << blk->k;
            size_t off = (char*)blk - (char*)base;
            size_t mateOff = off ^ size;

            BlockHead* mate = (BlockHead*)((char*)base + mateOff);

            if (!mate->used && mate->k == blk->k) {
                int lvl = blk->k - minOrder;
                freeLists[lvl]->remove(mate);
                bitmaps[lvl]->set(indexOf(mate, lvl), 0);
                if (blk > mate) blk = mate;
                blk->k++;
                continue;
            }
            break;
        }
        int lvl = blk->k - minOrder;
        freeLists[lvl]->remove(blk);
        freeLists[lvl]->push(blk);
        bitmaps[lvl]->set(indexOf(blk, lvl), 1);
    }

public:
    // void* base;
    // LinkedList<BlockHead> freeLists[levels];
    // Bitmap bitmaps[levels];
    // int k;
    // bool used;

    VPool() {
        // 分配 freeLists 和 bitmaps
        freeLists = new LinkedList<BlockHead>*[levels];
        bitmaps = new Bitmap*[levels];
        base = malloc(poolSize);

        // 统一初始化各级别的链表和位图
        for (int lvl = 0; lvl < levels; lvl++) {
            freeLists[lvl] = new LinkedList<BlockHead>();
            bitmaps[lvl] = new Bitmap(poolSize >> (minOrder + lvl));
        }

        // 将整块内存作为根块添加到最高级
        BlockHead* root = reinterpret_cast<BlockHead*>(base);
        root->k = maxOrder;
        root->used = false;
        int topLvl = maxOrder - minOrder;
        freeLists[topLvl]->push(root);
        bitmaps[topLvl]->set(0, true);

        // 旧
        // freeLists = new LinkedList<BlockHead>*[levels];
        // bitmaps = new Bitmap[levels];
        // base = malloc(poolSize);
        // BlockHead* root = (BlockHead*)base;
        // root->k = maxOrder;
        // root->used = false; // 原始初始化失败

        // freeLists[levels - 1] = new LinkedList<BlockHead>();
        // freeLists[levels - 1]->push(root);
        // bitmaps[levels - 1] = Bitmap(poolSize >> minOrder);
        // bitmaps[levels - 1].set(0, true);

        // // 错误代码：运行时崩溃 -1073741819 (0xC0000005)
        // for (int i = 0; i < levels - 1; i++) {
        //     freeLists[i] = new LinkedList<BlockHead>();
        //     bitmaps[i] = Bitmap(poolSize >> (minOrder + i));
        // }
    }

    ~VPool() {
        for (int i = 0; i < levels; ++i) {
            delete freeLists[i];
            delete bitmaps[i];
        }
        delete[] freeLists;
        delete[] bitmaps;
        ::free(base);             // 逆天：原本局部变量 free(del) 会遮蔽全局 free
    }

    void* allocate(size_t size) {
        size += sizeof(BlockHead);
        int tarlvl = forSize(size);
        size_t blockSize = size_t(1) << (minOrder + tarlvl);

        int i = tarlvl;
        while (i < levels && freeLists[i]->empty()) {
            i++;
        }
        if (i == levels) return nullptr;

        while (i > tarlvl) {
            BlockHead* blk = freeLists[i]->pop();
            int k = minOrder + i - 1;
            size_t half = size_t(1) << k;
            BlockHead* mate = (BlockHead*)((char*)blk + half);

            mate->k = k;
            mate->used = false;
            freeLists[i - 1]->push(mate);
            bitmaps[i - 1]->set(indexOf(mate, i - 1), true);

            blk->k = k;
            blk->used = false;
            freeLists[i - 1]->push(blk);
            bitmaps[i - 1]->set(indexOf(blk, i - 1), true);
            i--;
        }

        BlockHead* blk = freeLists[tarlvl]->pop();
        bitmaps[tarlvl]->set(indexOf(blk, tarlvl), false);
        blk->used = true;
        return (char*)blk + sizeof(BlockHead);
    }

    void del(void* p) {
        if (!p)
            return;
        // 删除指针时还原为 BlockHead 指针
        BlockHead* blk = (BlockHead*)((char*)p - sizeof(BlockHead));
        int lvl = blk->k - minOrder;
        blk->used = false;
        freeLists[lvl]->push(blk);
        bitmaps[lvl]->set(indexOf(blk, lvl), true);
        merge(blk);
    }
};

int main() {
    cout << "\033[32m---CooCG   <(^_^<)----\033[0m" << endl;
    // cout << "---CooCG   <(^_^<)----" << endl;
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
