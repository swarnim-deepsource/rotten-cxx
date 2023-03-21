// RUN: %check_clang_tidy %s cert-flp30-c %t

float g(void);

void func(void) {
  // [CXX-W2024]: 3 "Loop increment expression is of type `float`"
  for (float x = 0.1f; x <= 1.0f; x += 0.1f) {}

  float f = 1.0f;
  // [CXX-W2024]: 3 "Loop increment expression is of type `float`"
  for (; f > 0; --f) {}

  // [CXX-W2024]: 3 "Loop increment expression is of type `float`"
  for (;;g()) {}

  for (int i = 0; i < 10; i += 1.0f) {}

  for (int i = 0; i < 10; ++i) {}
}
