// scatr-check: CXX-W2010,CXX-W2012
// RUN: %check_clang_tidy %s cppcoreguidelines-avoid-const-or-ref-data-members %t
namespace std {
template <typename T> struct unique_ptr {};

template <typename T> struct shared_ptr {};
} // namespace std

namespace gsl {
template <typename T> struct not_null {};
} // namespace gsl

struct Ok {
    int i;
    int* p;
    const int* pc;
    std::unique_ptr<int> up;
    std::shared_ptr<int> sp;
    gsl::not_null<int> n;
};

struct ConstMember {
    // [CXX-W2010]: 5 "Member `ConstMember::c` is const qualified"
    const int c;
};

struct LvalueRefMember {
    // [CXX-W2012]: 5 "Member `LvalueRefMember::lr` is a reference"
    int& lr;
};

struct ConstRefMember {
    // [CXX-W2012]
    const int& cr;
};

struct RvalueRefMember {
    // [CXX-W2012]
    int&& rr;
};

struct ConstAndRefMembers {
    // [CXX-W2010]
    const int c;
    // [CXX-W2012]
    int& lr;
    // [CXX-W2012]
    const int& cr;
    // [CXX-W2012]: 5 "Member `ConstAndRefMembers::rr` is a reference"
    int&& rr;
};

struct Foo {};

struct Ok2 {
    Foo i;
    Foo* p;
    const Foo* pc;
    std::unique_ptr<Foo> up;
    std::shared_ptr<Foo> sp;
    gsl::not_null<Foo> n;
};

struct ConstMember2 {
    // [CXX-W2010]
    const Foo c;
};

struct LvalueRefMember2 {
    // [CXX-W2012]
    Foo& lr;
};

struct ConstRefMember2 {
    // [CXX-W2012]
    const Foo& cr;
};

struct RvalueRefMember2 {
    // [CXX-W2012]
    Foo&& rr;
};

struct ConstAndRefMembers2 {
    // [CXX-W2010]
    const Foo c;
    // [CXX-W2012]
    Foo& lr;
    // [CXX-W2012]
    const Foo& cr;
    // [CXX-W2012]
    Foo&& rr;
};

using ConstType = const int;
using RefType = int&;
using ConstRefType = const int&;
using RefRefType = int&&;

struct WithAlias {
    // [CXX-W2010]
    ConstType c;
    // [CXX-W2012]
    RefType lr;
    // [CXX-W2012]
    ConstRefType cr;
    // [CXX-W2012]
    RefRefType rr;
};

template <int N> using Array = int[N];

struct ConstArrayMember {
    // [CXX-W2010]
    const Array<1> c;
};

struct LvalueRefArrayMember {
    // [CXX-W2012]
    Array<2>& lr;
};

struct ConstLvalueRefArrayMember {
    // [CXX-W2012]
    const Array<3>& cr;
};

struct RvalueRefArrayMember {
    // [CXX-W2012]
    Array<4>&& rr;
};

template <typename T> struct TemplatedOk {
    T t;
};

template <typename T> struct TemplatedConst {
    // [CXX-W2010]
    T t;
};

template <typename T> struct TemplatedConstRef {
    // [CXX-W2012]
    T t;
};

template <typename T> struct TemplatedRefRef {
    // [CXX-W2012]
    T t;
};

template <typename T> struct TemplatedRef {
    // [CXX-W2012]
    T t;
};
// [CXX-W2009]: 1 "Non-const variable `t1` placed in global scope"
TemplatedOk<int> t1{};
// [CXX-W2009]: 1 "Non-const variable `t2` placed in global scope"
TemplatedConst<const int> t2{123};
// [CXX-W2009]: 1 "Non-const variable `t3` placed in global scope"
TemplatedConstRef<const int&> t3{123};
// [CXX-W2009]: 1 "Non-const variable `t4` placed in global scope"
TemplatedRefRef<int&&> t4{123};
// [CXX-W2009]: 1 "Non-const variable `t5` placed in global scope"
TemplatedRef<int&> t5{t1.t};

// Lambdas capturing const or ref members should not trigger warnings
void lambdas() {
    int x1{123};
    const int x2{123};
    const int& x3{123};
    int&& x4{123};
    int& x5{x1};

    auto v1 = [x1] {};
    auto v2 = [x2] {};
    auto v3 = [x3] {};
    auto v4 = [x4] {};
    auto v5 = [x5] {};

    auto r1 = [&x1] {};
    auto r2 = [&x2] {};
    auto r3 = [&x3] {};
    auto r4 = [&x4] {};
    auto r5 = [&x5] {};

    auto iv = [=] {
        auto c1 = x1;
        auto c2 = x2;
        auto c3 = x3;
        auto c4 = x4;
        auto c5 = x5;
    };

    auto ir = [&] {
        auto c1 = x1;
        auto c2 = x2;
        auto c3 = x3;
        auto c4 = x4;
        auto c5 = x5;
    };
}
