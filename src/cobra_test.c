// scatr-check: CXX-S1004, CXX-S1003
#include "stdio.h"

void fn(int r) {
    printf("%d", *&r);
}

int main (int argc, char *argv[]) {
    char* tt = "this is a long one I think";
    char dst[10];
    // [CXX-S1003]
    strcpy(dst, tt);
    // [CXX-S1004]
    scanf("%s", dst);
}

#define ngx_regex_direct_alloc 0

void __attribute__((always_inline)) ngx_regex_free(void *p, void *data) {
    if (ngx_regex_direct_alloc) {
        ngx_free(p);
    }
}

void function_name_as_handler() {
    void* handler = ngx_regex_free;
}
