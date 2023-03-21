// scatr-check: CXX-P2001
// RUN: %check_clang_tidy %s performance-type-promotion-in-math-fn %t

double acos(double);
double acosh(double);
double asin(double);
double asinh(double);
double atan2(double, double);
double atan(double);
double atanh(double);
double cbrt(double);
double ceil(double);
double copysign(double, double);
double cos(double);
double cosh(double);
double erfc(double);
double erf(double);
double exp2(double);
double exp(double);
double expm1(double);
double fabs(double);
double fdim(double, double);
double floor(double);
double fma(double, double, double);
double fmax(double, double);
double fmin(double, double);
double fmod(double, double);
double frexp(double, int*);
double hypot(double, double);
double ilogb(double);
double ldexp(double, double);
double lgamma(double);
long long llrint(double);
double log10(double);
double log1p(double);
double log2(double);
double logb(double);
double log(double);
long lrint(double);
double modf(double);
double nearbyint(double);
double nextafter(double, double);
double nexttoward(double, long double);
double pow(double, double);
double remainder(double, double);
double remquo(double, double, int*);
double rint(double);
double round(double);
double scalbln(double, long);
double scalbn(double, int);
double sin(double);
double sinh(double);
double sqrt(double);
double tan(double);
double tanh(double);
double tgamma(double);
double trunc(double);
long long llround(double);
long lround(double);

void check_all_fns() {
    float a, b, c;
    int i;
    long l;
    int* int_ptr;

    // [CXX-P2001]: 5 "Call to `acos(..)` implicitly promotes `float` to `double`"
    acos(a);
    // [CXX-P2001]: 5 "Call to `acosh(..)` implicitly promotes `float` to `double`"
    acosh(a);
    // [CXX-P2001]: 5 "Call to `asin(..)` implicitly promotes `float` to `double`"
    asin(a);
    // [CXX-P2001]: 5 "Call to `asinh(..)` implicitly promotes `float` to `double`"
    asinh(a);
    // [CXX-P2001]: 5 "Call to `atan2(..)` implicitly promotes `float` to `double`"
    atan2(a, b);
    // [CXX-P2001]: 5 "Call to `atan(..)` implicitly promotes `float` to `double`"
    atan(a);
    // [CXX-P2001]: 5 "Call to `atanh(..)` implicitly promotes `float` to `double`"
    atanh(a);
    // [CXX-P2001]: 5 "Call to `cbrt(..)` implicitly promotes `float` to `double`"
    cbrt(a);
    // [CXX-P2001]: 5 "Call to `ceil(..)` implicitly promotes `float` to `double`"
    ceil(a);
    // [CXX-P2001]: 5 "Call to `copysign(..)` implicitly promotes `float` to `double`"
    copysign(a, b);
    // [CXX-P2001]: 5 "Call to `cos(..)` implicitly promotes `float` to `double`"
    cos(a);
    // [CXX-P2001]: 5 "Call to `cosh(..)` implicitly promotes `float` to `double`"
    cosh(a);
    // [CXX-P2001]: 5 "Call to `erf(..)` implicitly promotes `float` to `double`"
    erf(a);
    // [CXX-P2001]: 5 "Call to `erfc(..)` implicitly promotes `float` to `double`"
    erfc(a);
    // [CXX-P2001]: 5 "Call to `exp2(..)` implicitly promotes `float` to `double`"
    exp2(a);
    // [CXX-P2001]: 5 "Call to `exp(..)` implicitly promotes `float` to `double`"
    exp(a);
    // [CXX-P2001]: 5 "Call to `expm1(..)` implicitly promotes `float` to `double`"
    expm1(a);
    // [CXX-P2001]: 5 "Call to `fabs(..)` implicitly promotes `float` to `double`"
    fabs(a);
    // [CXX-P2001]: 5 "Call to `fdim(..)` implicitly promotes `float` to `double`"
    fdim(a, b);
    // [CXX-P2001]: 5 "Call to `floor(..)` implicitly promotes `float` to `double`"
    floor(a);
    // [CXX-P2001]: 5 "Call to `fma(..)` implicitly promotes `float` to `double`"
    fma(a, b, c);
    // [CXX-P2001]: 5 "Call to `fmax(..)` implicitly promotes `float` to `double`"
    fmax(a, b);
    // [CXX-P2001]: 5 "Call to `fmin(..)` implicitly promotes `float` to `double`"
    fmin(a, b);
    // [CXX-P2001]: 5 "Call to `fmod(..)` implicitly promotes `float` to `double`"
    fmod(a, b);
    // [CXX-P2001]: 5 "Call to `frexp(..)` implicitly promotes `float` to `double`"
    frexp(a, int_ptr);
    // [CXX-P2001]: 5 "Call to `hypot(..)` implicitly promotes `float` to `double`"
    hypot(a, b);
    // [CXX-P2001]: 5 "Call to `ilogb(..)` implicitly promotes `float` to `double`"
    ilogb(a);
    // [CXX-P2001]: 5 "Call to `ldexp(..)` implicitly promotes `float` to `double`"
    ldexp(a, b);
    // [CXX-P2001]: 5 "Call to `lgamma(..)` implicitly promotes `float` to `double`"
    lgamma(a);
    // [CXX-P2001]: 5 "Call to `llrint(..)` implicitly promotes `float` to `double`"
    llrint(a);
    // [CXX-P2001]: 5 "Call to `llround(..)` implicitly promotes `float` to `double`"
    llround(a);
    // [CXX-P2001]: 5 "Call to `log10(..)` implicitly promotes `float` to `double`"
    log10(a);
    // [CXX-P2001]: 5 "Call to `log1p(..)` implicitly promotes `float` to `double`"
    log1p(a);
    // [CXX-P2001]: 5 "Call to `log2(..)` implicitly promotes `float` to `double`"
    log2(a);
    // [CXX-P2001]: 5 "Call to `log(..)` implicitly promotes `float` to `double`"
    log(a);
    // [CXX-P2001]: 5 "Call to `logb(..)` implicitly promotes `float` to `double`"
    logb(a);
    // [CXX-P2001]: 5 "Call to `lrint(..)` implicitly promotes `float` to `double`"
    lrint(a);
    // [CXX-P2001]: 5 "Call to `lround(..)` implicitly promotes `float` to `double`"
    lround(a);
    // [CXX-P2001]: 5 "Call to `nearbyint(..)` implicitly promotes `float` to `double`"
    nearbyint(a);
    // [CXX-P2001]: 5 "Call to `nextafter(..)` implicitly promotes `float` to `double`"
    nextafter(a, b);
    // [CXX-P2001]: 5 "Call to `nexttoward(..)` implicitly promotes `float` to `double`"
    nexttoward(a, b);
    // [CXX-P2001]: 5 "Call to `pow(..)` implicitly promotes `float` to `double`"
    pow(a, b);
    // [CXX-P2001]: 5 "Call to `remainder(..)` implicitly promotes `float` to `double`"
    remainder(a, b);
    // [CXX-P2001]: 5 "Call to `remquo(..)` implicitly promotes `float` to `double`"
    remquo(a, b, int_ptr);
    // [CXX-P2001]: 5 "Call to `rint(..)` implicitly promotes `float` to `double`"
    rint(a);
    // [CXX-P2001]: 5 "Call to `round(..)` implicitly promotes `float` to `double`"
    round(a);
    // [CXX-P2001]: 5 "Call to `scalbln(..)` implicitly promotes `float` to `double`"
    scalbln(a, l);
    // [CXX-P2001]: 5 "Call to `scalbn(..)` implicitly promotes `float` to `double`"
    scalbn(a, i);
    // [CXX-P2001]: 5 "Call to `sin(..)` implicitly promotes `float` to `double`"
    sin(a);
    // [CXX-P2001]: 5 "Call to `sinh(..)` implicitly promotes `float` to `double`"
    sinh(a);
    // [CXX-P2001]: 5 "Call to `sqrt(..)` implicitly promotes `float` to `double`"
    sqrt(a);
    // [CXX-P2001]: 5 "Call to `tan(..)` implicitly promotes `float` to `double`"
    tan(a);
    // [CXX-P2001]: 5 "Call to `tanh(..)` implicitly promotes `float` to `double`"
    tanh(a);
    // [CXX-P2001]: 5 "Call to `tgamma(..)` implicitly promotes `float` to `double`"
    tgamma(a);
    // [CXX-P2001]: 5 "Call to `trunc(..)` implicitly promotes `float` to `double`"
    trunc(a);
}

// nexttoward/nexttowardf are weird -- the second param is always long double.
// So we warn if the first arg is a float, regardless of what the second arg is.
void check_nexttoward() {
    // [CXX-P2001]: 5 "Call to `nexttoward(..)` implicitly promotes `float` to `double`"
    nexttoward(0.f, 0);
    // [CXX-P2001]: 5 "Call to `nexttoward(..)` implicitly promotes `float` to `double`"
    nexttoward(0.f, 0l);
    // [CXX-P2001]: 5 "Call to `nexttoward(..)` implicitly promotes `float` to `double`"
    nexttoward(0.f, 0.f);
    // [CXX-P2001]: 5 "Call to `nexttoward(..)` implicitly promotes `float` to `double`"
    nexttoward(0.f, 0.);

    // No warnings for these.
    nexttoward(0., 0);
    nexttoward(0., 0.f);
    nexttoward(0., 0.);
}

// The second parameter to scalbn and scalbnf is an int, so we don't care what
// type you pass as that argument; we warn iff the first argument is a float.
void check_scalbn() {
    // [CXX-P2001]: 5 "Call to `scalbn(..)` implicitly promotes `float` to `double`"
    scalbn(0.f, 0);
    // [CXX-P2001]: 5 "Call to `scalbn(..)` implicitly promotes `float` to `double`"
    scalbn(0.f, static_cast<char>(0));

    // No warnings for these.
    scalbn(0., 0);
    scalbn(0., static_cast<char>(0));
}

// scalbln/scalblnf are like scalbn/scalbnf except their second arg is a long.
// Again, doesn't matter what we pass for the second arg; we warn iff the first
// arg is a float.
void check_scalbln() {
    // [CXX-P2001]: 5 "Call to `scalbln(..)` implicitly promotes `float` to `double`"
    scalbln(0.f, 0);
    // [CXX-P2001]: 5 "Call to `scalbln(..)` implicitly promotes `float` to `double`"
    scalbln(0.f, 0l);

    // No warnings for these.
    scalbln(0., 0);
    scalbln(0., 0l);
}

float cosf(float);
double foo(double);         // not a math.h function
float cos(float);           // not a math.h function (wrong signature)
double cos(double, double); // not a math.h function (wrong signature)

namespace std {
void cos(float);
} // namespace std

void check_no_warnings() {
    foo(0.); // no warning because not a math.h function.

    sin(0);        // no warning because arg is an int
    cos(0.);       // no warning because arg is a double
    std::cos(0.f); // no warning because not ::cos.
    cosf(0.f);     // no warning; we expect this to take a float
    cos(0.f);      // does not match the expected signature of ::cos
    cos(0.f, 0.f); // does not match the expected signature of ::cos

    // No warnings because all args are not floats.
    remainder(0., 0.f);
    remainder(0.f, 0.);
    remainder(0, 0.f);
    remainder(0.f, 0);
    fma(0.f, 0.f, 0);
    fma(0.f, 0.f, 0.);
    fma(0.f, 0., 0.f);
    fma(0., 0.f, 0.f);
}
