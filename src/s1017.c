// scatr-check: CXX-S1017
int get_index(void) {/**/}
int main(int argc, char* argv[]) {
    int i = get_index();
    if (i > 0) {
        int argn = argv[i - 10]; // -BORKED- "CXX-S1017"
        // seems to be weirdly broken in various cases randomly
        // considering forking it.
        // broken in the same way as "S1015"
    }
}
