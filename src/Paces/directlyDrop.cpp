// 给掉落物用的“直接往下掉”
#if !defined(DIRECTLYDROP)
#define DIRECTLYDROP
#include "paceHead.h"

point directlyDrop(Uint64 x) {
    point pos({0.,0.});
    int k;
    int m;
    if (x <= m) {
        pos.y = k * (x - m) * x;
    }
    else {
        pos.y = k * (x - m);
    }
}

#endif // DIRECTLYDROP
