// RUN: %check_clang_tidy %s cppcoreguidelines-avoid-non-const-global-variables
// %t

// [CXX-W2009]: "Non-const variable `nonConstInt` placed in global scope"
int nonConstInt = 0;

// [CXX-W2011]: "Variable `nonConstIntReference` provides access to non-const
// object"
int &nonConstIntReference = nonConstInt;

// [CXX-W2009]: "Non-const variable `pointerToNonConstInt` placed in global
// scope" [CXX-W2011]: "Variable `pointerToNonConstInt` provides access to
// non-const object"
int *pointerToNonConstInt = &nonConstInt;

// [CXX-W2011]: "Variable `constPointerToNonConstInt` provides access to
// non-const object"
int *const constPointerToNonConstInt = &nonConstInt;

namespace namespace_name {
// [CXX-W2009]
int nonConstNamespaceInt = 0;

const int constNamespaceInt = 0;
} // namespace namespace_name

const int constInt = 0;

// [CXX-W2009]
const int *pointerToConstInt = &constInt;

const int *const constPointerToConstInt = &constInt;

const int &constReferenceToConstInt = constInt;

constexpr int constexprInt = 0;

int function() {
  int nonConstReturnValue = 0;
  return nonConstReturnValue;
}

namespace {
// [CXX-W2009]
int nonConstAnonymousNamespaceInt = 0;
} // namespace

class DummyClass {
public:
  int nonConstPublicMemberVariable = 0;
  const int constPublicMemberVariable = 0;

private:
  int nonConstPrivateMemberVariable = 0;
  const int constPrivateMemberVariable = 0;
};

// [CXX-W2009]
DummyClass nonConstClassInstance;

// [CXX-W2009]; [CXX-W2011]
DummyClass *pointerToNonConstDummyClass = &nonConstClassInstance;

// [CXX-W2011]
DummyClass &referenceToNonConstDummyClass = nonConstClassInstance;

// [CXX-W2009]; [CXX-W2011]
int *nonConstPointerToMember =
    &nonConstClassInstance.nonConstPublicMemberVariable;

// [CXX-W2011]
int *const constPointerToNonConstMember =
    &nonConstClassInstance.nonConstPublicMemberVariable;

const DummyClass constClassInstance;

// [CXX-W2011]
DummyClass *const constPointerToNonConstDummyClass = &nonConstClassInstance;

// [CXX-W2009]
const DummyClass *nonConstPointerToConstDummyClass = &constClassInstance;

const DummyClass *const constPointerToConstDummyClass = &constClassInstance;

const int *const constPointerToConstMember =
    &constClassInstance.nonConstPublicMemberVariable;

const DummyClass &constReferenceToDummyClass = constClassInstance;

namespace namespace_name {
// [CXX-W2009]
DummyClass nonConstNamespaceClassInstance;

const DummyClass constDummyClassInstance;
} // namespace namespace_name

// CHECKING FOR NON-CONST GLOBAL ENUM /////////////////////////////////////////
enum DummyEnum { first, second };

// [CXX-W2009]
DummyEnum nonConstDummyEnumInstance = DummyEnum::first;

// [CXX-W2009]; [CXX-W2011]
DummyEnum *pointerToNonConstDummyEnum = &nonConstDummyEnumInstance;

// [CXX-W2011]
DummyEnum &referenceToNonConstDummyEnum = nonConstDummyEnumInstance;

// [CXX-W2011]
DummyEnum *const constPointerToNonConstDummyEnum = &nonConstDummyEnumInstance;

const DummyEnum constDummyEnumInstance = DummyEnum::first;

// [CXX-W2009]
const DummyEnum *nonConstPointerToConstDummyEnum = &constDummyEnumInstance;

const DummyEnum *const constPointerToConstDummyEnum = &constDummyEnumInstance;

const DummyEnum &referenceToConstDummyEnum = constDummyEnumInstance;

namespace namespace_name {
// [CXX-W2009]
DummyEnum nonConstNamespaceEnumInstance = DummyEnum::first;

const DummyEnum constNamespaceEnumInstance = DummyEnum::first;
} // namespace namespace_name

namespace {
// [CXX-W2009]
DummyEnum nonConstAnonymousNamespaceEnumInstance = DummyEnum::first;
} // namespace

// CHECKING FOR NON-CONST GLOBAL STRUCT ///////////////////////////////////////
struct DummyStruct {
public:
  int structIntElement = 0;
  const int constStructIntElement = 0;

private:
  int privateStructIntElement = 0;
};

// [CXX-W2009]
DummyStruct nonConstDummyStructInstance;

// [CXX-W2009]
// [CXX-W2011]
DummyStruct *pointerToNonConstDummyStruct = &nonConstDummyStructInstance;

// [CXX-W2011]
DummyStruct &referenceToNonConstDummyStruct = nonConstDummyStructInstance;

// [CXX-W2011]
DummyStruct *const constPointerToNonConstDummyStruct =
    &nonConstDummyStructInstance;

const DummyStruct constDummyStructInstance;

// [CXX-W2009]
const DummyStruct *nonConstPointerToConstDummyStruct =
    &constDummyStructInstance;

const DummyStruct *const constPointerToConstDummyStruct =
    &constDummyStructInstance;

const DummyStruct &referenceToConstDummyStruct = constDummyStructInstance;

namespace namespace_name {
// [CXX-W2009]
DummyStruct nonConstNamespaceDummyStructInstance;

const DummyStruct constNamespaceDummyStructInstance;
} // namespace namespace_name

namespace {
// [CXX-W2009]
DummyStruct nonConstAnonymousNamespaceStructInstance;
} // namespace

// CHECKING FOR NON-CONST GLOBAL UNION ////////////////////////////////////////
union DummyUnion {
  int unionInteger;
  char unionChar;
};

// [CXX-W2009]
DummyUnion nonConstUnionIntInstance = {0x0};

// [CXX-W2009]; [CXX-W2011]
DummyUnion *nonConstPointerToNonConstUnionInt = &nonConstUnionIntInstance;

// [CXX-W2011]
DummyUnion *const constPointerToNonConstUnionInt = &nonConstUnionIntInstance;

// [CXX-W2011]
DummyUnion &referenceToNonConstUnionInt = nonConstUnionIntInstance;

const DummyUnion constUnionIntInstance = {0x0};

// [CXX-W2009]
const DummyUnion *nonConstPointerToConstUnionInt = &constUnionIntInstance;

const DummyUnion *const constPointerToConstUnionInt = &constUnionIntInstance;

const DummyUnion &referenceToConstUnionInt = constUnionIntInstance;

namespace namespace_name {
// [CXX-W2009]
DummyUnion nonConstNamespaceDummyUnionInstance;

const DummyUnion constNamespaceDummyUnionInstance = {0x0};
} // namespace namespace_name

namespace {
// [CXX-W2009]
DummyUnion nonConstAnonymousNamespaceUnionInstance = {0x0};
} // namespace

// CHECKING FOR NON-CONST GLOBAL FUNCTION POINTER /////////////////////////////
int dummyFunction() { return 0; }

typedef int (*functionPointer)();
// [CXX-W2009]
functionPointer fp1 = &dummyFunction;

typedef int (*const functionConstPointer)();
// [CXX-W2009]
functionPointer fp2 = &dummyFunction;

// CHECKING FOR NON-CONST GLOBAL TEMPLATE VARIABLE ////////////////////////////
template <class T> constexpr T templateVariable = T(0L);

// CHECKING AGAINST FALSE POSITIVES INSIDE FUNCTION SCOPE /////////////////////
int main() {
  for (int i = 0; i < 3; ++i) {
    static int staticNonConstLoopVariable = 42;
    int nonConstLoopVariable = 42;
    nonConstInt = nonConstLoopVariable + i + staticNonConstLoopVariable;
  }
}