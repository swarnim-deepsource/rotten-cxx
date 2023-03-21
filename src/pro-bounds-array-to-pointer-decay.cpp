// RUN: %check_clang_tidy %s cppcoreguidelines-pro-bounds-array-to-pointer-decay %t
#include <stddef.h>

namespace gsl {
template <class T>
class array_view {
public:
  template <class U, size_t N>
  array_view(U (&arr)[N]);
};
}

void pointerfun(int *p);
void arrayfun(int p[]);
void arrayviewfun(gsl::array_view<int> &p);
size_t s();

void f() {
  int a[5];
  // [CXX-C1000]: 14 "Array is implicitly decaying into a pointer"
  pointerfun(a);
  pointerfun((int *)a); // OK, explicit cast
  // [CXX-C1000]: 12 "Array is implicitly decaying into a pointer"
  arrayfun(a);

  // [CXX-C1000]: 14 "Array is implicitly decaying into a pointer"
  pointerfun(a + s() - 10); // Convert to &a[g() - 10];

  gsl::array_view<int> av(a);
  arrayviewfun(av); // OK

  int i = a[0];      // OK
  int j = a[(1 + 2)];// OK
  pointerfun(&a[0]); // OK

  for (auto &e : a) // OK, iteration internally decays array to pointer
    e = 1;
}

const char *g() {
  return "clang"; // OK, decay string literal to pointer
}
const char *g2() {
    return ("clang"); // OK, ParenExpr hides the literal-pointer decay
}

void f2(void *const *);
void bug25362() {
  void *a[2];
  f2(static_cast<void *const*>(a)); // OK, explicit cast
}

void issue31155(int i) {
  const char *a = i ? "foo" : "bar";    // OK, decay string literal to pointer
  const char *b = i ? "foo" : "foobar"; // OK, decay string literal to pointer

  char arr[1];
  // [CXX-C1000]: 23 "Array is implicitly decaying into a pointer"
  const char *c = i ? arr : "bar";
  // [CXX-C1000]: 31 "Array is implicitly decaying into a pointer"
  const char *d = i ? "foo" : arr;
}
