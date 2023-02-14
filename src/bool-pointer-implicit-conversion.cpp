// RUN: %check_clang_tidy %s bugprone-bool-pointer-implicit-conversion %t

bool *SomeFunction();
void SomeOtherFunction(bool *);
bool F();
void G(bool);

template <typename T> void t(T b) {
  if (b) {
  }
}

void foo() {
  bool *b = SomeFunction();
  // [CXX-W2002]: 7 "Dubious check of `bool *b` against `nullptr`"
  if (b) {
  }

  // [CXX-W2002]: 14 "Dubious check of `bool *b` against `nullptr`"
  if (F() && b) {
  }

  // TODO: warn here.
  if (b) {
    G(b);
  }

#define TESTMACRO if (b || F())

  TESTMACRO {}

  t(b);

  if (!b) {
    // no-warning
  }

  if (SomeFunction()) {
    // no-warning
  }

  bool *c = SomeFunction();
  if (c) {
    (void)c;
    (void)*c; // no-warning
  }

  if (c) {
    *c = true; // no-warning
  }

  if (c) {
    c[0] = false; // no-warning
  }

  if (c) {
    SomeOtherFunction(c); // no-warning
  }

  if (c) {
    delete[] c; // no-warning
  }

  if (c) {
    *(c) = false; // no-warning
  }

  struct {
    bool *b;
  } d = {SomeFunction()};

  // [CXX-W2002]: 7 "Dubious check of `bool *d.b` against `nullptr`"
  if (d.b) {
  }

  if (d.b) {
    (void)*d.b; // no-warning
  }

#define CHECK(b)                                                               \
  if (b) {                                                                     \
  }
  CHECK(c)
}

struct H {
  bool *b;
  void foo() const {
    // [CXX-W2002]: 9 "Dubious check of `bool *b` against `nullptr`"
    if (b) {
    }

    if (b) {
      (void)*b; // no-warning
    }
  }
};
