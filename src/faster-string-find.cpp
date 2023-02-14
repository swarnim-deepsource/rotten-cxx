// RUN: %check_clang_tidy %s performance-faster-string-find %t
// RUN: %check_clang_tidy -check-suffix=CUSTOM %s performance-faster-string-find
// %t -- \
// RUN:   -config="{CheckOptions: \
// RUN:             [{key: performance-faster-string-find.StringLikeClasses, \
// RUN:               value: '::llvm::StringRef;'}]}"

namespace std {
template <typename Char> struct basic_string {
  int find(const Char *, int = 0) const;
  int find(const Char *, int, int) const;
  int rfind(const Char *) const;
  int find_first_of(const Char *) const;
  int find_first_not_of(const Char *) const;
  int find_last_of(const Char *) const;
  int find_last_not_of(const Char *) const;
};

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;

template <typename Char> struct basic_string_view {
  int find(const Char *, int = 0) const;
  int find(const Char *, int, int) const;
  int rfind(const Char *) const;
  int find_first_of(const Char *) const;
  int find_first_not_of(const Char *) const;
  int find_last_of(const Char *) const;
  int find_last_not_of(const Char *) const;
};

typedef basic_string_view<char> string_view;
typedef basic_string_view<wchar_t> wstring_view;
} // namespace std

namespace llvm {
struct StringRef {
  int find(const char *) const;
};
} // namespace llvm

struct NotStringRef {
  int find(const char *);
};

void StringFind() {
  std::string Str;
  // [CXX-P2003]: "Found inefficient method `find` for the character look-up."
  Str.find("a");

  // Works with the pos argument.
  // [CXX-P2003]: "Found inefficient method `find` for the character look-up."
  Str.find("a", 1);

  // Doens't work with strings smaller or larger than 1 char.
  Str.find("");
  Str.find("ab");

  // Doesn't do anything with the 3 argument overload.
  Str.find("a", 1, 1);

  // Other methods that can also be replaced
  // [CXX-P2003]: "Found inefficient method `rfind` for the character look-up."
  Str.rfind("a");
  // [CXX-P2003]: "Found inefficient method `find_first_of` for the character
  // look-up."
  Str.find_first_of("a");
  // [CXX-P2003]: "Found inefficient method `find_first_not_of` for the
  // character look-up."
  Str.find_first_not_of("a");
  // [CXX-P2003]: "Found inefficient method `find_last_of` for the character
  // look-up."
  Str.find_last_of("a");
  // [CXX-P2003]: "Found inefficient method `find_last_not_of` for the character
  // look-up."
  Str.find_last_not_of("a");

  // std::wstring should work.
  std::wstring WStr;
  // [CXX-P2003]: "Found inefficient method `find` for the character look-up."
  WStr.find(L"n");
  // Even with unicode that fits in one wide char.
  // [CXX-P2003]: "Found inefficient method `find` for the character look-up."
  WStr.find(L"\x3A9");

  // std::string_view and std::wstring_view should work.
  std::string_view StrView;
  // [CXX-P2003]: "Found inefficient method `find` for the character look-up."
  StrView.find("n");
  std::wstring_view WStrView;

  // [CXX-P2003]: "Found inefficient method `find` for the character look-up."
  WStrView.find(L"n");
  // [CXX-P2003]: "Found inefficient method `find` for the character look-up."
  WStrView.find(L"\x3A9");

  // Also with other types, but only if it was specified in the options.
  llvm::StringRef sr;
  sr.find("x");
  NotStringRef nsr;
  nsr.find("x");
}

template <typename T> int FindTemplateDependant(T value) {
  return value.find("A");
}
template <typename T> int FindTemplateNotDependant(T pos) {
  // [CXX-P2003]: "Found inefficient method `find` for the character look-up."
  return std::string().find("A", pos);
}

int FindStr() {
  return FindTemplateDependant(std::string()) + FindTemplateNotDependant(1);
}

#define STR_MACRO(str) str.find("A."
#define POS_MACRO(pos) std::string().find("A", pos)

int Macros() {
  // FIXME: Raise CXX-P2003
  return STR_MACRO(std::string()) + POS_MACRO(1);
}
