// scatr-check: CXX-W2008
// RUN: %check_clang_tidy %s performance-move-const-arg %t

namespace std {
template <typename>
struct remove_reference;

template <typename _Tp>
struct remove_reference {
  typedef _Tp type;
};

template <typename _Tp>
struct remove_reference<_Tp &> {
  typedef _Tp type;
};

template <typename _Tp>
struct remove_reference<_Tp &&> {
  typedef _Tp type;
};

template <typename _Tp>
constexpr typename std::remove_reference<_Tp>::type &&move(_Tp &&__t) {
  return static_cast<typename std::remove_reference<_Tp>::type &&>(__t);
}

template <typename _Tp>
constexpr _Tp &&
forward(typename remove_reference<_Tp>::type &__t) noexcept {
  return static_cast<_Tp &&>(__t);
}

} // namespace std

class A {
public:
  A() {}
  A(const A &rhs) {}
  // [CXX-P2008]: "Move constructor is declared without `noexcept`."
  A(A &&rhs) {}
};

struct TriviallyCopyable {
  int i;
};

void f(TriviallyCopyable) {}

void g() {
  TriviallyCopyable obj;
// [CXX-P2010]: 5 "`std::move` of trivally-copyable variable `obj` has no effect"
  f(std::move(obj));
}

int f1() {
// [CXX-P2010]: 10 "`std::move` of trivally-copyable expression has no effect"
  return std::move(42);
}

int f2(int x2) {
// [CXX-P2010]: 10 "`std::move` of trivally-copyable variable `x2` has no effect"
  return std::move(x2);
}

int *f3(int *x3) {
// [CXX-P2010]: 10 "`std::move` of trivally-copyable variable `x3` has no effect"
  return std::move(x3);
}

A f4(A x4) { return std::move(x4); }

//[CXX-P2009]: "The const qualified `x5` parameter is copied on each invocation, consider making it a const reference"
A f5(const A x5) {
// [CXX-P2010]: 10 "`std::move` of const qualified variable `x5` has no effect"
  return std::move(x5);
}

template <typename T>
T f6(const T x6) {
  return std::move(x6);
}

void f7() { int a = f6(10); }

#define M1(x) x
void f8() {
  const A a;
  M1(A b = std::move(a);)
}

#define M2(x) std::move(x)
int f9() { return M2(1); }

template <typename T>
T f10(const int x10) {
// [CXX-P2010]: 10 "`std::move` of const qualified variable `x10` has no effect"
  return std::move(x10);
}
void f11() {
  f10<int>(1);
  f10<double>(1);
}

class NoMoveSemantics {
public:
  NoMoveSemantics();
  NoMoveSemantics(const NoMoveSemantics &);

  NoMoveSemantics &operator=(const NoMoveSemantics &);
};

void callByConstRef(const NoMoveSemantics &);
void callByConstRef(int i, const NoMoveSemantics &);

void moveToConstReferencePositives() {
  NoMoveSemantics obj;

  // Basic case.
  callByConstRef(std::move(obj));

  // Also works for second argument.
  callByConstRef(1, std::move(obj));

  // Works if std::move() applied to a temporary.
  callByConstRef(std::move(NoMoveSemantics()));

  // Works if calling a copy constructor.
  NoMoveSemantics other(std::move(obj));

  // Works if calling assignment operator.
  other = std::move(obj);
}

class MoveSemantics {
public:
  MoveSemantics();
  // [CXX-P2008]: "Move constructor is declared without `noexcept`."
  MoveSemantics(MoveSemantics &&);

  // [CXX-P2008]: "Move-assignment constructor is declared without `noexcept`."
  MoveSemantics &operator=(MoveSemantics &&);
};

void callByValue(MoveSemantics);

void callByRValueRef(MoveSemantics &&);

template <class T>
void templateFunction(T obj) {
  T other = std::move(obj);
}

#define M3(T, obj)            \
  do {                        \
    T other = std::move(obj); \
  } while (true)

#define CALL(func) (func)()

void moveToConstReferenceNegatives() {
  // No warning when actual move takes place.
  MoveSemantics move_semantics;
  callByValue(std::move(move_semantics));
  callByRValueRef(std::move(move_semantics));
  MoveSemantics other(std::move(move_semantics));
  other = std::move(move_semantics);

  // No warning if std::move() not used.
  NoMoveSemantics no_move_semantics;
  callByConstRef(no_move_semantics);

  // No warning if instantiating a template.
  templateFunction(no_move_semantics);

  // No warning inside of macro expansions.
  M3(NoMoveSemantics, no_move_semantics);

  // No warning inside of macro expansion, even if the macro expansion is inside
  // a lambda that is, in turn, an argument to a macro.
  CALL([no_move_semantics] { M3(NoMoveSemantics, no_move_semantics); });

  auto lambda = [] {};
  auto lambda2 = std::move(lambda);
}

class MoveOnly {
public:
  MoveOnly(const MoveOnly &other) = delete;
  MoveOnly &operator=(const MoveOnly &other) = delete;
  MoveOnly(MoveOnly &&other) = default;
  MoveOnly &operator=(MoveOnly &&other) = default;
};
template <class T>
void Q(T);
void moveOnlyNegatives(MoveOnly val) {
  Q(std::move(val));
}

void fmovable(MoveSemantics);

void lambda1() {
  auto f = [](MoveSemantics m) {
    fmovable(std::move(m));
  };
  f(MoveSemantics());
}

template<class T> struct function {};

template<typename Result, typename... Args>
class function<Result(Args...)> {
public:
  function() = default;
  void operator()(Args... args) const {
    fmovable(std::forward<Args>(args)...);
  }
};

void functionInvocation() {
  function<void(MoveSemantics)> callback;
  MoveSemantics m;
  callback(std::move(m));
}

void lambda2() {
  function<void(MoveSemantics)> callback;

// [CXX-P2010]: 24 "`std::move` of trivally-copyable variable `callback` has no effect"
  auto f = [callback = std::move(callback)](MoveSemantics m) mutable {
    callback(std::move(m));
  };
  f(MoveSemantics());
}

void showInt(int &&v);
void showInt(int v1, int &&v2);
void showPointer(const char *&&s);
void showPointer2(const char *const &&s);
void showTriviallyCopyable(TriviallyCopyable &&obj);
void showTriviallyCopyablePointer(const TriviallyCopyable *&&obj);
void testFunctions() {
  int a = 10;
// [CXX-P2010]: 11 "`std::move` of trivally-copyable variable `a` has no effect"
  showInt(std::move(a));
  showInt(int());
// [CXX-P2010]: 14 "`std::move` of trivally-copyable variable `a` has no effect"
  showInt(a, std::move(a));
  const char* s = "";
// [CXX-P2010]: 15 "`std::move` of trivally-copyable variable `s` has no effect"
  showPointer(std::move(s));
// [CXX-P2010]: 16 "`std::move` of trivally-copyable variable `s` has no effect"
  showPointer2(std::move(s)); 
  TriviallyCopyable *obj = new TriviallyCopyable();
// [CXX-P2010]: 25 "`std::move` of trivally-copyable expression has no effect"
  showTriviallyCopyable(std::move(*obj));
// [CXX-P2010]: 32 "`std::move` of trivally-copyable variable `obj` has no effect"
  showTriviallyCopyablePointer(std::move(obj));
}
template <class T>
void forwardToShowInt(T && t) {
  showInt(static_cast<T &&>(t));
}
void testTemplate() {
  int a = 10;
// [CXX-P2010]: 20 "`std::move` of trivally-copyable variable `a` has no effect"
  forwardToShowInt(std::move(a));
}

struct Tmp {
  Tmp();
  Tmp(int &&a);
  Tmp(int v1, int &&a);
  Tmp(const char *&&s);
  Tmp(TriviallyCopyable&& obj);
  Tmp(const TriviallyCopyable *&&obj);
  void showTmp(TriviallyCopyable&& t);
  static void showTmpStatic(TriviallyCopyable&& t);
};
void testMethods() {
  Tmp t;
  int a = 10;
// [CXX-P2010]: 10 "`std::move` of trivally-copyable variable `a` has no effect"
  Tmp t1(std::move(a));
// [CXX-P2010]: 13 "`std::move` of trivally-copyable variable `a` has no effect"
  Tmp t2(a, std::move(a));
  const char* s = "";
// [CXX-P2010]: 10 "`std::move` of trivally-copyable variable `s` has no effect"
  Tmp t3(std::move(s));
  TriviallyCopyable *obj = new TriviallyCopyable();
// [CXX-P2010]: 10 "`std::move` of trivally-copyable expression has no effect"
  Tmp t4(std::move(*obj));
// [CXX-P2010]: 10 "`std::move` of trivally-copyable variable `obj` has no effect"
  Tmp t5(std::move(obj));
// [CXX-P2010]: 13 "`std::move` of trivally-copyable expression has no effect"
  t.showTmp(std::move(*obj));
// [CXX-P2010]: 22 "`std::move` of trivally-copyable expression has no effect"
  Tmp::showTmpStatic(std::move(*obj));
}

void showA(A &&v) {}
void testA() {
  A a;
  showA(std::move(a));
}

void testFuncPointer() {
  int a = 10;
  void (*choice)(int, int &&);
  choice = showInt;
// [CXX-P2010]: 24 "`std::move` of trivally-copyable variable `a` has no effect"
// [CXX-P2010]: 10 "`std::move` of trivally-copyable variable `a` has no effect"
  choice(std::move(a), std::move(a));
}
