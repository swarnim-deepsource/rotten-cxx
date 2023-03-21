// scatr-check: CXX-W1162
#include "stdio.h"

void main() {
    if (1) {
    }
    // [CXX-W1162]: "Empty `if`/`else`/`while`/`for` statement"
    if (1)
        ;
    // -SUPPRESSED- not raised on do "CXX-W1162" "Empty `if`/`else`/`while`/`for` statement"
    do; while (0);
    if (1)
        printf("\n");
    // [CXX-W1162]: "Empty `if`/`else`/`while`/`for` statement"
    // [CXX-W1243]: "Loop body is not enclosed in `{}`"
    for (int i = 1; i < 10; i++);
    for (int u = 1; u < 10; u++) {
    }
}
