// RUN: %check_clang_tidy %s performance-for-range-copy %t -- --
// -fno-delayed-template-parsing

namespace std {

template <typename _Tp> struct remove_reference {
  typedef _Tp type;
};
template <typename _Tp> struct remove_reference<_Tp &> {
  typedef _Tp type;
};
template <typename _Tp> struct remove_reference<_Tp &&> {
  typedef _Tp type;
};

template <typename _Tp>
constexpr typename std::remove_reference<_Tp>::type &&move(_Tp &&__t) {
  return static_cast<typename std::remove_reference<_Tp>::type &&>(__t);
}

} // namespace std

template <typename T> struct Iterator {
  void operator++() {}
  const T &operator*() {
    // [CXX-W2011]
    static T *TT = new T();
    return *TT;
  }
  bool operator!=(const Iterator &) { return false; }
  typedef const T &const_reference;
};
template <typename T> struct View {
  View() = default;
  T begin() { return T(); }
  T begin() const { return T(); }
  T end() { return T(); }
  T end() const { return T(); }
  typedef typename T::const_reference const_reference;
};

struct ConstructorConvertible {};

struct S {
  S();
  S(const S &);
  S(const ConstructorConvertible &) {}
  ~S();
  S &operator=(const S &);
};

struct Convertible {
  operator S() const { return S(); }
};

void negativeConstReference() {
  for (const S &S1 : View<Iterator<S>>()) {
  }
}

void negativeUserDefinedConversion() {
  Convertible C[0];
  for (const S S1 : C) {
  }
}

void negativeImplicitConstructorConversion() {
  ConstructorConvertible C[0];
  for (const S S1 : C) {
  }
}

template <typename T> void uninstantiated() {
  // [CXX-P2004]: "Expensive copy to `S1` in every iteration."
  for (const S S1 : View<Iterator<S>>()) {
  }

  // Don't warn on dependent types.
  for (const T t1 : View<Iterator<T>>()) {
  }
}

template <typename T> void instantiated() {
  // [CXX-P2004]: "Expensive copy to `S2` in every iteration."
  for (const S S2 : View<Iterator<S>>()) {
  }

  // [CXX-P2004]: "Expensive copy to `T2` in every iteration."
  for (const T T2 : View<Iterator<T>>()) {
  }
}

template <typename T> void instantiatedNegativeTypedefConstReference() {
  for (typename T::const_reference T2 : T()) {
    // [CXX-P2005]: "Variable `S1`, of an expensive-type, is copy-constructer
    // but never used."
    S S1 = T2;
  }
}

void f() {
  instantiated<int>();
  instantiated<S>();
  instantiatedNegativeTypedefConstReference<View<Iterator<S>>>();
}

struct Mutable {
  Mutable() {}
  Mutable(const Mutable &) = default;
  Mutable(Mutable &&) = default;
  Mutable(const Mutable &, const Mutable &) {}
  void setBool(bool B) {}
  bool constMethod() const { return true; }
  Mutable &operator[](int I) { return *this; }
  bool operator==(const Mutable &Other) const { return true; }
  ~Mutable() {}
};

struct Point {
  ~Point() {}
  int x, y;
};

Mutable &operator<<(Mutable &Out, bool B) {
  Out.setBool(B);
  return Out;
}

bool operator!=(const Mutable &M1, const Mutable &M2) { return false; }

void use(const Mutable &M);
void use(int I);
void useTwice(const Mutable &M1, const Mutable &M2);
void useByValue(Mutable M);
void useByConstValue(const Mutable M);
void mutate(Mutable *M);
void mutate(Mutable &M);
void onceConstOnceMutated(const Mutable &M1, Mutable &M2);

void negativeVariableIsMutated() {
  for (auto M : View<Iterator<Mutable>>()) {
    mutate(M);
  }
  for (auto M : View<Iterator<Mutable>>()) {
    mutate(&M);
  }
  for (auto M : View<Iterator<Mutable>>()) {
    M.setBool(true);
  }
}

void negativeOnceConstOnceMutated() {
  for (auto M : View<Iterator<Mutable>>()) {
    onceConstOnceMutated(M, M);
  }
}

void negativeVarIsMoved() {
  for (auto M : View<Iterator<Mutable>>()) {
    auto Moved = std::move(M);
  }
}

void negativeNonConstOperatorIsInvoked() {
  for (auto NonConstOperatorInvokee : View<Iterator<Mutable>>()) {
    auto &N = NonConstOperatorInvokee[0];
  }
}

void negativeNonConstNonMemberOperatorInvoked() {
  for (auto NonConstOperatorInvokee : View<Iterator<Mutable>>()) {
    NonConstOperatorInvokee << true;
  }
}

void negativeConstCheapToCopy() {
  for (const int I : View<Iterator<int>>()) {
  }
}

void negativeConstCheapToCopyTypedef() {
  typedef const int ConstInt;
  for (ConstInt C : View<Iterator<ConstInt>>()) {
  }
}

void negativeCheapToCopy() {
  for (int I : View<Iterator<int>>()) {
    use(I);
  }
}

void negativeCheapToCopyTypedef() {
  typedef int Int;
  for (Int I : View<Iterator<Int>>()) {
    use(I);
  }
}

void positiveOnlyConstMethodInvoked() {
  // [CXX-P2004]: "Expensive copy to `M` in every iteration."
  for (auto M : View<Iterator<Mutable>>()) {
    M.constMethod();
  }
}

void positiveOnlyUsedAsConstArguments() {
  // [CXX-P2004]: "Expensive copy to `UsedAsConst` in every iteration."
  for (auto UsedAsConst : View<Iterator<Mutable>>()) {
    use(UsedAsConst);
    useTwice(UsedAsConst, UsedAsConst);
    useByValue(UsedAsConst);
    useByConstValue(UsedAsConst);
  }
}

void positiveOnlyAccessedFieldAsConst() {
  // [CXX-P2004]: "Expensive copy to `UsedAsConst` in every iteration."
  for (auto UsedAsConst : View<Iterator<Point>>()) {
    use(UsedAsConst.x);
    use(UsedAsConst.y);
  }
}

void positiveOnlyUsedInCopyConstructor() {
  // [CXX-P2004]: "Expensive copy to `A` in every iteration."
  for (auto A : View<Iterator<Mutable>>()) {
    // [CXX-P2005]: "Variable `Copy`, of an expensive-type, is copy-constructer
    // but never used."
    Mutable Copy = A;
    // [CXX-P2005]: "Variable `Copy2`, of an expensive-type, is copy-constructer
    // but never used."
    Mutable Copy2(A);
  }
}

void positiveTwoConstConstructorArgs() {
  // [CXX-P2004]: "Expensive copy to `A` in every iteration."
  for (auto A : View<Iterator<Mutable>>()) {
    Mutable Copy(A, A);
  }
}

void PositiveConstMemberOperatorInvoked() {
  // [CXX-P2004]: "Expensive copy to `ConstOperatorInvokee` in every iteration."
  for (auto ConstOperatorInvokee : View<Iterator<Mutable>>()) {
    bool result = ConstOperatorInvokee == Mutable();
  }
}

void PositiveConstNonMemberOperatorInvoked() {
  // [CXX-P2004]: "Expensive copy to `ConstOperatorInvokee` in every iteration."
  for (auto ConstOperatorInvokee : View<Iterator<Mutable>>()) {
    bool result = ConstOperatorInvokee != Mutable();
  }
}

void IgnoreLoopVariableNotUsedInLoopBody() {
  for (auto _ : View<Iterator<S>>()) {
  }
}

template <typename T> struct ValueReturningIterator {
  void operator++() {}
  T operator*() { return T(); }
  bool operator!=(const ValueReturningIterator &) { return false; }
  typedef const T &const_reference;
};

void negativeValueIterator() {
  // Check does not trigger for iterators that return elements by value.
  for (const S SS : View<ValueReturningIterator<S>>()) {
  }
}

View<Iterator<S>> createView(S) { return View<Iterator<S>>(); }

void positiveValueIteratorUsedElseWhere() {
  // [CXX-P2004]: "Expensive copy to `SS` in every iteration."
  for (const S SS : createView(*ValueReturningIterator<S>())) {
  }
}
