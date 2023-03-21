// scatr-check: CXX-P2008
// RUN: %check_clang_tidy %s performance-noexcept-move-constructor %t

class A {
  // [CXX-P2008]: "Move constructor is declared without `noexcept`."
  A(A &&);
  // [CXX-P2008]: "Move-assignment constructor is declared without `noexcept`."
  A &operator=(A &&);
};

struct B {
  static constexpr bool kFalse = false;
  // [CXX-P2008]: "Move constructor's `noexcept` expression evaluates to false."
  B(B &&) noexcept(kFalse);
};

#define NULL 0

struct C {
  // [CXX-P2008]: "Move constructor's `noexcept` expression evaluates to false."
  C(C &&) noexcept(NULL);
};

struct D {
  static constexpr int kFalse = 0;
  // [CXX-P2008]: "Move constructor's `noexcept` expression evaluates to false."
  D(D &&) noexcept(kFalse);
};

class OK {};

void f() {
  OK a;
  a = OK();
}

class OK1 {
public:
  OK1();
  OK1(const OK1 &);
  OK1(OK1 &&) noexcept;
  OK1 &operator=(OK1 &&) noexcept;
  void f();
  void g() noexcept;
};

class OK2 {
  static constexpr bool kTrue = true;

public:
  OK2(OK2 &&) noexcept(true) {}
  OK2 &operator=(OK2 &&) noexcept(kTrue) { return *this; }
};

struct OK3 {
  OK3(OK3 &&) noexcept(false) {}
  OK3 &operator=(OK3 &&) = delete;
};

struct OK4 {
  OK4(OK4 &&) noexcept = default;
  OK4 &operator=(OK4 &&) noexcept = default;
};

struct OK5 {
  OK5(OK5 &&) noexcept(true) = default;
  OK5 &operator=(OK5 &&) noexcept(true) = default;
};
