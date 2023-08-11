// scatr-check: CXX-W2065

// We need NULL macro, but some buildbots don't like including <cstddef> header
// This is a portable way of getting it to work
#undef NULL
#define NULL 0L

template<typename T>
void functionTaking(T);

struct Struct {
  int member;
};


////////// Implicit conversion from bool.

void implicitConversionFromBoolSimpleCases() {
  bool boolean = true;

  functionTaking<bool>(boolean);

  // [CXX-W2065]
  functionTaking<int>(boolean);

  // [CXX-W2065]
  functionTaking<unsigned long>(boolean);

  // [CXX-W2065]
  functionTaking<char>(boolean);

  // [CXX-W2065]
  functionTaking<float>(boolean);

  // [CXX-W2065]
  functionTaking<double>(boolean);
}

float implicitConversionFromBoolInReturnValue() {
  bool boolean = false;
  // [CXX-W2065]
  return boolean;
}

void implicitConversionFromBoolInSingleBoolExpressions(bool b1, bool b2) {
  bool boolean = true;
  boolean = b1 ^ b2;
  boolean = b1 && b2;
  boolean |= !b1 || !b2;
  boolean &= b1;
  boolean = b1 == true;
  boolean = b2 != false;

  // [CXX-W2065]
  int integer = boolean - 3;

  // [CXX-W2065]
  float floating = boolean / 0.3f;

  // [CXX-W2065]
  char character = boolean;
}

void implicitConversionFromBoollInComplexBoolExpressions() {
  bool boolean = true;
  bool anotherBoolean = false;

  // [CXX-W2065]
  int integer = boolean && anotherBoolean;

  // [CXX-W2065]
  unsigned long unsignedLong = (! boolean) + 4ul;

  // [CXX-W2065]
  float floating = (boolean || anotherBoolean) * 0.3f;

  // [CXX-W2065]
  double doubleFloating = (boolean && (anotherBoolean || boolean)) * 0.3;
}

void implicitConversionFromBoolLiterals() {
  // [CXX-W2065]
  functionTaking<int>(true);

  // [CXX-W2065]
  functionTaking<unsigned long>(false);

  // [CXX-W2065]
  functionTaking<signed char>(true);

  // [CXX-W2065]
  functionTaking<float>(false);

  // [CXX-W2065]
  functionTaking<double>(true);
}

void implicitConversionFromBoolInComparisons() {
  bool boolean = true;
  int integer = 0;

  // [CXX-W2065]
  functionTaking<bool>(boolean == integer);

  // [CXX-W2065]
  functionTaking<bool>(integer != boolean);
}

void ignoreBoolComparisons() {
  bool boolean = true;
  bool anotherBoolean = false;

  functionTaking<bool>(boolean == anotherBoolean);
  functionTaking<bool>(boolean != anotherBoolean);
}

void ignoreExplicitCastsFromBool() {
  bool boolean = true;

  int integer = static_cast<int>(boolean) + 3;
  float floating = static_cast<float>(boolean) * 0.3f;
  char character = static_cast<char>(boolean);
}

void ignoreImplicitConversionFromBoolInMacroExpansions() {
  bool boolean = true;

  #define CAST_FROM_BOOL_IN_MACRO_BODY boolean + 3
  int integerFromMacroBody = CAST_FROM_BOOL_IN_MACRO_BODY;

  #define CAST_FROM_BOOL_IN_MACRO_ARGUMENT(x) x + 3
  int integerFromMacroArgument = CAST_FROM_BOOL_IN_MACRO_ARGUMENT(boolean);
}

namespace ignoreImplicitConversionFromBoolInTemplateInstantiations {

template<typename T>
void templateFunction() {
  bool boolean = true;
  T uknownType = boolean + 3;
}

void useOfTemplateFunction() {
  templateFunction<int>();
}

} // namespace ignoreImplicitConversionFromBoolInTemplateInstantiations

////////// Implicit conversions to bool.

void implicitConversionToBoolSimpleCases() {
  int integer = 10;
  // [CXX-W2065]
  functionTaking<bool>(integer);

  unsigned long unsignedLong = 10;
  // [CXX-W2065]
  functionTaking<bool>(unsignedLong);

  float floating = 0.0f;
  // [CXX-W2065]
  functionTaking<bool>(floating);

  double doubleFloating = 1.0f;
  // [CXX-W2065]
  functionTaking<bool>(doubleFloating);

  signed char character = 'a';
  // [CXX-W2065]
  functionTaking<bool>(character);

  int* pointer = nullptr;
  // [CXX-W2065]
  functionTaking<bool>(pointer);

  auto pointerToMember = &Struct::member;
  // [CXX-W2065]
  functionTaking<bool>(pointerToMember);
}

void implicitConversionToBoolInSingleExpressions() {
  int integer = 10;
  // [CXX-W2065]
  bool boolComingFromInt = integer;

  float floating = 10.0f;
  // [CXX-W2065]
  bool boolComingFromFloat = floating;

  signed char character = 'a';
  // [CXX-W2065]
  bool boolComingFromChar = character;

  int* pointer = nullptr;
  // [CXX-W2065]
  bool boolComingFromPointer = pointer;
}

void implicitConversionToBoolInComplexExpressions() {
  bool boolean = true;

  int integer = 10;
  int anotherInteger = 20;
  // [CXX-W2065]
  bool boolComingFromInteger = integer + anotherInteger;

  float floating = 0.2f;
  // [CXX-W2065]
  bool boolComingFromFloating = floating - 0.3f || boolean;

  double doubleFloating = 0.3;
  // [CXX-W2065]
  bool boolComingFromDoubleFloating = (doubleFloating - 0.4) && boolean;
}

void implicitConversionInNegationExpressions() {
  int integer = 10;
  // [CXX-W2065]
  bool boolComingFromNegatedInt = !integer;

  float floating = 10.0f;
  // [CXX-W2065]
  bool boolComingFromNegatedFloat = ! floating;

  signed char character = 'a';
  // [CXX-W2065]
  bool boolComingFromNegatedChar = (! character);

  int* pointer = nullptr;
  // [CXX-W2065]
  bool boolComingFromNegatedPointer = not pointer;
}

void implicitConversionToBoolInControlStatements() {
  int integer = 10;
  // [CXX-W2065]
  if (integer) {}

  long int longInteger = 0.2f;
  // [CXX-W2065]
  for (;longInteger;) {}

  float floating = 0.3f;
  // [CXX-W2065]
  while (floating) {}

  double doubleFloating = 0.4;
  // [CXX-W2065]
  do {} while (doubleFloating);
}

bool implicitConversionToBoolInReturnValue() {
  float floating = 1.0f;
  // [CXX-W2065]
  return floating;
}

void implicitConversionToBoolFromLiterals() {
  // [CXX-W2065]
  functionTaking<bool>(0);

  // [CXX-W2065]
  functionTaking<bool>(1);

  // [CXX-W2065]
  functionTaking<bool>(2ul);


  // [CXX-W2065]
  functionTaking<bool>(0.0f);

  // [CXX-W2065]
  functionTaking<bool>(1.0f);

  // [CXX-W2065]
  functionTaking<bool>(2.0);


  // [CXX-W2065]
  functionTaking<bool>('\0');

  // [CXX-W2065]
  functionTaking<bool>('a');


  // [CXX-W2065]
  functionTaking<bool>("");

  // [CXX-W2065]
  functionTaking<bool>("abc");

  // [CXX-W2065]
  functionTaking<bool>(NULL);
}

void implicitConversionToBoolFromUnaryMinusAndZeroLiterals() {
  // [CXX-W2065]
  functionTaking<bool>(-0);

  // [CXX-W2065]
  functionTaking<bool>(-0.0f);

  // [CXX-W2065]
  functionTaking<bool>(-0.0);
}

void implicitConversionToBoolInWithOverloadedOperators() {
  struct UserStruct {
    int operator()(int x) { return x; }
    int operator+(int y) { return y; }
  };

  UserStruct s;

  // [CXX-W2065]
  functionTaking<bool>(s(0));

  // [CXX-W2065]
  functionTaking<bool>(s + 2);
}

int functionReturningInt();
int* functionReturningPointer();

void ignoreImplicitConversionToBoolWhenDeclaringVariableInControlStatements() {
  if (int integer = functionReturningInt()) {}

  while (int* pointer = functionReturningPointer()) {}
}

void ignoreExplicitCastsToBool() {
  int integer = 10;
  bool boolComingFromInt = static_cast<bool>(integer);

  float floating = 10.0f;
  bool boolComingFromFloat = static_cast<bool>(floating);

  char character = 'a';
  bool boolComingFromChar = static_cast<bool>(character);

  int* pointer = nullptr;
  bool booleanComingFromPointer = static_cast<bool>(pointer);
}

void ignoreImplicitConversionToBoolInMacroExpansions() {
  int integer = 3;

  #define CAST_TO_BOOL_IN_MACRO_BODY integer && false
  bool boolFromMacroBody = CAST_TO_BOOL_IN_MACRO_BODY;

  #define CAST_TO_BOOL_IN_MACRO_ARGUMENT(x) x || true
  bool boolFromMacroArgument = CAST_TO_BOOL_IN_MACRO_ARGUMENT(integer);
}

namespace ignoreImplicitConversionToBoolInTemplateInstantiations {

template<typename T>
void templateFunction() {
  T unknownType = 0;
  bool boolean = unknownType;
}

void useOfTemplateFunction() {
  templateFunction<int>();
}

} // namespace ignoreImplicitConversionToBoolInTemplateInstantiations

namespace ignoreUserDefinedConversionOperator {

struct StructWithUserConversion {
  operator bool();
};

void useOfUserConversion() {
  StructWithUserConversion structure;
  functionTaking<bool>(structure);
}

} // namespace ignoreUserDefinedConversionOperator

namespace ignore_1bit_bitfields {

struct S {
  int a;
  int b : 1;
  int c : 2;

  // [CXX-W2065]
  S(bool a, bool b, bool c) : a(a), b(b), c(c) {}
};

bool f(S& s) {
  // [CXX-W2065]
  functionTaking<bool>(s.a);
  functionTaking<bool>(s.b);
  // [CXX-W2065]
  s.a = true;
  s.b = true;
  // [CXX-W2065]
  s.c = true;
  // [CXX-W2065]
  functionTaking<bool>(s.c);
}

} // namespace ignore_1bit_bitfields
