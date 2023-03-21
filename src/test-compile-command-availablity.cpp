// scatr-check: CXX-P2000, CXX-P2009
#include <unordered_set>
#include <iostream>
#include <algorithm>

void f(std::string p) { // [CXX-P2009]
    std::cout << p << std::endl;
}

void f2(std::unordered_set<int> s) {
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  std::find(s.begin(), s.end(), 46);
}
