// scatr-check: CXX-W2014
// cert-dcl50-cpp

// Variadic function definitions are diagnosed.
// [CXX-W2014]: "Found variadic function `f1` instead of funciton parameter packs"
void f1(int, ...) {}

// Variadic function *declarations* are not diagnosed.
void f2(int, ...); // ok

// Function parameter packs are good, however.
template <typename Arg, typename... Ts>
void f3(Arg F, Ts... Rest) {}

struct S {
  void f(int, ...); // ok
  // [CXX-W2014]: "Found variadic function `f1` instead of funciton parameter packs"
  void f1(int, ...) {}
};

// Function definitions that are extern "C" are good.
extern "C" void f4(int, ...) {} // ok
extern "C" {
  void f5(int, ...) {} // ok
}
