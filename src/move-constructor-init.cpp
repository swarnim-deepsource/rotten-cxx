// scatr-check: CXX-P2011
// RUN: %check_clang_tidy %s performance-move-constructor-init,modernize-pass-by-value %t -- \
// RUN: -config='{CheckOptions: \
// RUN:  [{key: modernize-pass-by-value.ValuesOnly, value: true}]}' \
// RUN: -- -isystem %clang_tidy_headers

// CHECK-FIXES: #include <utility>
template <class T> struct remove_reference      {typedef T type;};
template <class T> struct remove_reference<T&>  {typedef T type;};
template <class T> struct remove_reference<T&&> {typedef T type;};

template <typename T>
typename remove_reference<T>::type&& move(T&& arg) {
  return static_cast<typename remove_reference<T>::type&&>(arg);
}

struct C {
  C() = default;
  C(const C&) = default;
};

struct B {
  B() {}
  B(const B&) {}
  // [CXX-P2008]
  B(B &&) {}
};

struct D : B {
  D() : B() {}
  D(const D &RHS) : B(RHS) {}
  // [CXX-P2008]
  // [CXX-P2011]: "Copy constructor of `B` is used to initialize `RHS` in move constructor"
  D(D &&RHS) : B(RHS) {}
};

struct E : B {
  E() : B() {}
  E(const E &RHS) : B(RHS) {}
  // [CXX-P2008]
  E(E &&RHS) : B(move(RHS)) {} // ok
};

struct F {
  C M;

  // [CXX-P2008]
  F(F &&) : M(C()) {} // ok
};

struct G {
  G() = default;
  G(const G&) = default;
  G(G&&) = delete;
};

struct H : G {
  H() = default;
  H(const H&) = default;
  // [CXX-P2008]
  H(H &&RHS) : G(RHS) {} // ok
};

struct I {
  I(const I &) = default; // suppresses move constructor creation
};

struct J : I {
  // [CXX-P2008]
  J(J &&RHS) : I(RHS) {} // ok
};

struct K {}; // Has implicit copy and move constructors, is trivially copyable
struct L : K {
  // [CXX-P2008]
  L(L &&RHS) : K(RHS) {} // ok
};

struct M {
  B Mem;
  // [CXX-P2008]
  // [CXX-P2011]: "Copy constructor of `B` is used to initialize `RHS.Mem` in move constructor"
  M(M &&RHS) : Mem(RHS.Mem) {}
};

struct N {
  B Mem;
  // [CXX-P2008]
  N(N &&RHS) : Mem(move(RHS.Mem)) {}
};

struct O {
  // [CXX-P2008]
  O(O&& other) : b(other.b) {} // ok
  // [CXX-W2010]
  const B b;
};

struct P {
  P(O&& other) : b(other.b) {} // ok
  B b;
};

struct Movable {
  Movable(Movable &&) = default;
  Movable(const Movable &) = default;
  Movable &operator=(const Movable &) = default;
  ~Movable() {}
};

struct TriviallyCopyable {
  TriviallyCopyable() = default;
  TriviallyCopyable(TriviallyCopyable &&) = default;
  TriviallyCopyable(const TriviallyCopyable &) = default;
};

struct Positive {
  // [CXX-P2009]
  Positive(Movable M) : M_(M) {}
  Movable M_;
};

struct NegativeMultipleInitializerReferences {
  // [CXX-P2009]
  NegativeMultipleInitializerReferences(Movable M) : M_(M), n_(M) {}
  Movable M_;
  Movable n_;
};

struct NegativeReferencedInConstructorBody {
  // [CXX-P2009]
  NegativeReferencedInConstructorBody(Movable M) : M_(M) { M_ = M; }
  Movable M_;
};

struct NegativeParamTriviallyCopyable {
  NegativeParamTriviallyCopyable(TriviallyCopyable T) : T_(T) {}
  NegativeParamTriviallyCopyable(int I) : I_(I) {}

  TriviallyCopyable T_;
  int I_;
};

struct NegativeNotPassedByValue {
  // This const ref constructor isn't warned about because the ValuesOnly option is set.
  NegativeNotPassedByValue(const Movable &M) : M_(M) {}
  // [CXX-P2009]
  NegativeNotPassedByValue(const Movable M) : M_(M) {}
  NegativeNotPassedByValue(Movable &M) : M_(M) {}
  NegativeNotPassedByValue(Movable *M) : M_(*M) {}
  NegativeNotPassedByValue(const Movable *M) : M_(*M) {}
  Movable M_;
};

struct Immovable {
  Immovable(const Immovable &) = default;
  Immovable(Immovable &&) = delete;
};

struct NegativeImmovableParameter {
  NegativeImmovableParameter(Immovable I) : I_(I) {}
  Immovable I_;
};
