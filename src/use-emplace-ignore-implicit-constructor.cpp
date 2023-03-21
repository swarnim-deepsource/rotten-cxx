// scatr-check: CXX-W2028

namespace std {
template <typename>
class initializer_list
{
public:
  initializer_list() noexcept {}
};

template <typename T>
class vector {
public:
  vector() = default;
  vector(initializer_list<T>) {}

  void push_back(const T &) {}
  void push_back(T &&) {}

  template <typename... Args>
  void emplace_back(Args &&... args){};
  ~vector();
};

} // namespace std

void testInts() {
  std::vector<int> v;
  v.push_back(42);
  v.push_back(int(42));
  v.push_back(int{42});
  v.push_back(42.0);
  int z;
  v.push_back(z);
}

struct Something {
  Something(int a, int b = 41) {}
  Something() {}
  void push_back(Something);
  int getInt() { return 42; }
};

struct Convertable {
  operator Something() { return Something{}; }
};

struct Zoz {
  Zoz(Something, int = 42) {}
};

Zoz getZoz(Something s) { return Zoz(s); }

void test_Something() {
  std::vector<Something> v;

  // [CXX-W2028]: "Consider using `emplace_back` over `push`"
  v.push_back(Something(1, 2));

  // [CXX-W2028]: "Consider using `emplace_back` over `push`"
  v.push_back(Something{1, 2});

  // [CXX-W2028]: "Consider using `emplace_back` over `push`"
  v.push_back(Something());

  // [CXX-W2028]: "Consider using `emplace_back` over `push`"
  v.push_back(Something{});

  Something Different;
  // [CXX-W2028]: "Consider using `emplace_back` over `push`"
  v.push_back(Something(Different.getInt(), 42));

  v.push_back(Different.getInt());
  v.push_back(42);

  Something temporary(42, 42);
  temporary.push_back(temporary);
  v.push_back(temporary);

  v.push_back(Convertable());
  v.push_back(Convertable{});
  Convertable s;
  v.push_back(s);
}

template <typename ElemType>
void dependOnElem() {
  std::vector<ElemType> v;
  v.push_back(ElemType(42));
}

template <typename ContainerType>
void dependOnContainer() {
  ContainerType v;
  v.push_back(Something(42));
}

void callDependent() {
  dependOnElem<Something>();
  dependOnContainer<std::vector<Something>>();
}

void test2() {
  std::vector<Zoz> v;
  // [CXX-W2028]: "Consider using `emplace_back` over `push`"
  v.push_back(Zoz(Something(21, 37)));

  // [CXX-W2028]: "Consider using `emplace_back` over `push`"
  v.push_back(Zoz(Something(21, 37), 42));

  v.push_back(getZoz(Something(1, 2)));
}
