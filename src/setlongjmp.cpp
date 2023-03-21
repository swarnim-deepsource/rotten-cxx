// scatr-check: CXX-W2015
// RUN: %check_clang_tidy %s cert-err52-cpp %t

typedef void *jmp_buf;
extern int __setjmpimpl(jmp_buf);
#define setjmp(x) __setjmpimpl(x)
[[noreturn]] extern void longjmp(jmp_buf, int);

namespace std {
using ::jmp_buf;
using ::longjmp;
}

const static jmp_buf env = 0;
void g() {
  // [CXX-W2015]: "Using `longjmp` in place of throw-catch idioms"
  std::longjmp(env, 1);
  // [CXX-W2015]: "Using `longjmp` in place of throw-catch idioms"
  ::longjmp(env, 1);
  // [CXX-W2015]: "Using `longjmp` in place of throw-catch idioms"
  longjmp(env, 1);
}

void f() {
  // [CXX-W2015]: "Using `setjmp` in place of throw-catch idioms"
  (void)setjmp(env);
}
