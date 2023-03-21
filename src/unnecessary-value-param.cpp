// scatr-check: CXX-P2009
struct ExpensiveToCopyType {
  const ExpensiveToCopyType & constReference() const {
    return *this;
  }
  void nonConstMethod();
  virtual ~ExpensiveToCopyType();
};

void mutate(ExpensiveToCopyType &);
void mutate(ExpensiveToCopyType *);
void useAsConstReference(const ExpensiveToCopyType &);
void useByValue(ExpensiveToCopyType);

template <class T> class Vector {
 public:
  using iterator = T*;
  using const_iterator = const T*;

  Vector(const Vector&);
  Vector& operator=(const Vector&);

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
};

// This class simulates std::pair<>. It is trivially copy constructible
// and trivially destructible, but not trivially copy assignable.
class SomewhatTrivial {
 public:
  SomewhatTrivial();
  SomewhatTrivial(const SomewhatTrivial&) = default;
  ~SomewhatTrivial() = default;
  SomewhatTrivial& operator=(const SomewhatTrivial&);
};

struct MoveOnlyType {
  MoveOnlyType(const MoveOnlyType &) = delete;
  MoveOnlyType(MoveOnlyType &&) = default;
  ~MoveOnlyType();
  void constMethod() const;
};

struct ExpensiveMovableType {
  ExpensiveMovableType();
  // [CXX-P2008]
  ExpensiveMovableType(ExpensiveMovableType &&);
  ExpensiveMovableType(const ExpensiveMovableType &) = default;
  ExpensiveMovableType &operator=(const ExpensiveMovableType &) = default;
  // [CXX-P2008]
  ExpensiveMovableType &operator=(ExpensiveMovableType &&);
  ~ExpensiveMovableType();
};

void positiveExpensiveConstValue(const ExpensiveToCopyType Obj);
// [CXX-P2009]
void positiveExpensiveConstValue(const ExpensiveToCopyType Obj) {
}

void positiveExpensiveValue(ExpensiveToCopyType Obj);
// [CXX-P2009]
void positiveExpensiveValue(ExpensiveToCopyType Obj) {
  Obj.constReference();
  useAsConstReference(Obj);
  // [CXX-P2005]
  auto Copy = Obj;
  useByValue(Obj);
}

// [CXX-P2009]
void positiveVector(Vector<ExpensiveToCopyType> V) {
  for (const auto& Obj : V) {
    useByValue(Obj);
  }
}

void positiveWithComment(const ExpensiveToCopyType /* important */ S);
// [CXX-P2009]
void positiveWithComment(const ExpensiveToCopyType /* important */ S) {
}

// [CXX-P2009]
void positiveUnnamedParam(const ExpensiveToCopyType) {
}

void positiveAndNegative(const ExpensiveToCopyType ConstCopy, const ExpensiveToCopyType& ConstRef, ExpensiveToCopyType Copy);
// [CXX-P2009]
void positiveAndNegative(const ExpensiveToCopyType ConstCopy, const ExpensiveToCopyType& ConstRef, ExpensiveToCopyType Copy) {
}

struct PositiveConstValueConstructor {
  // [CXX-P2009]
  PositiveConstValueConstructor(const ExpensiveToCopyType ConstCopy) {}
};

// [CXX-P2009]
template <typename T> void templateWithNonTemplatizedParameter(const ExpensiveToCopyType S, T V) {
}

void instantiated() {
  templateWithNonTemplatizedParameter(ExpensiveToCopyType(), ExpensiveToCopyType());
  templateWithNonTemplatizedParameter(ExpensiveToCopyType(), 5);
}

template <typename T> void negativeTemplateType(const T V) {
}

void negativeArray(const ExpensiveToCopyType[]) {
}

void negativePointer(ExpensiveToCopyType* Obj) {
}

void negativeConstPointer(const ExpensiveToCopyType* Obj) {
}

void negativeConstReference(const ExpensiveToCopyType& Obj) {
}

void negativeReference(ExpensiveToCopyType& Obj) {
}

void negativeUniversalReference(ExpensiveToCopyType&& Obj) {
}

void negativeSomewhatTrivialConstValue(const SomewhatTrivial Somewhat) {
}

void negativeSomewhatTrivialValue(SomewhatTrivial Somewhat) {
}

void negativeConstBuiltIn(const int I) {
}

void negativeValueBuiltIn(int I) {
}

void negativeValueIsMutatedByReference(ExpensiveToCopyType Obj) {
  mutate(Obj);
}

void negativeValueIsMutatatedByPointer(ExpensiveToCopyType Obj) {
  mutate(&Obj);
}

void negativeValueIsReassigned(ExpensiveToCopyType Obj) {
  Obj = ExpensiveToCopyType();
}

void negativeValueNonConstMethodIsCalled(ExpensiveToCopyType Obj) {
  Obj.nonConstMethod();
}

struct PositiveValueUnusedConstructor {
  // [CXX-P2009]
  PositiveValueUnusedConstructor(ExpensiveToCopyType Copy) {}
};

struct PositiveValueCopiedConstructor {
  // [CXX-P2009]
  PositiveValueCopiedConstructor(ExpensiveToCopyType Copy) : Field(Copy) {}
  ExpensiveToCopyType Field;
};

struct PositiveValueMovableConstructor {
  // [CXX-P2009]
  PositiveValueMovableConstructor(ExpensiveMovableType Copy) : Field(Copy) {}
  ExpensiveMovableType Field;
};

struct NegativeValueMovedConstructor {
  NegativeValueMovedConstructor(ExpensiveMovableType Copy) : Field(static_cast<ExpensiveMovableType &&>(Copy)) {}
  ExpensiveMovableType Field;
};

template <typename T>
struct Container {
  typedef const T & const_reference;
};

void NegativeTypedefParam(const Container<ExpensiveToCopyType>::const_reference Param) {
}

#define UNNECESSARY_VALUE_PARAM_IN_MACRO_BODY()         \
  void inMacro(const ExpensiveToCopyType T) {           \
  }                                                     \
// Ensure fix is not applied.

UNNECESSARY_VALUE_PARAM_IN_MACRO_BODY()

#define UNNECESSARY_VALUE_PARAM_IN_MACRO_ARGUMENT(ARGUMENT)     \
  ARGUMENT

UNNECESSARY_VALUE_PARAM_IN_MACRO_ARGUMENT(void inMacroArgument(const ExpensiveToCopyType InMacroArg) {})

struct VirtualMethod {
  virtual ~VirtualMethod() {}
  virtual void handle(ExpensiveToCopyType T) const = 0;
};

struct NegativeOverriddenMethod : public VirtualMethod {
  void handle(ExpensiveToCopyType Overridden) const {}
};

struct VirtualMethodWarningOnly {
// [CXX-P2009]
  virtual void methodWithExpensiveValueParam(ExpensiveToCopyType T) {}
  virtual ~VirtualMethodWarningOnly() {}
};

struct PositiveNonVirualMethod {
// [CXX-P2009]
  void method(const ExpensiveToCopyType T) {}
};

struct NegativeDeletedMethod {
  ~NegativeDeletedMethod() {}
  NegativeDeletedMethod& operator=(NegativeDeletedMethod N) = delete;
};

void NegativeMoveOnlyTypePassedByValue(MoveOnlyType M) {
  M.constMethod();
}

// [CXX-P2009]: "The `E` parameter is copied on each invocation, consider making it a reference"
void PositiveMoveOnCopyConstruction(ExpensiveMovableType E) {
  // [CXX-P2005]
  auto F = E;
}

// [CXX-P2009]
void PositiveConstRefNotMoveSinceReferencedMultipleTimes(ExpensiveMovableType E) {
  // [CXX-P2005]
  auto F = E;
  // [CXX-P2005]
  auto G = E;
}

// [CXX-P2009]
void PositiveMoveOnCopyAssignment(ExpensiveMovableType E) {
  ExpensiveMovableType F;
  F = E;
}

struct NotCopyAssigned {
  NotCopyAssigned &operator=(const ExpensiveMovableType &);
};

// [CXX-P2009]
void PositiveNoMoveForNonCopyAssigmentOperator(ExpensiveMovableType E) {
  NotCopyAssigned N;
  N = E;
}

// The argument could be moved but is not since copy statement is inside a loop.
// [CXX-P2009]
void PositiveNoMoveInsideLoop(ExpensiveMovableType E) {
  for (;;) {
    // [CXX-P2005]
    auto F = E;
  }
}

// [CXX-P2009]
void PositiveConstRefNotMoveConstructible(ExpensiveToCopyType T) {
  // [CXX-P2005]
  auto U = T;
}

// [CXX-P2009]
void PositiveConstRefNotMoveAssignable(ExpensiveToCopyType A) {
  ExpensiveToCopyType B;
  B = A;
}

// Case where parameter in declaration is already const-qualified but not in
// implementation. Make sure a second 'const' is not added to the declaration.
void PositiveConstDeclaration(const ExpensiveToCopyType A);
// [CXX-P2009]
void PositiveConstDeclaration(ExpensiveToCopyType A) {}

void PositiveNonConstDeclaration(ExpensiveToCopyType A);
// [CXX-P2009]: "The const qualified `A` parameter is copied on each invocation, consider making it a const reference"
void PositiveNonConstDeclaration(const ExpensiveToCopyType A) {
}

// [CXX-P2009]
void PositiveOnlyMessageAsReferencedInCompilationUnit(ExpensiveToCopyType A) {}

void ReferenceFunctionOutsideOfCallExpr() {
  void (*ptr)(ExpensiveToCopyType) = &PositiveOnlyMessageAsReferencedInCompilationUnit;
}

// [CXX-P2009]
void PositiveMessageAndFixAsFunctionIsCalled(ExpensiveToCopyType A) {
}

void ReferenceFunctionByCallingIt() {
  PositiveMessageAndFixAsFunctionIsCalled(ExpensiveToCopyType());
}

// Virtual method overrides of dependent types cannot be recognized unless they
// are marked as override or final. Test that check is not triggered on methods
// marked with override or final.
template <typename T>
struct NegativeDependentTypeInterface {
  virtual void Method(ExpensiveToCopyType E) = 0;
};

template <typename T>
struct NegativeOverrideImpl : public NegativeDependentTypeInterface<T> {
  void Method(ExpensiveToCopyType E) override {}
};

template <typename T>
struct NegativeFinalImpl : public NegativeDependentTypeInterface<T> {
  void Method(ExpensiveToCopyType E) final {}
};

struct PositiveConstructor {
// [CXX-P2009]
  PositiveConstructor(ExpensiveToCopyType E) : E(E) {}

  ExpensiveToCopyType E;
};

struct NegativeUsingConstructor : public PositiveConstructor {
  using PositiveConstructor::PositiveConstructor;
};

void fun() {
  ExpensiveToCopyType E;
  NegativeUsingConstructor S(E);
}

template<typename T>
void templateFunction(T) {
}

template<>
// [CXX-P2009]
void templateFunction<ExpensiveToCopyType>(ExpensiveToCopyType E) {
  E.constReference();
}

template <class T>
// [CXX-P2009]
T templateSpecializationFunction(ExpensiveToCopyType E) {
  return T();
}

template <>
// [CXX-P2009]
bool templateSpecializationFunction(ExpensiveToCopyType E) {
  return true;
}

template <>
// [CXX-P2009]
int templateSpecializationFunction(ExpensiveToCopyType E) {
  return 0;
}
