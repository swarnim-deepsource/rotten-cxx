#include "stdio.h"

int main() {
    int arr[10] = {};
    int var = (int)getc(stdin); // -ARCHIVED- "CXX-S1001"
    arr[var] = "1"; // very risky security vulnerability, as it may
    // Allow for reading or writing to unintended parts of the memory.
}
