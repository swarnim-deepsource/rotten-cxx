// scatr-check: CXX-W2016
// RUN: %check_clang_tidy %s cert-msc50-cpp %t

int rand();
int rand(int);

namespace std {
using ::rand;
}

namespace nonstd {
  int rand();
}

void testFunction1() {
  // [CXX-W2016]: "Found using function `rand` which has limited randomness"
  int i = std::rand();

  // [CXX-W2016]: "Found using function `rand` which has limited randomness"
  int j = ::rand();

  int k = rand(i);

  int l = nonstd::rand();

  // [CXX-W2016]: "Found using function `rand` which has limited randomness"
  int m = rand();
}

