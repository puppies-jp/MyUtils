#include <dlfcn.h>
#include <stdio.h>

/*
dlsym
 シンボルがロードされたメモリーのアドレスという記述だとちょっとピンと来づらいですが、
 要は ld.so にロードされた関数への関数ポインタが取れるということです。
 これを使ってオリジナルの libc.so 側の printf() にアクセスすることになります。
*/

typedef int (*ftype)(const char *, ...);
// originalなprintf関数ポインタ
ftype ORIGIN_PRINT = (ftype)dlsym(RTLD_NEXT, "printf");

int printf(const char *_restrict, ...)
{
    ORIGIN_PRINT("[catch]: I`m hook library!!\n");
    return ORIGIN_PRINT(_restrict);
}

namespace
{
    struct alpha
    {
        alpha()
        {
            ORIGIN_PRINT("LOAD HOOK Library!!\n");
        }
    } alpha;
}

/*
g++ -fPIC -shared ./printhook.cpp -o hook.so -rdynamic -ldl -std=c++2a
gcc -ldl -dynamiclib printhook.cpp -o printhook.dylib
DYLD_INSERT_LIBRARIES=printf.dylib DYLD_FORCE_FLAT_NAMESPACE=YES  ./sample.out
LD_PRELOAD=./hook.so ./sample.out
*/