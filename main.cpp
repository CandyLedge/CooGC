#include <iostream>
#include "linklist.cpp"
#include "bitmap.cpp"
using namespace std;
static int maxOrder=5;
static int minOrder=1;
static int poolSize=1<<maxOrder;
static int levels=maxOrder-minOrder+1;

struct BlockHead {
    int k;
    bool used;
};

class VPool {
    void* base;
    LinkedList<BlockHead>** freeLists;
    Bitmap* bitmaps;

    //����С��
    int forSize(size_t s) {
        int lvl=0;
        size_t size=size_t(1)<<minOrder;
        for(;size<s&& lvl<levels-1;lvl++)
            size<<=1;
        return lvl;
    }

    size_t indexOf(BlockHead* blk,int lvl) {
        size_t off=(char*)blk-(char*)base;//������ʼ��ַ��ƫ����
        return off>>(minOrder+lvl);
    }

    void merge(BlockHead* blk) {
        //һֱ�ϲ�������Ϊֹ
        while(blk->k<maxOrder) {
            size_t size=size_t(1)<<blk->k;
            size_t off=(char*)blk-(char*)base;
            size_t mateOff=off^size;

            BlockHead* mate=(BlockHead*)((char*)base+mateOff);

            if(!mate->used&& mate->k==blk->k) {
                int lvl=blk->k-minOrder;
                freeLists[lvl]->remove(mate);
                bitmaps[lvl].set(indexOf(mate,lvl),0);
                if(blk>mate) blk=mate;
                blk->k++;
                continue;//��������Ͳ���������Ϻϲ���
            }
            break;
        }
        int lvl=blk->k-minOrder;
        freeLists[lvl]->remove(blk);
        freeLists[lvl]->push(blk);
        bitmaps[lvl].set(indexOf(blk,lvl),1);
    }
public:
    // void* base;
    // LinkedList<BlockHead> freeLists[levels];
    // Bitmap bitmaps[levels];
    // int k;
    // bool used;
    VPool(){
        freeLists = new LinkedList<BlockHead>*[levels];
        bitmaps = new Bitmap[levels];
        base=malloc(poolSize);
        BlockHead* root=(BlockHead*)base;
        root->k=maxOrder;
        root->used=false;//Ī����
        // freeLists[levels-1]->push(root);
        // bitmaps[levels-1]=Bitmap(poolSize>>minOrder);
        // bitmaps[levels-1].set(0,true);
        // for(int i=0;i<levels-1;i++) {
        //     bitmaps[i]=Bitmap(poolSize>>(minOrder+i));
        // }
        freeLists[levels - 1] = new LinkedList<BlockHead>();
        freeLists[levels - 1]->push(root);
        bitmaps[levels - 1] = Bitmap(poolSize >> minOrder);
        bitmaps[levels - 1].set(0, true);

        //�����ѽ������˳�����Ϊ -1073741819 (0xC0000005)
        for (int i = 0; i < levels - 1; i++) {
            freeLists[i] = new LinkedList<BlockHead>();
            bitmaps[i] = Bitmap(poolSize >> (minOrder + i));
        }
    }

    void* allocate(size_t size) {
        size+=sizeof(BlockHead);
        int tarlvl=forSize(size);
        int i=tarlvl;
        while(i<levels&&freeLists[i]->empty()) {
            i++;
        }
        if(i==levels) return nullptr;

        while(i>tarlvl) {
            BlockHead* blk=freeLists[i]->pop();
            bitmaps[i].set(indexOf(blk,i),false);
            int k=minOrder+1-1;
            size_t half=size_t(1)<<k;
            BlockHead* mate=(BlockHead*)((char*)base+half);

            mate->k=k;
            mate->used=false;
            freeLists[i-1]->push(mate);
            bitmaps[i-1].set(indexOf(mate,i-1),true);

            blk->k=k;
            blk->used=false;
            freeLists[i-1]->push(blk);
            bitmaps[i-1].set(indexOf(blk,i-1),true);
            i--;
        }
        BlockHead* blk=freeLists[tarlvl]->pop();
        bitmaps[tarlvl].set(indexOf(blk,tarlvl),false);
        blk->used=true;
        return (char*)blk+sizeof(BlockHead);
    }
    void free(void *p) {
        if(!p)
            return;
        //��ÿ�ͷ
        BlockHead* blk=(BlockHead*)(char*)p-sizeof(BlockHead);
        int lvl=blk->k-minOrder;
        blk->used=false;
        freeLists[lvl]->push(blk);
        bitmaps[lvl].set(indexOf(blk,lvl),true);
        merge(blk);
    }
};


int main() {
    // cout << "\033[32m---CooCG   <(^_^<)----\033[0m" << endl;
    cout << "---CooCG   <(^_^<)----" << endl;
    cout<<"========================="<<endl;
    VPool pool;

    // ���Է�����ͷ�
    void* ptr1 = pool.allocate(64);
    cout << "Allocated 64 bytes\n";
    void* ptr2 = pool.allocate(128);
    cout << "Allocated 128 bytes\n";

    pool.free(ptr1);
    cout << "Deallocated 64 bytes\n";
    pool.free(ptr2);
    cout << "Deallocated 128 bytes\n";

    return 0;
}