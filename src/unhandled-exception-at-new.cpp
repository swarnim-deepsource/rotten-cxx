// scatr-check: CXX-W2020

namespace std {
typedef __typeof__(sizeof(0)) size_t;
enum class align_val_t : std::size_t {};
class exception {};
class bad_alloc : public exception {};
class bad_array_new_length : public bad_alloc {};
struct nothrow_t {};
extern const nothrow_t nothrow;
} // namespace std

void *operator new(std::size_t, const std::nothrow_t &) noexcept;
void *operator new(std::size_t, std::align_val_t, const std::nothrow_t &) noexcept;
void *operator new(std::size_t, void *) noexcept;

class A {};
typedef std::bad_alloc badalloc1;
using badalloc2 = std::bad_alloc;
using badalloc3 = std::bad_alloc &;

void *operator new(std::size_t, int, int);
void *operator new(std::size_t, int, int, int) noexcept;

struct ClassSpecificNew {
  void *operator new(std::size_t);
  void *operator new(std::size_t, std::align_val_t);
  void *operator new(std::size_t, int, int) noexcept;
  void *operator new(std::size_t, int, int, int);
};

void silent_S1021(void);

void f1() noexcept {
    // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
  int *I1 = new int;
  try {
    int *I2 = new int;
    try {
      int *I3 = new int;
    } catch (A) {
      silent_S1021();
    }
  } catch (std::bad_alloc) {
    silent_S1021();
  }

  try {
    int *I = new int;
  } catch (std::bad_alloc &) {
    silent_S1021();
  }

  try {
    int *I = new int;
  } catch (const std::bad_alloc &) {
    silent_S1021();
  }

  try {
    int *I = new int;
  } catch (badalloc1) {
    silent_S1021();
  }

  try {
    int *I = new int;
  } catch (badalloc1 &) {
    silent_S1021();
  }

  try {
    int *I = new int;
  } catch (const badalloc1 &) {
    silent_S1021();
  }

  try {
    int *I = new int;
  } catch (badalloc2) {
    silent_S1021();
  }

  try {
    int *I = new int;
  } catch (badalloc3) {
    silent_S1021();
  }

  try {
    // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
    int *I = new int;
  } catch (std::bad_alloc *) {
    silent_S1021();
  }

  try {
    // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
    int *I = new int;
  } catch (A) {
    silent_S1021();
  }
}

void f2() noexcept {
  try {
    int *I = new int;
  } catch (A) {
    silent_S1021();
  } catch (std::bad_alloc) {
    silent_S1021();
  }

  try {
    int *I = new int;
  } catch (...) {
    silent_S1021();
  }

  try {
    int *I = new int;
  } catch (const std::exception &) {
    silent_S1021();
  }

  try {
  // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
    int *I = new int;
  } catch (const std::bad_array_new_length &) {
    silent_S1021();
  }
}

void f_new_nothrow() noexcept {
  int *I1 = new (std::nothrow) int;
  int *I2 = new (static_cast<std::align_val_t>(1), std::nothrow) int;
}

void f_new_placement() noexcept {
  char buf[100];
  int *I = new (buf) int;
}

void f_new_user_defined() noexcept {
  // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
  int *I1 = new (1, 2) int;
  int *I2 = new (1, 2, 3) int;
}

void f_class_specific() noexcept {
  // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
  ClassSpecificNew *N1 = new ClassSpecificNew;
  // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
  ClassSpecificNew *N2 = new (static_cast<std::align_val_t>(1)) ClassSpecificNew;
  ClassSpecificNew *N3 = new (1, 2) ClassSpecificNew;
  // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
  ClassSpecificNew *N4 = new (1, 2, 3) ClassSpecificNew;
}

void f_est_none() {
  int *I = new int;
}

void f_est_noexcept_false() noexcept(false) {
  int *I = new int;
}

void f_est_noexcept_true() noexcept(true) {
  // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
  int *I = new int;
}

void f_est_dynamic_none() throw() {
  // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
  int *I = new int;
}

void f_est_dynamic_1() throw(std::bad_alloc) {
  int *I = new int;
}

void f_est_dynamic_2() throw(A) {
  // the exception specification list is not checked
  int *I = new int;
}

void f_try() noexcept try {
  int *I = new int;
} catch (const std::bad_alloc &) {
  silent_S1021();
}

void f_try_bad() noexcept try {
  // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
  int *I = new int;
} catch (const A &) {
  silent_S1021();
}

void f_embedded_try() noexcept {
  try {
    try {
      int *I = new int;
    } catch (const A &) {
      silent_S1021();
    }
  } catch (const std::bad_alloc &) {
    silent_S1021();
  }
}

template <bool P>
void f_est_noexcept_dependent_unused() noexcept(P) {
  int *I = new int;
}

template <bool P>
void f_est_noexcept_dependent_used() noexcept(P) {
  // [CXX-W2020]: "Exception `std::bad_alloc` is not caught while allocating memeory with `new`"
  int *I = new int;
}

template <class T>
void f_dependent_new() {
  T *X = new T;
}

void f_1() {
  f_est_noexcept_dependent_used<true>();
}
