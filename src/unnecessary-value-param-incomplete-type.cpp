// RUN: %check_clang_tidy %s performance-unnecessary-value-param %t -- -fix-errors

// Ensure that incomplete types result in an error from the frontend and not a
// clang-tidy diagnostic about IncompleteType being expensive to copy.
struct IncompleteType;
void NegativeForIncompleteType(IncompleteType I) {
}

