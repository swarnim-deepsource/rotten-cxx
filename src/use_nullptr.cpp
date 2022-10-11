#include <memory>

void func_with_ptr(int dc1, void *ptr, int dc2) {}

int main(void) {
  int *x = 0;
  /// problem: passing literal 0 to func_with_ptr for void pointer
  func_with_ptr(0, 0, 1);

  // problem: passing literal NULL to func_with_ptr for void pointer
  // func_with_ptr(0, NULL, 1);

  /// all cool
  func_with_ptr(0, nullptr, 0);
}

/*
 * An example for function returning 0 or NULL
 *
 * Another example involving a method
 *
 * Another example with templates
 */

/*
 * impl autofix
 *  - one in same line
 *  - one in seperate line
 *
 * ignore lib code
 *
 * multiplex checks
 *
 * impl two checks
 *
 * add checks for objective c adn cpp
 */
