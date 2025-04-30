#include <iostream>
#include "VPool.h"
#include "LinkedList.h"
#include "BlockHead.h"
#include "Bitmap.h"
#include "HPoint.h"
#include "Object.h"

using namespace std;

int main() {
    cout << "\033[32m---CooCG   <(^_^<)----\033[0m" << endl;
    cout << "=========================" << endl;

    {
        HPoint<Object> a(new Object());
        HPoint<Object> b = a;
        a.reset();
        b = HPoint<Object>(new Object());

        HPoint<Object> c;
        c = b;
        c = c;

        HPoint<Object> d(std::move(b));

        auto weak = HPoint<Object>::weakRef(c.get());
    }

    return 0;
}
