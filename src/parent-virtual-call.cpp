// RUN: %check_clang_tidy %s bugprone-parent-virtual-call %t

extern int foo();

class A {
public:
  A() = default;
  virtual ~A() = default;

  virtual int virt_1() { return foo() + 1; }
  virtual int virt_2() { return foo() + 2; }

  int non_virt() { return foo() + 3; }
  static int stat() { return foo() + 4; }
};

class B : public A {
public:
  B() = default;

  // Nothing to fix: calls to direct parent.
  int virt_1() override { return A::virt_1() + 3; }
  int virt_2() override { return A::virt_2() + 4; }
};

class C : public B {
public:
  // [CXX-W2008]: 34 "Grand-parent method `A::virt_1(..)` called; Overridden
  // method found in parent record/s: `B`"
  int virt_1() override { return A::virt_1() + B::virt_1(); }
  // [CXX-W2008]: 34 "Grand-parent method `A::virt_1(..)` called; Overridden
  // method found in parent record/s: `B`"
  int virt_2() override { return A::virt_1() + B::virt_1(); }

  // Test that non-virtual and static methods are not affected by this cherker.
  int method_c() { return A::stat() + A::non_virt(); }
};

// Check aliased type names
using A1 = A;
typedef A A2;
#define A3 A

class C2 : public B {
public:
  // [CXX-W2008]: 34 "Grand-parent method `A::virt_1(..)` called; Overridden
  // method found in parent record/s: `B`" [CXX-W2008]: 49 "Grand-parent method
  // `A::virt_2(..)` called; Overridden method found in parent record/s: `B`"
  int virt_1() override { return A1::virt_1() + A2::virt_1() + A3::virt_1(); }
  // CHECK-FIXES:  int virt_1() override { return B::virt_1() + B::virt_1() +
  // B::virt_1(); }
};

// Test that the check affects grand-grand..-parent calls too.
class D : public C {
public:
  // [CXX-W2008]: 34 "Grand-parent method `A::virt_1(..)` called; Overridden
  // method found in parent record/s: `C`" [CXX-W2008]: 48 "Grand-parent method
  // `B::virt_2(..)` called; Overridden method found in parent record/s: `C`"
  int virt_1() override { return A::virt_1() + B::virt_1() + D::virt_1(); }

  // [CXX-W2008]: 34 "Grand-parent method `A::virt_1(..)` called; Overridden
  // method found in parent record/s: `C`" [CXX-W2008]: 48 "Grand-parent method
  // `B::virt_2(..)` called; Overridden method found in parent record/s: `C`"
  int virt_2() override { return A::virt_1() + B::virt_1() + D::virt_1(); }
};

// Test classes in namespaces.
namespace {
class BN : public A {
public:
  int virt_1() override { return A::virt_1() + 3; }
  int virt_2() override { return A::virt_2() + 4; }
};
} // namespace

namespace N1 {
class A {
public:
  A() = default;
  virtual int virt_1() { return foo() + 1; }
  virtual int virt_2() { return foo() + 2; }
};
} // namespace N1

namespace N2 {
class BN : public N1::A {
public:
  int virt_1() override { return A::virt_1() + 3; }
  int virt_2() override { return A::virt_2() + 4; }
};
} // namespace N2

class CN : public BN {
public:
  // [CXX-W2008]: 34 "Grand-parent method `A::virt_1(..)` called; Overridden
  // method found in parent record/s: `BN`"
  int virt_1() override { return A::virt_1() + BN::virt_1(); }
  // [CXX-W2008]: 34 "Grand-parent method `A::virt_2(..)` called; Overridden
  // method found in parent record/s: `BN`"
  int virt_2() override { return A::virt_1() + BN::virt_1(); }
};

class CNN : public N2::BN {
public:
  // [CXX-W2008]: 34 "Grand-parent method `N1::A::virt_1(..)` called; Overridden
  // method found in parent record/s: `N2::BN`"
  int virt_1() override { return N1::A::virt_1() + N2::BN::virt_1(); }
  // [CXX-W2008]: 34 "Grand-parent method `N1::A::virt_1(..)` called; Overridden
  // method found in parent record/s: `N2::BN`"
  int virt_2() override { return N1::A::virt_1() + N2::BN::virt_1(); }
};

// Test multiple inheritance fixes
class AA {
public:
  AA() = default;
  virtual ~AA() = default;

  virtual int virt_1() { return foo() + 1; }
  virtual int virt_2() { return foo() + 2; }

  int non_virt() { return foo() + 3; }
  static int stat() { return foo() + 4; }
};

class BB_1 : virtual public AA {
public:
  BB_1() = default;

  // Nothing to fix: calls to parent.
  int virt_1() override { return AA::virt_1() + 3; }
  int virt_2() override { return AA::virt_2() + 4; }
};

class BB_2 : virtual public AA {
public:
  BB_2() = default;
  int virt_1() override { return AA::virt_1() + 3; }
};

class CC : public BB_1, public BB_2 {
public:
  // [CXX-W2008]: 34 "Grand-parent method `AA::virt_1(..)` called; Overridden
  // method found in parent record/s: `BB_1(..)`, `BB_2(..)`"
  int virt_1() override { return AA::virt_1() + 3; }
};

// Test that virtual method is not diagnosed as not overridden in parent.
class BI : public A {
public:
  BI() = default;
};

class CI : BI {
  int virt_1() override { return A::virt_1(); }
};

// Test templated classes.
template <class F> class BF : public A {
public:
  int virt_1() override { return A::virt_1() + 3; }
};

// Test templated parent class.
class CF : public BF<int> {
public:
  // [CXX-W2008]: 34 "Grand-parent method `A::virt_1(..)` called; Overridden
  // method found in parent record/s: `BF`"
  int virt_1() override { return A::virt_1(); }
};

// Test both templated class and its parent class.
template <class F> class DF : public BF<F> {
public:
  DF() = default;
  // [CXX-W2008]: 34 "Grand-parent method `A::virt_1(..)` called; Overridden
  // method found in parent record/s: `BF`"
  int virt_1() override { return A::virt_1(); }
};

// Just to instantiate DF<F>.
int bar() { return (new DF<int>())->virt_1(); }
