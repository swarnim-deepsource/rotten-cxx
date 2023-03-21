// scatr-check: CXX-S1014

void foo() {
    int i[2] = {10, 20};
    // The following code will not work on platforms
    //  where width of `int` may not be `4` `char`(s)
    // [CXX-S1014] : Risky cast after possibly misaligned pointer offset
    int j = *(int*)((char*)i + 4);
}
