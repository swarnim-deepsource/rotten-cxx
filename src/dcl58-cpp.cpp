#include "system-header-simulation.h"

namespace A {
  namespace B {
// [CXX-W2009]: 5 "Non-const variable `b` placed in global scope"
    int b;
  }
}

namespace A {
  namespace B {
// [CXX-W2009]: 5 "Non-const variable `c` placed in global scope"
    int c;
  }
}

namespace posix {
// [CXX-W2017]: 1 "Found modification to standard namespace"
namespace foo {
// [CXX-W2009]: 1 "Non-const variable `foobar` placed in global scope"
int foobar;
}
}

namespace std {
// [CXX-W2009]: 1 "Non-const variable `stdInt` placed in global scope"
// [CXX-W2017]: 1 "Found modification to standard namespace"
int stdInt;
// [CXX-W2009]: 1 "Non-const variable `stdInt1` placed in global scope"
// [CXX-W2017]: 1 "Found modification to standard namespace"
int stdInt1;
}

namespace foobar {
  namespace std {
// [CXX-W2009]: 5 "Non-const variable `bar` placed in global scope"
    int bar;
  }
}

namespace posix {
// [CXX-W2017]: 1 "Found modification to standard namespace"
namespace std {
}
} // namespace posix

// [CXX-W2017]: 16 "Found modification to standard namespace"
namespace posix::a {
}

namespace std {
// no-warning: empty
} // namespace std

namespace std {
// Warn for non-NamedDecls as well.
// [CXX-W2017]: 1 "Found modification to standard namespace"
static_assert(1 == 1, "non-NamedDecl");
} // namespace std

enum class MyError {
  ErrorA,
  ErrorB
};

namespace std {
// no-warning: Class template specialized by a program-defined type.
template <>
struct is_error_code_enum<MyError> : std::true_type {};

// no-warning: Function template specialized by a program-defined type.
template<>
void swap<MyError>(MyError &a, MyError &b);
}

using ConstBoolPtr = const bool *;

namespace std {
// class template, builtin type
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
struct is_error_code_enum<bool> : std::true_type {};
// function template, builtin type
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
void swap<bool>(bool &, bool &);
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
void swap<ConstBoolPtr>(ConstBoolPtr &, ConstBoolPtr &);
} // namespace std

namespace std {
// class template, std type
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
struct is_error_code_enum<std::io_errc> : std::true_type {};
// function template, std type
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
void swap<std::io_errc>(std::io_errc &, std::io_errc &);
} // namespace std

// parameter pack, has program-defined type
namespace std {
// no-warning: there is one program-defined type.
template <>
class tuple<int, MyError, std::io_errc> {};
} // namespace std

// parameter pack, only builtin or std type
namespace std {
// Forbid variadic specializations over only `std::` or builtin types.
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
class tuple<int, const std::io_errc, float> {};
} // namespace std

namespace std {
// Test nested standard declarations.
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
struct is_error_code_enum<std::Outer::Inner> : std::true_type {};
} // namespace std

namespace std {
// Test nested namespace.
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
struct is_error_code_enum<std::detail::X> : std::true_type {};
} // namespace std

// Test member function template specializations.
namespace std {
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
bool less<void>::operator()<int &&, float &&>(int &&, float &&) const {
  return true;
}
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
bool less<void>::operator()<MyError &&, MyError &&>(MyError &&, MyError &&) const {
  return true;
}
} // namespace std

// Test member class template specializations.
namespace std {
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
struct less<void>::X<bool> {};
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
struct less<void>::X<MyError> {};
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <typename T>
struct less<void>::X<MyError, T> {};
} // namespace std

// We did not open the 'std' namespace, but still specialized the member
// function of 'std::less'.
// no-note: There is no opening of 'std' namespace, hence no note emitted.
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
bool std::less<void>::operator()<int &&, int &&>(int &&, int &&) const {
  return true;
}

namespace SpaceA {
namespace SpaceB {
class MapKey {
  int Type = 0;

public:
  MapKey() = default;
  int getType() const { return Type; }
};
} // namespace SpaceB
} // namespace SpaceA

// no-warning: Specializing for 'std::hash' for a program-defined type.
template <>
struct std::hash<::SpaceA::SpaceB::MapKey> {
  // no-warning
  unsigned long operator()(const ::SpaceA::SpaceB::MapKey &K) const {
    return K.getType();
  }
  // no-warning
  bool operator()(const ::SpaceA::SpaceB::MapKey &K1,
                  const ::SpaceA::SpaceB::MapKey &K2) const {
    return K1.getType() < K2.getType();
  }
};

using myint = int;

// The type alias declaration is the same as typedef, does not introduce a
// program-defined type.
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
struct std::hash<myint> {
  // no-warning: The warning was already reported for the struct itself.
  unsigned long operator()(const myint &K) const {
    return K;
  }
  // no-warning: The warning was already reported for the struct itself.
  bool operator()(const myint &K1,
                  const myint &K2) const {
    return K1 < K2;
  }
};

// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
struct ::std::hash<long> {
  unsigned long operator()(const long &K) const {
    return K;
  }
};

namespace ranges {
namespace detail {
struct diffmax_t {};
using LongT = long;
} // namespace detail
} // namespace ranges

namespace std {
// no-warning: specialization with an user-defined type
template <>
struct numeric_limits<::ranges::detail::diffmax_t> {
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = true;
  static constexpr ::ranges::detail::diffmax_t max() noexcept {
    return {};
  }
};
inline constexpr bool numeric_limits<::ranges::detail::diffmax_t>::is_signed;
inline constexpr bool numeric_limits<::ranges::detail::diffmax_t>::is_integer;
} // namespace std

namespace std {
// specialization with type alias to non-program-defined-type
// [CXX-W2017]: 1 "Found modification to standard namespace"
template <>
struct numeric_limits<::ranges::detail::LongT> {
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = true;
  static constexpr ::ranges::detail::LongT max() noexcept {
    return 1;
  }
};
inline constexpr bool numeric_limits<::ranges::detail::LongT>::is_signed;
inline constexpr bool numeric_limits<::ranges::detail::LongT>::is_integer;
} // namespace std
