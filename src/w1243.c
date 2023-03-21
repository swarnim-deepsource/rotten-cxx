// scatr-check: CXX-W1243
void foo() {
    // [CXX-W1243]: "Loop body is not enclosed in `{}`"
    for (int i = 1; i < 10; i++) printf("\n");

    // -SUPPRESSED- on multi line "CXX-W1243"
    for (int i = 1; i < 10; i++)
        printf("\n");
}
