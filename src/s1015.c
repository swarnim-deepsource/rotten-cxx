void foo(int size) {
    void* mem = malloc(size); // can be -ve
}

void bar(int size) { // `size` possibly -ve
    char* bcc_ptr = 0;
    char* sess = "as";
    if (size > 0) {
        strncpy(sess, bcc_ptr, size); // -BORKED- "CXX-S1015"
        // seems to be weirdly broken in various cases randomly
        // considering forking it.
    }
}