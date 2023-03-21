// scatr-check: CXX-W2021
// RUN: %check_clang_tidy %s cert-oop57-cpp %t -- \
// RUN: -config='{CheckOptions: \
// RUN:  [{key: cert-oop57-cpp.MemSetNames, value: mymemset}, \
// RUN:  {key: cert-oop57-cpp.MemCpyNames, value: mymemcpy}, \
// RUN:  {key: cert-oop57-cpp.MemCmpNames, value: mymemcmp}]}' \
// RUN: --

void mymemset(void *, unsigned char, decltype(sizeof(int)));
void mymemcpy(void *, const void *, decltype(sizeof(int)));
int mymemcmp(const void *, const void *, decltype(sizeof(int)));

namespace std {
void memset(void *, unsigned char, decltype(sizeof(int)));
void memcpy(void *, const void *, decltype(sizeof(int)));
void memmove(void *, const void *, decltype(sizeof(int)));
void strcpy(void *, const void *, decltype(sizeof(int)));
int memcmp(const void *, const void *, decltype(sizeof(int)));
int strcmp(const void *, const void *, decltype(sizeof(int)));
} // namespace std

struct Trivial {
  int I;
  int J;
};

struct NonTrivial {
  int I;
  int J;

  NonTrivial() : I(0), J(0) {}
  NonTrivial &operator=(const NonTrivial &Other) {
    I = Other.I;
    J = Other.J;
    return *this;
  }

  bool operator==(const Trivial &Other) const {
    return I == Other.I && J == Other.J;
  }
  bool operator!=(const Trivial &Other) const {
    return !(*this == Other);
  }
};

void foo(const Trivial &Other) {
  Trivial Data;
  std::memset(&Data, 0, sizeof(Data));
  std::memset(&Data, 0, sizeof(Trivial));
  std::memcpy(&Data, &Other, sizeof(Data));
  std::memmove(&Data, &Other, sizeof(Data));
  std::strcpy(&Data, &Other, sizeof(Data));
  std::memcmp(&Data, &Other, sizeof(Data));
  std::strcmp(&Data, &Other, sizeof(Data));
}

void bar(const NonTrivial &Other) {
  NonTrivial Data;
  // [CXX-W2021]: "Behavior of calling `memset` on a non-trivally constructable class is undefined"
  std::memset(&Data, 0, sizeof(Data));
  // Check it detects sizeof(Type) as well as sizeof(Instantiation)
  // [CXX-W2021]: "Behavior of calling `memset` on a non-trivally constructable class is undefined"
  std::memset(&Data, 0, sizeof(NonTrivial));
  // [CXX-W2021]: "Behavior of calling `memcpy` on a non-trivally copiable class is undefined"
  std::memcpy(&Data, &Other, sizeof(Data));
  // [CXX-W2021]: "Behavior of calling `memmove` on a non-trivally copiable class is undefined"
  std::memmove(&Data, &Other, sizeof(Data));
  // [CXX-W2021]: "Behavior of calling `strcpy` on a non-trivally copiable class is undefined"
  std::strcpy(&Data, &Other, sizeof(Data));
  // [CXX-W2021]: "Using `memcmp` instead of the comparison operator provided by the class"
  std::memcmp(&Data, &Other, sizeof(Data));
  // [CXX-W2021]: "Using `strcmp` instead of the comparison operator provided by the class"
  std::strcmp(&Data, &Other, sizeof(Data));
}

void baz(const NonTrivial &Other) {
  NonTrivial Data;
  mymemset(&Data, 0, sizeof(Data));
  mymemcpy(&Data, &Other, sizeof(Data));
  mymemcmp(&Data, &Other, sizeof(Data));
}

void nonNullSetValue() {
  NonTrivial Data;
  // Check non-null-valued second argument.
  // [CXX-W2021]: "Behavior of calling `memset` on a non-trivally constructable class is undefined"
  std::memset(&Data, 1, sizeof(Data));
}

void nonLiteralSetValue(char C) {
  NonTrivial Data;
  // Check non-literal second argument.
  // [CXX-W2021]: "Behavior of calling `memset` on a non-trivally constructable class is undefined"
  std::memset(&Data, C, sizeof(Data));
}
