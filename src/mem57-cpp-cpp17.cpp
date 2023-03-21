// scatr-check: CXX-W2018
// RUN: %check_clang_tidy %s -std=c++14 cert-mem57-cpp %t
// RUN: clang-tidy --extra-arg='-std=c++17' --extra-arg='-faligned-allocation' -checks='-*,cert-mem57-cpp' --extra-arg=-Wno-unused-variable --warnings-as-errors='*' %s
// RUN: clang-tidy --extra-arg='-std=c++20' --extra-arg='-faligned-allocation' -checks='-*,cert-mem57-cpp' --extra-arg=-Wno-unused-variable --warnings-as-errors='*' %s

struct alignas(128) Vector {
  char Elems[128];
};

void f() {
  // [CXX-W2018]: "Misaligned object -- operator `new` creates an alignment 16 but user-defined alignment is 128"
  auto *V1 = new Vector;
  // [CXX-W2018]: "Misaligned object -- operator `new` creates an alignment 16 but user-defined alignment is 128"
  auto *V1_Arr = new Vector[2];
}
