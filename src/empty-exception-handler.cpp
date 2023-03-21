// scatr-check: CXX-S1021
int foo() {
    try {
        bar();
    } catch (Exception &E) {} // [CXX-S1021]
}

void reportException(Exception& E)
int negetive_test() {
    try {
         ;
     } catch (Exception& E) {
         reportException(E);
     }
}
