// scatr-check: CXX-S1009
int bar(int var) {
    int dst = 0;
    // -ARCHIVED- "CXX-S1009"
    memcpy(&dst, &var, sizeof(int)); // source is untrusted
}
