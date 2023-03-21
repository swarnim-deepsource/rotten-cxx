// scatr-check: CXX-A1002
int foo(char* user_input, int value) {
    // [CXX-A1002]
    printf(user_input, value); // format string is untrusted
    // Try to avoid such cases or use custom functions that validate and escape 
    // possible issues with the input.
}
