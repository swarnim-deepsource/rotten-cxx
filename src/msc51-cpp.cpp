// scatr-check: CXX-A1000
  // RUN: %check_clang_tidy %s cert-msc51-cpp %t -- \
  // RUN:     -config="{CheckOptions: [{key: cert-msc51-cpp.DisallowedSeedTypes, value: 'some_type,time_t'}]}"

namespace std {

void srand(int seed);

template <class UIntType, UIntType a, UIntType c, UIntType m>
struct linear_congruential_engine {
  linear_congruential_engine(int _ = 0);
  void seed(int _ = 0);
};
using default_random_engine = linear_congruential_engine<unsigned int, 1, 2, 3>;

using size_t = int;
template <class UIntType, size_t w, size_t n, size_t m, size_t r,
          UIntType a, size_t u, UIntType d, size_t s,
          UIntType b, size_t t,
          UIntType c, size_t l, UIntType f>
struct mersenne_twister_engine {
  mersenne_twister_engine(int _ = 0);
  void seed(int _ = 0);
};
using mt19937 = mersenne_twister_engine<unsigned int, 32, 624, 397, 21, 0x9908b0df, 11, 0xffffffff, 7, 0x9d2c5680, 15, 0xefc60000, 18, 1812433253>;

template <class UIntType, size_t w, size_t s, size_t r>
struct subtract_with_carry_engine {
  subtract_with_carry_engine(int _ = 0);
  void seed(int _ = 0);
};
using ranlux24_base = subtract_with_carry_engine<unsigned int, 24, 10, 24>;

template <class Engine, size_t p, size_t r>
struct discard_block_engine {
  discard_block_engine();
  discard_block_engine(int _);
  void seed();
  void seed(int _);
};
using ranlux24 = discard_block_engine<ranlux24_base, 223, 23>;

template <class Engine, size_t w, class UIntType>
struct independent_bits_engine {
  independent_bits_engine();
  independent_bits_engine(int _);
  void seed();
  void seed(int _);
};
using independent_bits = independent_bits_engine<ranlux24_base, 223, int>;

template <class Engine, size_t k>
struct shuffle_order_engine {
  shuffle_order_engine();
  shuffle_order_engine(int _);
  void seed();
  void seed(int _);
};
using shuffle_order = shuffle_order_engine<ranlux24_base, 223>;

struct random_device {
  random_device();
  int operator()();
};
} // namespace std

using time_t = unsigned int;
time_t time(time_t *t);

void f() {
  const int seed = 2;
  time_t t;

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  std::srand(0);
  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  std::srand(seed);
  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  std::srand(time(&t));

  // One instantiation for every engine
  // [CXX-A1000]: 30 "Predicatable values from randon number generator due to default argument"
  std::default_random_engine engine1;
  // [CXX-A1000]: 30 "Predicatable values from randon number generator due to constant value as argument"
  std::default_random_engine engine2(1);
  // [CXX-A1000]: 30 "Predicatable values from randon number generator due to constant value as argument"
  std::default_random_engine engine3(seed);

  // [CXX-A1000]: 30 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  std::default_random_engine engine4(time(&t));

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to default argument"
  engine1.seed();

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine1.seed(1);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine1.seed(seed);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  engine1.seed(time(&t));


  // [CXX-A1000]: 16 "Predicatable values from randon number generator due to default argument"
  std::mt19937 engine5;

  // [CXX-A1000]: 16 "Predicatable values from randon number generator due to constant value as argument"
  std::mt19937 engine6(1);

  // [CXX-A1000]: 16 "Predicatable values from randon number generator due to constant value as argument"
  std::mt19937 engine7(seed);

  // [CXX-A1000]: 16 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  std::mt19937 engine8(time(&t));

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to default argument"
  engine5.seed();

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine5.seed(1);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine5.seed(seed);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  engine5.seed(time(&t));


  // [CXX-A1000]: 22 "Predicatable values from randon number generator due to default argument"
  std::ranlux24_base engine9;

  // [CXX-A1000]: 22 "Predicatable values from randon number generator due to constant value as argument"
  std::ranlux24_base engine10(1);

  // [CXX-A1000]: 22 "Predicatable values from randon number generator due to constant value as argument"
  std::ranlux24_base engine11(seed);

  // [CXX-A1000]: 22 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  std::ranlux24_base engine12(time(&t));

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to default argument"
  engine9.seed();

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine9.seed(1);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine9.seed(seed);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  engine9.seed(time(&t));


  // [CXX-A1000]: 17 "Predicatable values from randon number generator due to default argument"
  std::ranlux24 engine13;

  // [CXX-A1000]: 17 "Predicatable values from randon number generator due to constant value as argument"
  std::ranlux24 engine14(1);

  // [CXX-A1000]: 17 "Predicatable values from randon number generator due to constant value as argument"
  std::ranlux24 engine15(seed);

  // [CXX-A1000]: 17 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  std::ranlux24 engine16(time(&t));

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to default argument"
  engine13.seed();

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine13.seed(1);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine13.seed(seed);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  engine13.seed(time(&t));


  // [CXX-A1000]: 25 "Predicatable values from randon number generator due to default argument"
  std::independent_bits engine17;

  // [CXX-A1000]: 25 "Predicatable values from randon number generator due to constant value as argument"
  std::independent_bits engine18(1);

  // [CXX-A1000]: 25 "Predicatable values from randon number generator due to constant value as argument"
  std::independent_bits engine19(seed);

  // [CXX-A1000]: 25 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  std::independent_bits engine20(time(&t));

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to default argument"
  engine17.seed();

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine17.seed(1);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine17.seed(seed);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  engine17.seed(time(&t));


  // [CXX-A1000]: 22 "Predicatable values from randon number generator due to default argument"
  std::shuffle_order engine21;

  // [CXX-A1000]: 22 "Predicatable values from randon number generator due to constant value as argument"
  std::shuffle_order engine22(1);

  // [CXX-A1000]: 22 "Predicatable values from randon number generator due to constant value as argument"
  std::shuffle_order engine23(seed);

  // [CXX-A1000]: 22 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  std::shuffle_order engine24(time(&t));

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to default argument"
  engine21.seed();

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine21.seed(1);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to constant value as argument"
  engine21.seed(seed);

  // [CXX-A1000]: 3 "Predicatable values from randon number generator due to predictable argument of type `time_t`"
  engine21.seed(time(&t));

}

struct A {
  A(int _ = 0);
  void seed(int _ = 0);
};

void g() {
  int n = 1;
  std::default_random_engine engine1(n);
  std::mt19937 engine2(n);
  std::ranlux24_base engine3(n);
  std::ranlux24 engine4(n);
  std::independent_bits engine5(n);
  std::shuffle_order engine6(n);

  std::random_device dev;
  std::default_random_engine engine7(dev());
  std::mt19937 engine8(dev());
  std::ranlux24_base engine9(dev());
  std::ranlux24 engine10(dev());
  std::independent_bits engine11(dev());
  std::shuffle_order engine12(dev());

  A a1;
  A a2(1);
  a1.seed();
  a1.seed(1);
  a1.seed(n);
}
