// scatr-check: CXX-W2016
// RUN: %check_clang_tidy %s cert-msc30-c %t

extern int rand(void);
int nonrand(void);

int cTest(void) {
  // [CXX-W2016]: "Found using function `rand` which has limited randomness"
  int i = rand();

  int k = nonrand();

  return 0;
}
