void bar() {
    double db = 10.0123132970923;
    // -ARCHIVED- "CXX-S1011"
    float ft_c = db;           // potentially lossy assignment
    float ft_cpp = (float)db; // potentially lossy cast
}
