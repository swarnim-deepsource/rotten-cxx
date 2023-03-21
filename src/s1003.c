// scatr-check: CXX-S1003
int main (int argc, char *argv[]) {
    char* tt = "this is a long one I think";
    char dst[10];
    // [CXX-S1003]
    strcpy(dst, tt);
}
