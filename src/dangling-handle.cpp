// RUN: %check_clang_tidy -std=c++11,c++14 %s bugprone-dangling-handle %t -- \
// RUN:   -config="{CheckOptions: \
// RUN:             [{key: bugprone-dangling-handle.HandleClasses, \
// RUN:               value: 'std::basic_string_view; ::llvm::StringRef;'}]}"
// FIXME: Fix the checker to work in C++17 mode.

namespace std {
template <typename T>
class vector {
 public:
  using const_iterator = const T*;
  using iterator = T*;
  using size_type = int;

  void assign(size_type count, const T& value);
  iterator insert(const_iterator pos, const T& value);
  iterator insert(const_iterator pos, T&& value);
  iterator insert(const_iterator pos, size_type count, const T& value);
  void push_back(const T&);
  void push_back(T&&);
  void resize(size_type count, const T& value);
};

template <typename, typename>
class pair {};

template <typename T>
class set {
 public:
  using const_iterator = const T*;
  using iterator = T*;

  std::pair<iterator, bool> insert(const T& value);
  std::pair<iterator, bool> insert(T&& value);
  iterator insert(const_iterator hint, const T& value);
  iterator insert(const_iterator hint, T&& value);
};

template <typename Key, typename Value>
class map {
 public:
  using value_type = pair<Key, Value>;
  value_type& operator[](const Key& key);
  value_type& operator[](Key&& key);
};

class basic_string_view;

class basic_string {
 public:
  basic_string();
  basic_string(const char*);

  operator basic_string_view() const noexcept;

  ~basic_string();
};

typedef basic_string string;

class basic_string_view {
 public:
  basic_string_view(const char*);
};

typedef basic_string_view string_view;

}  // namespace std

namespace llvm {

class StringRef {
 public:
  StringRef();
  StringRef(const char*);
  StringRef(const std::string&);
};

}  // namespace llvm

std::string ReturnsAString();
std::string& ReturnsARefString();

void Positives() {
  // [CXX-W2004]: 20 "Handle `std::basic_string_view` outlives its value"
  std::string_view view1 = std::string();

  // [CXX-W2004]: 20 "Handle `std::basic_string_view` outlives its value"
  std::string_view view_2 = ReturnsAString();

  std::string_view valid_view = ReturnsARefString();

  // [CXX-W2004]: 3 "Handle `std::basic_string_view` outlives its value"
  view1 = std::string();

  const std::string& str_ref = "";
  // [CXX-W2004]: 20 "Handle `std::basic_string_view` outlives its value"
  std::string_view view3 = true ? "A" : str_ref;
  // [CXX-W2004]: 3 "Handle `std::basic_string_view` outlives its value"
  view3 = true ? "A" : str_ref;

  // [CXX-W2004]: 20 "Handle `std::basic_string_view` outlives its value"
  std::string_view view4(ReturnsAString());
}

const char static_array[] = "A";
std::string_view ReturnStatements(int i, std::string value_arg,
                                  const std::string &ref_arg) {
  const char array[] = "A";
  const char* ptr = "A";
  std::string s;
  static std::string ss;
  switch (i) {
    // Bad cases
    case 0:
      // [CXX-W2004]: 7 "Handle `std::basic_string_view` outlives its value"
      return array;  // refers to local
    case 1:
      // [CXX-W2004]: 7 "Handle `std::basic_string_view` outlives its value"
      return s;  // refers to local
    case 2:
      // [CXX-W2004]: 7 "Handle `std::basic_string_view` outlives its value"
      return std::string();  // refers to temporary
    case 3:
      // [CXX-W2004]: 7 "Handle `std::basic_string_view` outlives its value"
      return value_arg;  // refers to by-value arg

    // Ok cases
    case 100:
      return ss;  // refers to static
    case 101:
      return static_array;  // refers to static
    case 102:
      return ptr;  // pointer is ok
    case 103:
      return ref_arg;  // refers to by-ref arg
  }

  struct S {
    std::string_view view() { return value; }
    std::string value;
  };

  (void)[&]()->std::string_view {
    // This should not warn. The string is bound by reference.
    return s;
  };
  (void)[=]() -> std::string_view {
    // This should not warn. The reference is valid as long as the lambda.
    return s;
  };
  (void)[=]() -> std::string_view {
    // FIXME: This one should warn. We are returning a reference to a local
    // lambda variable.
    std::string local;
    return local;
  };
  return "";
}

void Containers() {
  std::vector<std::string_view> v;
  // [CXX-W2004]: 3 "Handle `std::basic_string_view` outlives its value"
  v.assign(3, std::string());
  // [CXX-W2004]: 3 "Handle `std::basic_string_view` outlives its value"
  v.insert(nullptr, std::string());
  // [CXX-W2004]: 3 "Handle `std::basic_string_view` outlives its value"
  v.insert(nullptr, 3, std::string());
  // [CXX-W2004]: 3 "Handle `std::basic_string_view` outlives its value"
  v.push_back(std::string());
  // [CXX-W2004]: 3 "Handle `std::basic_string_view` outlives its value"
  v.resize(3, std::string());

  std::set<std::string_view> s;
  // [CXX-W2004]: 3 "Handle `std::basic_string_view` outlives its value"
  s.insert(std::string());
  // [CXX-W2004]: 3 "Handle `std::basic_string_view` outlives its value"
  s.insert(nullptr, std::string());

  std::map<std::string_view, int> m;
  // [CXX-W2004]: 3 "Handle `std::basic_string_view` outlives its value"
  m[std::string()];
}

void TakesAStringView(std::string_view);

void Negatives(std::string_view default_arg = ReturnsAString()) {
  std::string str;
  std::string_view view = str;

  TakesAStringView(std::string());
}
