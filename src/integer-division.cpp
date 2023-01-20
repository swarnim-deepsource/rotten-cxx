// clang_tidy
// Functions expecting a floating-point parameter.
void floatArg(float x) {}
void doubleArg(double x) {}
void longDoubleArg(long double x) {}

// Functions expected to return a floating-point value.
float singleDiv() {
  int x = -5;
  int y = 2;
  // [CXX-W2001]: 10 `x / y` integer division used in a floating point context"
  return x / y;
}

double wrongOrder(int x, int y) {
  // [CXX-W2001]: 10 "`x / y` integer division used in a floating point context"
  return x / y / 0.1;
}

long double rightOrder(int x, int y) {
  return 0.1 / x / y; // OK
}

// Typical mathematical functions.
float sin(float);
double acos(double);
long double tanh(long double);

namespace std {
using ::sin;
}

template <typename T> void intDivSin(T x) { sin(x); }

int intFunc(int);

struct X {
  int n;
  void m() {
    // [CXX-W2001]: 9 "`n / 3` integer division used in a floating point context"
    sin(n / 3);
  }
};

void integerDivision() {
  char a = 2;
  short b = -5;
  int c = 9784;
  enum third { x, y, z = 2 };
  third d = z;
  char e[] = {'a', 'b', 'c'};
  char f = *(e + 1 / a);
  bool g = 1;

  // [CXX-W2001]: 11 "`c / (2 + 2)` integer division used in a floating point context"
  sin(1 + c / (2 + 2));
  sin(c / (1 + .5));
  sin((c + .5) / 3);

  // [CXX-W2001]: 7 "`intFunc(3) / 5` integer division used in a floating point context"
  sin(intFunc(3) / 5);
  // [CXX-W2001]: 8 "`2 / intFunc(7)` integer division used in a floating point context"
  acos(2 / intFunc(7));

  // [CXX-W2001]: 16 "`2 / 3` integer division used in a floating point context"
  floatArg(1 + 2 / 3);
  // [CXX-W2001]: 11 "`2 / 3` integer division used in a floating point context"
  sin(1 + 2 / 3);

  // [CXX-W2001]: 19 "`2 / 3` integer division used in a floating point context"
  intFunc(sin(1 + 2 / 3));

  floatArg(1 + intFunc(1 + 2 / 3));
  floatArg(1 + 3 * intFunc(a / b));

  1 << (2 / 3);
  1 << intFunc(2 / 3);

#define M_SIN sin(a / b);
  // TODO: My guess is Unknown location error 8 "`x / y` integer division used in a floating point context"
  M_SIN

  // [CXX-W2001]: 20 "`a / b` integer division used in a floating point context"
  intDivSin<float>(a / b);

  // [CXX-W2001]: 21 "`c / d` integer division used in a floating point context"
  intDivSin<double>(c / d);

  // [CXX-W2001]: 26 "`f / g` integer division used in a floating point context"
  intDivSin<long double>(f / g);

  // [CXX-W2001]: 12 "`1 / 3` integer division used in a floating point context"
  floatArg(1 / 3);

  // [CXX-W2001]: 13 "`a / b` integer division used in a floating point context"
  doubleArg(a / b);

  // [CXX-W2001]: 17 "`3 / d` integer division used in a floating point context"
  longDoubleArg(3 / d);

  // [CXX-W2001]: 12 "`a / b` integer division used in a floating point context"
  floatArg(a / b / 0.1);

  // [CXX-W2001]: 13 "`1 / 3` integer division used in a floating point context"
  doubleArg(1 / 3 / 0.1);

  // [CXX-W2001]: 12 "`2 / 3` integer division used in a floating point context"
  std::sin(2 / 3);

  // [CXX-W2001]: 10 "`7 / d` integer division used in a floating point context"
  ::acos(7 / d);

  // [CXX-W2001]: 8 "`f / g` integer division used in a floating point context"
  tanh(f / g);

  floatArg(0.1 / a / b);
  doubleArg(0.1 / 3 / 1);

  singleDiv();
  wrongOrder(a, b);
  rightOrder(a, b);

  // [CXX-W2001]: 7 "`a / b` integer division used in a floating point context"
  sin(a / b);

  // [CXX-W2001]: 8 "`f / d` integer division used in a floating point context"
  acos(f / d);

  // [CXX-W2001]: 8 "`c / g` integer division used in a floating point context"
  tanh(c / g);

  sin(3.0 / a);
  acos(b / 3.14);
  tanh(3.14 / f / g);

  sin((float)3 / a);
  // [CXX-P2001]: 3 `acos(..)` accepts a `double` causing implicit type promotion of `float`
  acos(b / (float)3);
  // [CXX-W2001]: 8 "`(int)3.1400000000000001 / f / g` integer division used in a floating point context", `(int)3.1400000000000001 / f`
  tanh((int)3.14 / f / g);

  sin(3.0 / (float)a);
  acos((float)b / 3.14);
  // [CXX-W2001]: 8 "`(int)3.1400000000000001 / f` integer division used in a floating point context"
  tanh((int)3.14 / f / (float)g);
}
