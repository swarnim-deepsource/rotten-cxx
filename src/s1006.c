// scatr-check: CXX-S1006
int main() {
    int* ints = (int*)malloc(64); // [CXX-S1006]
    malloc_init(); // suppressed
    int* ptr = malloc_workaround_ints(64); // suppressed
    int len = 32;
    char* src = malloc(len);
}
