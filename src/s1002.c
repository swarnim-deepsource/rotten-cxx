void index(int var) {
    int arr[10] = {}; // -ARCHIVED- "CXX-S1002"
    arr[var] = "1"; // very risky security vulnerability, as it may
    // Allow for reading or writing to unintended parts of the memory.
}