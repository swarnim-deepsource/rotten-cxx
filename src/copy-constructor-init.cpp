// RUN: %check_clang_tidy %s bugprone-copy-constructor-init %t

class NonCopyable {
public:
  NonCopyable() = default;
  NonCopyable(const NonCopyable &) = delete;

private:
  int a;
};

class NonCopyable2 {
public:
  NonCopyable2() = default;

private:
  NonCopyable2(const NonCopyable2 &);
  int a;
};

class Copyable {
public:
  Copyable() = default;
  Copyable(const Copyable &) = default;

private:
  int a;
};

class Copyable2 {
public:
  Copyable2() = default;
  Copyable2(const Copyable2 &) = default;

private:
  int a;
};

class Copyable3 : public Copyable {
public:
  Copyable3() = default;
  Copyable3(const Copyable3 &) = default;
};

template <class C>
class Copyable4 {
public:
  Copyable4() = default;
  Copyable4(const Copyable4 &) = default;

private:
  int a;
};

template <class T, class S>
class Copyable5 {
public:
  Copyable5() = default;
  Copyable5(const Copyable5 &) = default;

private:
  int a;
};

class EmptyCopyable {
public:
  EmptyCopyable() = default;
  EmptyCopyable(const EmptyCopyable &) = default;
};

template <typename T>
using CopyableAlias = Copyable5<T, int>;

typedef Copyable5<int, int> CopyableAlias2;

namespace std {
  class X : public Copyable2 {
    // [CXX-W2003]: 5 "`std::X::X(..)` is calling a base constructor other than the copy constructor"
    X(const X &other) {}
  };
}

class X : public Copyable, public EmptyCopyable {
  X(const X &other) : Copyable(other) {}
};

class X2 : public Copyable2 {
  // [CXX-W2003]: 3 "`X2::X2(..)` is calling a base constructor other than the copy constructor"
  X2(const X2 &other) {}
};

class X2_A : public Copyable2 {
  // [CXX-W2003]: 3 "`X2_A::X2_A(..)` is calling a base constructor other than the copy constructor"
  X2_A(const X2_A &) {}
};

class X3 : public Copyable, public Copyable2 {
  // [CXX-W2003]: 3 "`X3::X3(..)` is calling a base constructor other than the copy constructor"
  X3(const X3 &other) : Copyable(other) {}
};

class X4 : public Copyable {
  // [CXX-W2003]: 3 "`X4::X4(..)` is calling a base constructor other than the copy constructor"
  X4(const X4 &other) : Copyable() {}
};

class X5 : public Copyable3 {
  // [CXX-W2003]: 3 "`X5::X5(..)` is calling a base constructor other than the copy constructor"
  X5(const X5 &other) {}
};

class X6 : public Copyable2, public Copyable3 {
  // [CXX-W2003]: 3 "`X6::X6(..)` is calling a base constructor other than the copy constructor"
  X6(const X6 &other) {}
};

class X7 : public Copyable, public Copyable2 {
  // [CXX-W2003]: 3 "`X7::X7(..)` is calling a base constructor other than the copy constructor"
  X7(const X7 &other) : Copyable() {}
};

class X8 : public Copyable4<int> {
  X8(const X8 &other) : Copyable4(other) {}
};

class X9 : public Copyable4<int> {
  // [CXX-W2003]: 3 "`X9::X9(..)` is calling a base constructor other than the copy constructor"
  X9(const X9 &other) : Copyable4() {}
};

class X10 : public Copyable4<int> {
  // [CXX-W2003]: 3 "`X10::X10(..)` is calling a base constructor other than the copy constructor"
  X10(const X10 &other) {}
};

class X11 : public Copyable5<int, float> {
  // [CXX-W2003]: 3 "`X11::X11(..)` is calling a base constructor other than the copy constructor"
  X11(const X11 &other) {}
};

class X12 : public CopyableAlias<float> {
  // [CXX-W2003]: 3 "`X12::X12(..)` is calling a base constructor other than the copy constructor"
  X12(const X12 &other) {}
};

template <typename T>
class X13 : T {
  X13(const X13 &other) {}
};

template class X13<EmptyCopyable>;
template class X13<Copyable>;

#define FROMMACRO                \
  class X14 : public Copyable2 { \
    X14(const X14 &other) {}     \
  };

// TODO: No support for marco reporting!
FROMMACRO

class X15 : public CopyableAlias2 {
  // [CXX-W2003]: 3 "`X15::X15(..)` is calling a base constructor other than the copy constructor"
  X15(const X15 &other) {}
};

class X16 : public NonCopyable {
  X16(const X16 &other) {}
};

class X17 : public NonCopyable2 {
  X17(const X17 &other) {}
};

class X18 : private Copyable {
  // [CXX-W2003]: 3 "`X18::X18(..)` is calling a base constructor other than the copy constructor"
  X18(const X18 &other) {}
};

class X19 : private Copyable {
  X19(const X19 &other) : Copyable(other) {}
};
