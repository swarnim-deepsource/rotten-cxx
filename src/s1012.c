// scatr-check: CXX-S1012, CXX-W2016

void foo() {
    int* mem = (int*)malloc(sizeof(int));
    *mem = 10;
    // [CXX-W2016]
    if (rand() % 10 == 0) {
        // [CXX-S1012]
        free(mem);
    }
    printf("%d", *mem); // possibly use after free
}
