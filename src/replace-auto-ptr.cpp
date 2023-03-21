// scatr-check: CXX-W2026

#include <memory>

// Instrumentation for auto_ptr_ref test.
struct Base {};
struct Derived : Base {};
// [CXX-W2026]
std::auto_ptr<Derived> create_derived_ptr();


// Test function return values (declaration)
// [CXX-W2026]
std::auto_ptr<char> f_5();


// Test function parameters.
// [CXX-W2026]
void f_6(std::auto_ptr<int>);
// [CXX-W2026]
void f_7(const std::auto_ptr<int> &);


// Test on record type fields.
struct A {
// [CXX-W2026]
  std::auto_ptr<int> field;
// [CXX-W2026]
  typedef std::auto_ptr<int> int_ptr_type;
};


// FIXME: Test template WITH instantiation.
template <typename T> struct B {
// [CXX-W2026]
  typedef typename std::auto_ptr<T> created_type;
// [CXX-W2026]
  created_type create() { return std::auto_ptr<T>(new T()); }
};


// Test 'using' in a namespace (declaration)
namespace ns_1 {
// Test multiple using declarations.
// [CXX-W2026]
  using std::auto_ptr;
// [CXX-W2026]
  using std::auto_ptr;
}


namespace ns_2 {
template <typename T> struct auto_ptr {};
}

void f_1() {
// [CXX-W2026]
  std::auto_ptr<int> a;

  // Check that spaces aren't modified unnecessarily.
// [CXX-W2026]
  std:: auto_ptr <int> b;
// [CXX-W2026]
  std :: auto_ptr < char > c(new char());

  // Test construction from a temporary.
// [CXX-W2026]
  std::auto_ptr<char> d = std::auto_ptr<char>();
// [CXX-W2026]
  typedef std::auto_ptr<int> int_ptr_t;
  int_ptr_t e(new int());

  // Test pointers.
// [CXX-W2026]
  std::auto_ptr<int> *f;

  // Test 'static' declarations.
// [CXX-W2026]
  static std::auto_ptr<int> g;

  // Test with cv-qualifiers.
// [CXX-W2026]
  const std::auto_ptr<int> h;
// [CXX-W2026]
  volatile std::auto_ptr<int> i;
// [CXX-W2026]
  const volatile std::auto_ptr<int> j;

  // Test auto and initializer-list.
// [CXX-W2026]
  auto k = std::auto_ptr<int>{};
// [CXX-W2026]
  std::auto_ptr<int> l{std::auto_ptr<int>()};

  // Test interlocked auto_ptr.
// [CXX-W2026]
  std::auto_ptr<std::auto_ptr<int> > m;

  // Test temporaries.
// [CXX-W2026]
  std::auto_ptr<char>();

  // Test void-specialization.
// [CXX-W2026]
  std::auto_ptr<void> n;

  // Test template WITH instantiation (instantiation).
  B<double> o;
// [CXX-W2026]
  std::auto_ptr<double> p(o.create());

  // Test 'using' in a namespace ("definition").
// [CXX-W2026]
  ns_1::auto_ptr<int> q;

  // Test construction with an 'auto_ptr_ref'.
// [CXX-W2026]
  std::auto_ptr<Base> r(create_derived_ptr());
}

// Test without the nested name specifiers.
void f_2() {
  using namespace std;
// [CXX-W2026]
  auto_ptr<int> a;
}

// Test using declaration.
void f_3() {
// [CXX-W2026]
  using std::auto_ptr;
// [CXX-W2026]
  auto_ptr<int> a;
}

// Test messing-up with macros.
void f_4() {
#define MACRO_1 <char>
// [CXX-W2026]
  std::auto_ptr MACRO_1 p(new char());
// [CXX-W2026]
#define MACRO_2 auto_ptr
  std::MACRO_2<int> q;
// [CXX-W2026]
#define MACRO_3(Type) std::auto_ptr<Type>
  MACRO_3(float)r(new float());
// [CXX-W2026]
#define MACRO_4 std::auto_ptr
  using MACRO_4;
#undef MACRO_1
#undef MACRO_2
#undef MACRO_3
#undef MACRO_4
}

// Test function return values (definition).
// [CXX-W2026]
std::auto_ptr<char> f_5()
{
  // Test constructor.
// [CXX-W2026]
  return std::auto_ptr<char>(new char());
}

// Test that non-std auto_ptr aren't replaced.
void f_8() {
  ns_2::auto_ptr<char> a;
  using namespace ns_2;
  auto_ptr<int> b;
}

// Fail to modify when the template is never instantiated.
//
// This might not be an issue. If it's never used it doesn't really matter if
// it's changed or not. If it's a header and one of the source use it, then it
// will still be changed.
template <typename X>
void f() {
  std::auto_ptr<X> p;
}

// FIXME: Alias template could be replaced if a matcher existed.
namespace std {
template <typename T> using aaaaaaaa = auto_ptr<T>;
}

// We want to avoid replacing 'aaaaaaaa' by unique_ptr here. It's better to
// change the type alias directly.
std::aaaaaaaa<int> d;

// [CXX-W2026]
void takes_ownership_fn(std::auto_ptr<int> x);
// [CXX-W2026]
std::auto_ptr<int> get_by_value();

class Wrapper {
 public:
// [CXX-W2026]
  std::auto_ptr<int> &get_wrapped();

 private:
// [CXX-W2026]
  std::auto_ptr<int> wrapped;
};

void f() {
// [CXX-W2026]
  std::auto_ptr<int> a, b, c;
  Wrapper wrapper_a, wrapper_b;
  a = b;
  wrapper_a.get_wrapped() = wrapper_b.get_wrapped();

  // Test that 'std::move()' is inserted when call to the
  // copy-constructor are made.
  takes_ownership_fn(c);
  takes_ownership_fn(wrapper_a.get_wrapped());

  // [CXX-W2026]
  std::auto_ptr<int> d[] = { std::auto_ptr<int>(new int(1)),
                            // [CXX-W2026]
                             std::auto_ptr<int>(new int(2)) };
  // [CXX-W2026]
  std::auto_ptr<int> e = d[0];

  // Test that std::move() is not used when assigning an rvalue
// [CXX-W2026]
  std::auto_ptr<int> f;
// [CXX-W2026]
  f = std::auto_ptr<int>(new int(0));
// [CXX-W2026]
  std::auto_ptr<int> g = get_by_value();
}
