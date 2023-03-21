// scatr-check: CXX-S1005
int test() {
    char dest[32] = {};
    char* src = "this is probably a longer string that you expected to see?";
    strncpy(dest, src, strlen(src));
}
