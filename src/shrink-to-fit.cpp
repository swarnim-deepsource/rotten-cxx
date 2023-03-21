// scatr-check: CXX-W2027

namespace std {
template <typename T> struct vector { void swap(vector &other); };
}

void f() {
  std::vector<int> v;

  // [CXX-W2027]: "Call `shrink_to_fit()` rather than using the copy-swap trick"
  std::vector<int>(v).swap(v);

  std::vector<int> &vref = v;
  // [CXX-W2027]: "Call `shrink_to_fit()` rather than using the copy-swap trick"
  std::vector<int>(vref).swap(vref);

  std::vector<int> *vptr = &v;
  // [CXX-W2027]: "Call `shrink_to_fit()` rather than using the copy-swap trick"
  std::vector<int>(*vptr).swap(*vptr);
}

struct X {
  std::vector<int> v;
  void f() {
  // [CXX-W2027]: "Call `shrink_to_fit()` rather than using the copy-swap trick"
    std::vector<int>(v).swap(v);

    std::vector<int> *vptr = &v;
  // [CXX-W2027]: "Call `shrink_to_fit()` rather than using the copy-swap trick"
    std::vector<int>(*vptr).swap(*vptr);
  }
};

template <typename T> void g() {
  std::vector<int> v;
  // [CXX-W2027]: "Call `shrink_to_fit()` rather than using the copy-swap trick"
  std::vector<int>(v).swap(v);

  std::vector<T> v2;
  std::vector<T>(v2).swap(v2);
}

template <typename T> void g2() {
  std::vector<int> v;
  // [CXX-W2027]: "Call `shrink_to_fit()` rather than using the copy-swap trick"
  std::vector<int>(v).swap(v);

  T v3;
  T(v3).swap(v3);
}

#define COPY_AND_SWAP_INT_VEC(x) std::vector<int>(x).swap(x)

void h() {
  g<int>();
  g<double>();
  g<bool>();
  g2< std::vector<int> >();
  std::vector<int> v;
  COPY_AND_SWAP_INT_VEC(v);
}

void PR38315() {
  typedef std::vector<int> Vector;
  Vector v;
  // [CXX-W2027]: "Call `shrink_to_fit()` rather than using the copy-swap trick"
  Vector(v).swap(v);

  using Vector2 = std::vector<int>;
  Vector2 v2;
  // [CXX-W2027]: "Call `shrink_to_fit()` rather than using the copy-swap trick"
  Vector2(v2).swap(v2);
}
