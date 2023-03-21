// scatr-check: CXX-S2004
int main (int argc, char *argv[]) {
    char* tt = "this is a long one I think";
    char dst[10];
    // [CXX-S1004]
    scanf("%s", dst);
}
