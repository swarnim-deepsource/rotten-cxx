// scatr-check: CXX-W2022
namespace std {

struct future {};

enum class launch {
  async,
  deferred
};

template <typename Function, typename... Args>
future async(Function &&, Args &&...);

template <typename Function, typename... Args>
future async(launch, Function &&, Args &&...);

template <typename ForwardIt, typename T>
ForwardIt remove(ForwardIt, ForwardIt, const T &);

template <typename ForwardIt, typename UnaryPredicate>
ForwardIt remove_if(ForwardIt, ForwardIt, UnaryPredicate);

template <typename ForwardIt>
ForwardIt unique(ForwardIt, ForwardIt);

template <typename T>
struct default_delete;

template <typename T, typename Deleter = std::default_delete<T>>
struct unique_ptr {
  T *release() noexcept;
};

template <typename T>
struct char_traits;

template <typename T>
struct allocator;

template <typename CharT,
          typename Traits = char_traits<CharT>,
          typename Allocator = allocator<CharT>>
struct basic_string {
  bool empty() const;
};

typedef basic_string<char> string;

template <typename T, typename Allocator = std::allocator<T>>
struct vector {
  bool empty() const noexcept;
};

// the check should be able to match std lib calls even if the functions are
// declared inside inline namespaces
inline namespace v1 {

template <typename T>
T *launder(T *);

} // namespace v1
} // namespace std

struct Foo {
  void f();
};

int increment(int i) {
  return i + 1;
}

void useFuture(const std::future &fut);

void warning() {
  // [CXX-W2022]: "Return value of the function `async` is never checked"
  std::async(increment, 42);

  // [CXX-W2022]: "Return value of the function `async` is never checked"
  std::async(std::launch::async, increment, 42);

  Foo F;
  // [CXX-W2022]: "Return value of the function `launder` is never checked"
  std::launder(&F);

  // [CXX-W2022]: "Return value of the function `remove` is never checked"
  std::remove(nullptr, nullptr, 1);

  // [CXX-W2022]: "Return value of the function `remove_if` is never checked"
  std::remove_if(nullptr, nullptr, nullptr);

  // [CXX-W2022]: "Return value of the function `unique` is never checked"
  std::unique(nullptr, nullptr);

  std::unique_ptr<Foo> UPtr;
  // [CXX-W2022]: "Return value of the function `release` is never checked"
  UPtr.release();

  std::string Str;
  // [CXX-W2022]: "Return value of the function `empty` is never checked"
  Str.empty();

  std::vector<Foo> Vec;
  // [CXX-W2022]: "Return value of the function `empty` is never checked"
  Vec.empty();

  // test discarding return values inside different kinds of statements

  // [CXX-W2022]: "Return value of the function `remove` is never checked"
  auto Lambda = [] { std::remove(nullptr, nullptr, 1); };

  if (true)
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);
  else if (true)
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);
  else
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);

  while (true)
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);

  do
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);
  while (true);

  for (;;)
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);

  // [CXX-W2022]: "Return value of the function `remove` is never checked"
  for (std::remove(nullptr, nullptr, 1);;)
    ;

  // [CXX-W2022]: "Return value of the function `remove` is never checked"
  for (;; std::remove(nullptr, nullptr, 1))
    ;

  for (auto C : "foo")
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);

  switch (1) {
  case 1:
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);
    break;
  default:
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);
    break;
  }

  try {
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);
  } catch (...) {
    // [CXX-W2022]: "Return value of the function `remove` is never checked"
    std::remove(nullptr, nullptr, 1);
  }
}

void noWarning() {
  auto AsyncRetval1 = std::async(increment, 42);
  auto AsyncRetval2 = std::async(std::launch::async, increment, 42);

  Foo FNoWarning;
  auto LaunderRetval = std::launder(&FNoWarning);

  auto RemoveRetval = std::remove(nullptr, nullptr, 1);

  auto RemoveIfRetval = std::remove_if(nullptr, nullptr, nullptr);

  auto UniqueRetval = std::unique(nullptr, nullptr);

  std::unique_ptr<Foo> UPtrNoWarning;
  auto ReleaseRetval = UPtrNoWarning.release();

  std::string StrNoWarning;
  auto StrEmptyRetval = StrNoWarning.empty();

  std::vector<Foo> VecNoWarning;
  auto VecEmptyRetval = VecNoWarning.empty();

  // test using the return value in different kinds of expressions
  useFuture(std::async(increment, 42));
  std::launder(&FNoWarning)->f();
  delete std::launder(&FNoWarning);

  if (std::launder(&FNoWarning))
    ;
  for (; std::launder(&FNoWarning);)
    ;
  while (std::launder(&FNoWarning))
    ;
  do
    ;
  while (std::launder(&FNoWarning));
  switch (std::unique(1, 1))
    ;

  // cast to void should allow ignoring the return value
  (void)std::async(increment, 42);

  // test discarding return value of functions that are not configured to be checked
  increment(1);

  // test that the check is disabled inside GNU statement expressions
  ({ std::async(increment, 42); });
  auto StmtExprRetval = ({ std::async(increment, 42); });
}
