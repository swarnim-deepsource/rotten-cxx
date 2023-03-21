// scatr-check: CXX-W2029

// [CXX-W2029]
typedef int Type;
// [CXX-W2029]
typedef long LL;
// [CXX-W2029]
typedef int Bla;
// [CXX-W2029]
typedef Bla Bla2;
// [CXX-W2029]
typedef void (*type)(int, int);
// [CXX-W2029]
typedef void (*type2)();

class Class {
// [CXX-W2029]
  typedef long long Type;
    };
// [CXX-W2029]
typedef void (Class::*MyPtrType)(Bla) const;

class Iterable {
public:
  class Iterator {};
};

template <typename T>
class Test {
// [CXX-W2029]
  typedef typename T::iterator Iter;
    };

using balba = long long;

union A {};
// [CXX-W2029]
typedef void (A::*PtrType)(int, int) const;
// [CXX-W2029]
typedef Class some_class;
// [CXX-W2029]
typedef Class Cclass;
// [CXX-W2029]
typedef Cclass cclass2;

class cclass {};
// [CXX-W2029]
typedef void (cclass::*MyPtrType3)(Bla);

using my_class = int;
// [CXX-W2029]
typedef Test<my_class *> another;
// [CXX-W2029]
typedef int* PInt;
// [CXX-W2029]
typedef int bla1, bla2, bla3;

#define CODE typedef int INT
CODE;

struct Foo;
#define Bar Baz
// [CXX-W2029]
typedef Foo Bar;

#define TYPEDEF typedef
TYPEDEF Foo Bak;

#define FOO Foo
// [CXX-W2029]
typedef FOO Bam;
// [CXX-W2029]
typedef struct Foo Bap;
// [CXX-W2029]
struct Foo typedef Bap2;
// [CXX-W2029]
Foo typedef Bap3;
// [CXX-W2029]
typedef struct Unknown Baq;
// [CXX-W2029]
struct Unknown2 typedef Baw;
// [CXX-W2029]
int typedef Bax;
// [CXX-W2029]
typedef struct Q1 { int a; } S1;
// [CXX-W2029]
typedef struct { int b; } S2;
// [CXX-W2029]
struct Q2 { int c; } typedef S3;
// [CXX-W2029]
struct { int d; } typedef S4;

namespace my_space {
  class my_cclass {};
// [CXX-W2029]
  typedef my_cclass FuncType;
}

#define lol 4
// [CXX-W2029]
typedef unsigned Map[lol];
// [CXX-W2029]
typedef void (*fun_type)();

namespace template_instantiations {
template <typename T>
class C {
 protected:
// [CXX-W2029]
  typedef C<T> super;
      virtual void f();

public:
  virtual ~C();
};

class D : public C<D> {
  void f() override { super::f(); }
};
class E : public C<E> {
  void f() override { super::f(); }
};
}

template <typename T1, typename T2>
class TwoArgTemplate {
// [CXX-W2029]
  typedef TwoArgTemplate<T1, T2> self;
    };

template <bool B, typename T>
struct S {};
// [CXX-W2029]
typedef S<(0 > 0), int> S_t, *S_p;
// [CXX-W2029]
typedef S<(0 < 0), int> S2_t, *S2_p;
// [CXX-W2029]
typedef S<(0 > 0 && (3 > 1) && (1 < 1)), int> S3_t;

template <bool B>
struct Q {};

constexpr bool b[1] = {true};
// [CXX-W2029]
typedef Q<b[0 < 0]> Q_t, *Q_p;
// [CXX-W2029]
typedef Q<b[0 < 0]> Q2_t;

struct T {
  constexpr T(bool) {}

  static constexpr bool b = true;
};
// [CXX-W2029]
typedef Q<T{0 < 0}.b> Q3_t, *Q3_p;
// [CXX-W2029]
typedef Q<T{0 < 0}.b> Q3_t;
// [CXX-W2029]
typedef TwoArgTemplate<TwoArgTemplate<int, Q<T{0 < 0}.b> >, S<(0 < 0), Q<b[0 < 0]> > > Nested_t;

template <typename a>
class TemplateKeyword {
// [CXX-W2029]
  typedef typename a::template b<> d;
    // [CXX-W2029]
  typedef typename a::template b<>::c d2;
    };

template <typename... Args>
class Variadic {};
// [CXX-W2029]
typedef Variadic<Variadic<int, bool, Q<T{0 < 0}.b> >, S<(0 < 0), Variadic<Q<b[0 < 0]> > > > Variadic_t;
// [CXX-W2029]
typedef Variadic<Variadic<int, bool, Q<T{0 < 0}.b> >, S<(0 < 0), Variadic<Q<b[0 < 0]> > > > Variadic_t, *Variadic_p;
// [CXX-W2029]
typedef struct { int a; } R_t, *R_p;
// [CXX-W2029]
typedef enum { ea1, eb1 } EnumT1;

#include "modernize-use-using.h"
// [CXX-W2029]
typedef enum { ea2, eb2 } EnumT2_CheckTypedefImpactFromAnotherFile;

template <int A>
struct InjectedClassName {
// [CXX-W2029]
  typedef InjectedClassName b;
    };

template <int>
struct InjectedClassNameWithUnnamedArgument {
// [CXX-W2029]
  typedef InjectedClassNameWithUnnamedArgument b;
    };
// [CXX-W2029]
typedef struct { int a; union { int b; }; } PR50990;
// [CXX-W2029]
typedef struct { struct { int a; struct { struct { int b; } c; int d; } e; } f; int g; } PR50990_nested;
// [CXX-W2029]
typedef struct { struct { int a; } b; union { int c; float d; struct { int e; }; }; struct { double f; } g; } PR50990_siblings;