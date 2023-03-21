// scatr-check: CXX-S1016
int test() {
    char dest[32] = {};
    char* src = "this is probably a longer string that you expected to see?";
    strncpy(dest, src, sizeof(src)); // [CXX-S1016]
}
