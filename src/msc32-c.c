// scatr-check: CXX-A1000
// RUN: %check_clang_tidy %s cert-msc32-c %t -- -config="{CheckOptions: [{key: cert-msc32-c.DisallowedSeedTypes, value: 'some_type,time_t'}]}" -- -std=c99

void srand(int seed);
typedef int time_t;
time_t time(time_t *t);

void f(void) {
  // [CXX-A1000]: "Predicatable values from randon number generator due to constant value as argument"
  srand(1);

  const int a = 1;
  // [CXX-A1000]: "Predicatable values from randon number generator due to constant value as argument"
  srand(a);

  time_t t;
  // [CXX-A1000]: "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  srand(time(&t)); // Disallowed seed type
}

void g(void) {
  typedef int user_t;
  user_t a = 1;
  srand(a);

  int b = 1;
  srand(b); // Can not evaluate as int
}
