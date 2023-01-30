// RUN: %check_clang_tidy -std=c++17 %s
// performance-unnecessary-copy-initialization %t

template <typename T> struct Iterator {
  void operator++();
  T &operator*() const;
  bool operator!=(const Iterator &) const;
  typedef const T &const_reference;
};

template <typename T> struct ConstIterator {
  void operator++();
  const T &operator*() const;
  bool operator!=(const ConstIterator &) const;
  typedef const T &const_reference;
};

struct ExpensiveToCopyType {
  ExpensiveToCopyType();
  virtual ~ExpensiveToCopyType();
  const ExpensiveToCopyType &reference() const;
  using ConstRef = const ExpensiveToCopyType &;
  ConstRef referenceWithAlias() const;
  const ExpensiveToCopyType *pointer() const;
  void nonConstMethod();
  bool constMethod() const;
  template <typename A> const A &templatedAccessor() const;
  operator int() const; // Implicit conversion to int.
};

template <typename T> struct Container {
  bool empty() const;
  const T &operator[](int) const;
  const T &operator[](int);
  Iterator<T> begin();
  Iterator<T> end();
  ConstIterator<T> begin() const;
  ConstIterator<T> end() const;
  void nonConstMethod();
  bool constMethod() const;
};

using ExpensiveToCopyContainerAlias = Container<ExpensiveToCopyType>;

struct TrivialToCopyType {
  const TrivialToCopyType &reference() const;
};

struct WeirdCopyCtorType {
  WeirdCopyCtorType();
  WeirdCopyCtorType(const WeirdCopyCtorType &w, bool oh_yes = true);

  void nonConstMethod();
  bool constMethod() const;
};

// [CXX-W2009]: "Non-const variable `global_expensive_to_copy_type` placed in
// global scope"
ExpensiveToCopyType global_expensive_to_copy_type;

const ExpensiveToCopyType &ExpensiveTypeReference();
const ExpensiveToCopyType &freeFunctionWithArg(const ExpensiveToCopyType &);
const ExpensiveToCopyType &
freeFunctionWithDefaultArg(const ExpensiveToCopyType *arg = nullptr);
const TrivialToCopyType &TrivialTypeReference();

void mutate(ExpensiveToCopyType &);
void mutate(ExpensiveToCopyType *);
void useAsConstPointer(const ExpensiveToCopyType *);
void useAsConstReference(const ExpensiveToCopyType &);
void useByValue(ExpensiveToCopyType);

void PositiveFunctionCall() {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = ExpensiveTypeReference();
  AutoAssigned.constMethod();

  // [CXX-P2005]: "Variable `AutoCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoCopyConstructed(ExpensiveTypeReference());
  AutoCopyConstructed.constMethod();

  // [CXX-P2005]: "Variable `VarAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarAssigned = ExpensiveTypeReference();
  // CHECK-MESSAGES: [[@LINE-1]]:29: warning: the const qualified variable
  // 'VarAssigned' CHECK-FIXES:   const ExpensiveToCopyType& VarAssigned =
  // ExpensiveTypeReference();
  VarAssigned.constMethod();

  // [CXX-P2005]: "Variable `VarCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarCopyConstructed(ExpensiveTypeReference());
  // CHECK-MESSAGES: [[@LINE-1]]:29: warning: the const qualified variable
  // 'VarCopyConstructed' CHECK-FIXES: const ExpensiveToCopyType&
  // VarCopyConstructed(ExpensiveTypeReference());
  VarCopyConstructed.constMethod();
}

void PositiveMethodCallConstReferenceParam(const ExpensiveToCopyType &Obj) {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = Obj.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoAssigned' CHECK-FIXES: const auto& AutoAssigned = Obj.reference();
  AutoAssigned.constMethod();

  // [CXX-P2005]: "Variable `AutoCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoCopyConstructed(Obj.reference());
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoCopyConstructed' CHECK-FIXES: const auto&
  // AutoCopyConstructed(Obj.reference());
  AutoCopyConstructed.constMethod();

  // [CXX-P2005]: "Variable `VarAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarAssigned = Obj.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:29: warning: the const qualified variable
  // 'VarAssigned' CHECK-FIXES: const ExpensiveToCopyType& VarAssigned =
  // Obj.reference();
  VarAssigned.constMethod();

  // [CXX-P2005]: "Variable `VarCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarCopyConstructed(Obj.reference());
  // CHECK-MESSAGES: [[@LINE-1]]:29: warning: the const qualified variable
  // 'VarCopyConstructed' CHECK-FIXES: const ExpensiveToCopyType&
  // VarCopyConstructed(Obj.reference());
  VarCopyConstructed.constMethod();
}

void PositiveMethodCallConstParam(const ExpensiveToCopyType Obj) {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = Obj.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoAssigned' CHECK-FIXES: const auto& AutoAssigned = Obj.reference();
  AutoAssigned.constMethod();

  // [CXX-P2005]: "Variable `AutoCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoCopyConstructed(Obj.reference());
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoCopyConstructed' CHECK-FIXES: const auto&
  // AutoCopyConstructed(Obj.reference());
  AutoCopyConstructed.constMethod();

  // [CXX-P2005]: "Variable `VarAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarAssigned = Obj.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:29: warning: the const qualified variable
  // 'VarAssigned' CHECK-FIXES: const ExpensiveToCopyType& VarAssigned =
  // Obj.reference();
  VarAssigned.constMethod();

  // [CXX-P2005]: "Variable `VarCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarCopyConstructed(Obj.reference());
  // CHECK-MESSAGES: [[@LINE-1]]:29: warning: the const qualified variable
  // 'VarCopyConstructed' CHECK-FIXES: const ExpensiveToCopyType&
  // VarCopyConstructed(Obj.reference());
  VarCopyConstructed.constMethod();
}

void PositiveMethodCallConstPointerParam(const ExpensiveToCopyType *const Obj) {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = Obj->reference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoAssigned' CHECK-FIXES: const auto& AutoAssigned = Obj->reference();
  AutoAssigned.constMethod();

  // [CXX-P2005]: "Variable `AutoCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoCopyConstructed(Obj->reference());
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoCopyConstructed' CHECK-FIXES: const auto&
  // AutoCopyConstructed(Obj->reference());
  AutoCopyConstructed.constMethod();

  // [CXX-P2005]: "Variable `VarAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarAssigned = Obj->reference();
  // CHECK-MESSAGES: [[@LINE-1]]:29: warning: the const qualified variable
  // 'VarAssigned' CHECK-FIXES: const ExpensiveToCopyType& VarAssigned =
  // Obj->reference();
  VarAssigned.constMethod();

  // [CXX-P2005]: "Variable `VarCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarCopyConstructed(Obj->reference());
  // CHECK-MESSAGES: [[@LINE-1]]:29: warning: the const qualified variable
  // 'VarCopyConstructed' CHECK-FIXES: const ExpensiveToCopyType&
  // VarCopyConstructed(Obj->reference());
  VarCopyConstructed.constMethod();
}

void PositiveOperatorCallConstReferenceParam(
    const Container<ExpensiveToCopyType> &C) {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = C[42];
  AutoAssigned.constMethod();

  // [CXX-P2005]: "Variable `AutoCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoCopyConstructed(C[42]);
  AutoCopyConstructed.constMethod();

  // [CXX-P2005]: "Variable `VarAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarAssigned = C[42];
  VarAssigned.constMethod();

  // [CXX-P2005]: "Variable `VarCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarCopyConstructed(C[42]);
  VarCopyConstructed.constMethod();
}

void PositiveOperatorCallConstValueParam(
    const Container<ExpensiveToCopyType> C) {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = C[42];
  AutoAssigned.constMethod();

  // [CXX-P2005]: "Variable `AutoCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoCopyConstructed(C[42]);
  AutoCopyConstructed.constMethod();

  // [CXX-P2005]: "Variable `VarAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarAssigned = C[42];
  VarAssigned.constMethod();

  // [CXX-P2005]: "Variable `VarCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarCopyConstructed(C[42]);
  VarCopyConstructed.constMethod();
}

void PositiveOperatorCallConstValueParamAlias(
    const ExpensiveToCopyContainerAlias C) {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = C[42];
  AutoAssigned.constMethod();

  // [CXX-P2005]: "Variable `AutoCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoCopyConstructed(C[42]);
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoCopyConstructed'
  AutoCopyConstructed.constMethod();

  // [CXX-P2005]: "Variable `VarAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarAssigned = C[42];
  // CHECK-MESSAGES: [[@LINE-1]]:29: warning: the const qualified variable
  // 'VarAssigned'
  VarAssigned.constMethod();

  // [CXX-P2005]: "Variable `VarCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  const ExpensiveToCopyType VarCopyConstructed(C[42]);
  // CHECK-MESSAGES: [[@LINE-1]]:29: warning: the const qualified variable
  // 'VarCopyConstructed'
  VarCopyConstructed.constMethod();
}

void PositiveOperatorCallConstValueParam(
    const Container<ExpensiveToCopyType> *C) {
  // Not supported in original check: "Variable `AutoAssigned`, of an
  // expensive-type, is copy-constructer but only used as const."
  const auto AutoAssigned = (*C)[42];
  AutoAssigned.constMethod();

  // Not supported in original check: "Variable `AutoCopyConstructed`, of an
  // expensive-type, is copy-constructer but only used as const."
  const auto AutoCopyConstructed((*C)[42]);
  AutoCopyConstructed.constMethod();

  // Not supported in original check: "Variable `VarAssigned`, of an
  // expensive-type, is copy-constructer but only used as const."
  const ExpensiveToCopyType VarAssigned = C->operator[](42);
  VarAssigned.constMethod();

  // Not supported in original check: "Variable `VarCopyConstructed`, of an
  // expensive-type, is copy-constructer but only used as const."
  const ExpensiveToCopyType VarCopyConstructed(C->operator[](42));
  VarCopyConstructed.constMethod();
}

void PositiveLocalConstValue() {
  const ExpensiveToCopyType Obj;
  // [CXX-P2005]: "Variable `UnnecessaryCopy`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto UnnecessaryCopy = Obj.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'UnnecessaryCopy' CHECK-FIXES: const auto& UnnecessaryCopy =
  // Obj.reference();
  UnnecessaryCopy.constMethod();
}

void PositiveLocalConstRef() {
  const ExpensiveToCopyType Obj;
  const ExpensiveToCopyType &ConstReference = Obj.reference();
  // [CXX-P2005]: "Variable `UnnecessaryCopy`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto UnnecessaryCopy = ConstReference.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'UnnecessaryCopy' CHECK-FIXES: const auto& UnnecessaryCopy =
  // ConstReference.reference();
  UnnecessaryCopy.constMethod();
}

void PositiveLocalConstPointer() {
  const ExpensiveToCopyType Obj;
  const ExpensiveToCopyType *const ConstPointer = &Obj;
  // [CXX-P2005]: "Variable `UnnecessaryCopy`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto UnnecessaryCopy = ConstPointer->reference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'UnnecessaryCopy' CHECK-FIXES: const auto& UnnecessaryCopy =
  // ConstPointer->reference();
  UnnecessaryCopy.constMethod();
}

void NegativeFunctionCallTrivialType() {
  const auto AutoAssigned = TrivialTypeReference();
  const auto AutoCopyConstructed(TrivialTypeReference());
  const TrivialToCopyType VarAssigned = TrivialTypeReference();
  const TrivialToCopyType VarCopyConstructed(TrivialTypeReference());
}

void NegativeStaticLocalVar(const ExpensiveToCopyType &Obj) {
  static const auto StaticVar = Obj.reference();
}

void PositiveFunctionCallExpensiveTypeNonConstVariable() {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  auto AutoAssigned = ExpensiveTypeReference();
  AutoAssigned.constMethod();

  // [CXX-P2005]: "Variable `AutoCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  auto AutoCopyConstructed(ExpensiveTypeReference());
  AutoCopyConstructed.constMethod();

  // [CXX-P2005]: "Variable `VarAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  ExpensiveToCopyType VarAssigned = ExpensiveTypeReference();
  VarAssigned.constMethod();

  // [CXX-P2005]: "Variable `VarCopyConstructed`, of an expensive-type, is
  // copy-constructer but only used as const."
  ExpensiveToCopyType VarCopyConstructed(ExpensiveTypeReference());
  VarCopyConstructed.constMethod();
}

void positiveNonConstVarInCodeBlock(const ExpensiveToCopyType &Obj) {
  {
    // [CXX-P2005]: "Variable `Assigned`, of an expensive-type, is
    // copy-constructer but only used as const."
    auto Assigned = Obj.reference();
    // CHECK-MESSAGES: [[@LINE-1]]:10: warning: the variable 'Assigned'
    // CHECK-FIXES: const auto& Assigned = Obj.reference();
    Assigned.reference();
    useAsConstReference(Assigned);
    useByValue(Assigned);
  }
}

void positiveNonConstVarInCodeBlockWithAlias(const ExpensiveToCopyType &Obj) {
  {
    // [CXX-P2005]: "Variable `Assigned`, of an expensive-type, is
    // copy-constructer but only used as const."
    const ExpensiveToCopyType Assigned = Obj.referenceWithAlias();
    // CHECK-MESSAGES: [[@LINE-1]]:31: warning: the const qualified variable
    // CHECK-FIXES: const ExpensiveToCopyType& Assigned =
    // Obj.referenceWithAlias();
    useAsConstReference(Assigned);
  }
}

void negativeNonConstVarWithNonConstUse(const ExpensiveToCopyType &Obj) {
  {
    auto NonConstInvoked = Obj.reference();
    // CHECK-FIXES: auto NonConstInvoked = Obj.reference();
    NonConstInvoked.nonConstMethod();
  }
  {
    auto Reassigned = Obj.reference();
    // CHECK-FIXES: auto Reassigned = Obj.reference();
    Reassigned = ExpensiveToCopyType();
  }
  {
    auto MutatedByReference = Obj.reference();
    // CHECK-FIXES: auto MutatedByReference = Obj.reference();
    mutate(MutatedByReference);
  }
  {
    auto MutatedByPointer = Obj.reference();
    // CHECK-FIXES: auto MutatedByPointer = Obj.reference();
    mutate(&MutatedByPointer);
  }
}

void PositiveMethodCallNonConstRefNotModified(ExpensiveToCopyType &Obj) {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = Obj.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoAssigned' CHECK-FIXES: const auto& AutoAssigned = Obj.reference();
  AutoAssigned.constMethod();
}

void NegativeMethodCallNonConstRefIsModified(ExpensiveToCopyType &Obj) {
  const auto AutoAssigned = Obj.reference();
  const auto AutoCopyConstructed(Obj.reference());
  const ExpensiveToCopyType VarAssigned = Obj.reference();
  const ExpensiveToCopyType VarCopyConstructed(Obj.reference());
  mutate(&Obj);
}

void PositiveMethodCallNonConstNotModified(ExpensiveToCopyType Obj) {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = Obj.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoAssigned' CHECK-FIXES: const auto& AutoAssigned = Obj.reference();
  AutoAssigned.constMethod();
}

void NegativeMethodCallNonConstValueArgumentIsModified(
    ExpensiveToCopyType Obj) {
  Obj.nonConstMethod();
  const auto AutoAssigned = Obj.reference();
}

void PositiveMethodCallNonConstPointerNotModified(
    ExpensiveToCopyType *const Obj) {
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = Obj->reference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoAssigned' CHECK-FIXES: const auto& AutoAssigned = Obj->reference();
  Obj->constMethod();
  AutoAssigned.constMethod();
}

void NegativeMethodCallNonConstPointerIsModified(
    ExpensiveToCopyType *const Obj) {
  const auto AutoAssigned = Obj->reference();
  const auto AutoCopyConstructed(Obj->reference());
  const ExpensiveToCopyType VarAssigned = Obj->reference();
  const ExpensiveToCopyType VarCopyConstructed(Obj->reference());
  mutate(Obj);
}

void PositiveLocalVarIsNotModified() {
  ExpensiveToCopyType LocalVar;
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto AutoAssigned = LocalVar.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'AutoAssigned' CHECK-FIXES: const auto& AutoAssigned =
  // LocalVar.reference();
  AutoAssigned.constMethod();
}

void NegativeLocalVarIsModified() {
  ExpensiveToCopyType Obj;
  const auto AutoAssigned = Obj.reference();
  Obj = AutoAssigned;
}

struct NegativeConstructor {
  NegativeConstructor(const ExpensiveToCopyType &Obj) : Obj(Obj) {}
  ExpensiveToCopyType Obj;
};

#define UNNECESSARY_COPY_INIT_IN_MACRO_BODY(TYPE)                              \
  void functionWith##TYPE(const TYPE &T) {                                     \
    auto AssignedInMacro = T.reference();                                      \
  }                                                                            \
// Ensure fix is not applied.
// CHECK-FIXES: auto AssignedInMacro = T.reference();

// Raise CXX-P2005: "Variable `AssignedInMacro`, of an expensive-type, is
// copy-constructer but only used as const."
UNNECESSARY_COPY_INIT_IN_MACRO_BODY(ExpensiveToCopyType)
// CHECK-MESSAGES: [[@LINE-1]]:1: warning: the variable 'AssignedInMacro' is
// copy-constructed

#define UNNECESSARY_COPY_INIT_IN_MACRO_ARGUMENT(ARGUMENT) ARGUMENT

void PositiveMacroArgument(const ExpensiveToCopyType &Obj) {
  // Raise CXX-P2005: "Variable `CopyInMacroArg`, of an expensive-type, is
  // copy-constructer but only used as const."
  UNNECESSARY_COPY_INIT_IN_MACRO_ARGUMENT(auto CopyInMacroArg =
                                              Obj.reference());
  // CHECK-MESSAGES: [[@LINE-1]]:48: warning: the variable 'CopyInMacroArg' is
  // copy-constructed Ensure fix is not applied. CHECK-FIXES: auto
  // CopyInMacroArg = Obj.reference()
  CopyInMacroArg.constMethod();
}

void PositiveLocalCopyConstMethodInvoked() {
  ExpensiveToCopyType orig;
  // [CXX-P2005]: "Variable `copy_1`, of an expensive-type, is copy-constructer
  // but only used as const."
  ExpensiveToCopyType copy_1 = orig;
  copy_1.constMethod();
  orig.constMethod();
}

void PositiveLocalCopyUsingExplicitCopyCtor() {
  ExpensiveToCopyType orig;
  // [CXX-P2005]: "Variable `copy_2`, of an expensive-type, is copy-constructer
  // but only used as const."
  ExpensiveToCopyType copy_2(orig);
  // CHECK-MESSAGES: [[@LINE-1]]:23: warning: local copy 'copy_2'
  // CHECK-FIXES: const ExpensiveToCopyType& copy_2(orig);
  copy_2.constMethod();
  orig.constMethod();
}

void PositiveLocalCopyCopyIsArgument(const ExpensiveToCopyType &orig) {
  // [CXX-P2005]: "Variable `copy_3`, of an expensive-type, is copy-constructer
  // but only used as const."
  ExpensiveToCopyType copy_3 = orig;
  // CHECK-MESSAGES: [[@LINE-1]]:23: warning: local copy 'copy_3'
  // CHECK-FIXES: const ExpensiveToCopyType& copy_3 = orig;
  copy_3.constMethod();
}

void PositiveLocalCopyUsedAsConstRef() {
  ExpensiveToCopyType orig;
  // [CXX-P2005]: "Variable `copy_4`, of an expensive-type, is copy-constructer
  // but only used as const."
  ExpensiveToCopyType copy_4 = orig;
  // CHECK-MESSAGES: [[@LINE-1]]:23: warning: local copy 'copy_4'
  // CHECK-FIXES: const ExpensiveToCopyType& copy_4 = orig;
  useAsConstReference(orig);
  copy_4.constMethod();
}

void PositiveLocalCopyTwice() {
  ExpensiveToCopyType orig;
  // [CXX-P2005]: "Variable `copy_5`, of an expensive-type, is copy-constructer
  // but only used as const."
  ExpensiveToCopyType copy_5 = orig;
  // CHECK-MESSAGES: [[@LINE-1]]:23: warning: local copy 'copy_5'
  // CHECK-FIXES: const ExpensiveToCopyType& copy_5 = orig;
  // [CXX-P2005]: "Variable `copy_6`, of an expensive-type, is copy-constructer
  // but only used as const."
  ExpensiveToCopyType copy_6 = copy_5;
  // CHECK-MESSAGES: [[@LINE-1]]:23: warning: local copy 'copy_6'
  // CHECK-FIXES: const ExpensiveToCopyType& copy_6 = copy_5;
  copy_5.constMethod();
  copy_6.constMethod();
  orig.constMethod();
}

void PositiveLocalCopyWeirdCopy() {
  WeirdCopyCtorType orig;
  // [CXX-P2005]: "Variable `weird_1`, of an expensive-type, is copy-constructer
  // but only used as const."
  WeirdCopyCtorType weird_1(orig);
  // CHECK-MESSAGES: [[@LINE-1]]:21: warning: local copy 'weird_1'
  // CHECK-FIXES: const WeirdCopyCtorType& weird_1(orig);
  weird_1.constMethod();

  // [CXX-P2005]: "Variable `weird_2`, of an expensive-type, is copy-constructer
  // but only used as const."
  WeirdCopyCtorType weird_2 = orig;
  // CHECK-MESSAGES: [[@LINE-1]]:21: warning: local copy 'weird_2'
  // CHECK-FIXES: const WeirdCopyCtorType& weird_2 = orig;
  weird_2.constMethod();
}

void NegativeLocalCopySimpleTypes() {
  int i1 = 0;
  int i2 = i1;
}

void NegativeLocalCopyCopyIsModified() {
  ExpensiveToCopyType orig;
  ExpensiveToCopyType neg_copy_1 = orig;
  neg_copy_1.nonConstMethod();
}

void NegativeLocalCopyOriginalIsModified() {
  ExpensiveToCopyType orig;
  ExpensiveToCopyType neg_copy_2 = orig;
  orig.nonConstMethod();
}

void NegativeLocalCopyUsedAsRefArg() {
  ExpensiveToCopyType orig;
  ExpensiveToCopyType neg_copy_3 = orig;
  mutate(neg_copy_3);
}

void NegativeLocalCopyUsedAsPointerArg() {
  ExpensiveToCopyType orig;
  ExpensiveToCopyType neg_copy_4 = orig;
  mutate(&neg_copy_4);
}

void NegativeLocalCopyCopyFromGlobal() {
  ExpensiveToCopyType neg_copy_5 = global_expensive_to_copy_type;
}

void NegativeLocalCopyCopyToStatic() {
  ExpensiveToCopyType orig;
  static ExpensiveToCopyType neg_copy_6 = orig;
}

void NegativeLocalCopyNonConstInForLoop() {
  ExpensiveToCopyType orig;
  for (ExpensiveToCopyType neg_copy_7 = orig; orig.constMethod();
       orig.nonConstMethod()) {
    orig.constMethod();
  }
}

void NegativeLocalCopyWeirdNonCopy() {
  WeirdCopyCtorType orig;
  WeirdCopyCtorType neg_weird_1(orig, false);
  WeirdCopyCtorType neg_weird_2(orig, true);
}
void WarningOnlyMultiDeclStmt() {
  ExpensiveToCopyType orig;
  // [CXX-P2005]: "Variable `copy`, of an expensive-type, is copy-constructer
  // but only used as const."
  ExpensiveToCopyType copy = orig, copy2;
  copy.constMethod();
}

class Element {};

void implicitVarFalsePositive() {
  for (const Element &E : Container<Element>()) {
  }
}

// This should not trigger the check as the argument could introduce an alias.
void negativeInitializedFromFreeFunctionWithArg() {
  ExpensiveToCopyType Orig;
  const ExpensiveToCopyType Copy = freeFunctionWithArg(Orig);
}

void negativeInitializedFromFreeFunctionWithDefaultArg() {
  const ExpensiveToCopyType Copy = freeFunctionWithDefaultArg();
}

void negativeInitialzedFromFreeFunctionWithNonDefaultArg() {
  ExpensiveToCopyType Orig;
  const ExpensiveToCopyType Copy = freeFunctionWithDefaultArg(&Orig);
}

namespace std {
inline namespace __1 {

template <class> class function;
template <class R, class... ArgTypes> class function<R(ArgTypes...)> {
public:
  function();
  function(const function &Other);
  R operator()(ArgTypes... Args) const;
};

} // namespace __1
} // namespace std

void negativeStdFunction() {
  std::function<int()> Orig;
  std::function<int()> Copy = Orig;
  int i = Orig();
}

using Functor = std::function<int()>;

void negativeAliasedStdFunction() {
  Functor Orig;
  Functor Copy = Orig;
  int i = Orig();
}

typedef std::function<int()> TypedefFunc;

void negativeTypedefedStdFunction() {
  TypedefFunc Orig;
  TypedefFunc Copy = Orig;
  int i = Orig();
}

namespace fake {
namespace std {
template <class R, class... Args> struct function {
  // Custom copy constructor makes it expensive to copy;
  function(const function &);
  void constMethod() const;
};
} // namespace std

void positiveFakeStdFunction(std::function<void(int)> F) {
  // [CXX-P2005]: "Variable `Copy`, of an expensive-type, is copy-constructer
  // but only used as const."
  auto Copy = F;
  // CHECK-MESSAGES: [[@LINE-1]]:8: warning: local copy 'Copy' of the variable
  // 'F' is never modified; CHECK-FIXES: const auto& Copy = F;
  Copy.constMethod();
}

} // namespace fake

void positiveInvokedOnStdFunction(
    std::function<void(const ExpensiveToCopyType &)> Update,
    const ExpensiveToCopyType Orig) {
  // [CXX-P2005]: "Variable `Copy`, of an expensive-type, is copy-constructer
  // but only used as const."
  auto Copy = Orig.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:8: warning: the variable 'Copy' is
  // copy-constructed from a const reference CHECK-FIXES: const auto& Copy =
  // Orig.reference();
  Update(Copy);
}

void negativeInvokedOnStdFunction(
    std::function<void(ExpensiveToCopyType &)> Update,
    const ExpensiveToCopyType Orig) {
  auto Copy = Orig.reference();
  Update(Copy);
}

void negativeCopiedFromReferenceToModifiedVar() {
  ExpensiveToCopyType Orig;
  const auto &Ref = Orig;
  const auto NecessaryCopy = Ref;
  Orig.nonConstMethod();
}

void positiveCopiedFromReferenceToConstVar() {
  ExpensiveToCopyType Orig;
  const auto &Ref = Orig;
  // [CXX-P2005]: "Variable `UnnecessaryCopy`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto UnnecessaryCopy = Ref;
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: local copy 'UnnecessaryCopy' of
  // CHECK-FIXES: const auto& UnnecessaryCopy = Ref;
  Orig.constMethod();
  UnnecessaryCopy.constMethod();
}

void negativeCopiedFromGetterOfReferenceToModifiedVar() {
  ExpensiveToCopyType Orig;
  const auto &Ref = Orig.reference();
  const auto NecessaryCopy = Ref.reference();
  Orig.nonConstMethod();
}

void negativeAliasNonCanonicalPointerType() {
  ExpensiveToCopyType Orig;
  // The use of auto here hides that the type is a pointer type. The check needs
  // to look at the canonical type to detect the aliasing through this pointer.
  const auto Pointer = Orig.pointer();
  const auto NecessaryCopy = Pointer->reference();
  Orig.nonConstMethod();
}

void negativeAliasTypedefedType() {
  typedef const ExpensiveToCopyType &ReferenceType;
  ExpensiveToCopyType Orig;
  // The typedef hides the fact that this is a reference type. The check needs
  // to look at the canonical type to detect the aliasing.
  ReferenceType Ref = Orig.reference();
  const auto NecessaryCopy = Ref.reference();
  Orig.nonConstMethod();
}

void positiveCopiedFromGetterOfReferenceToConstVar() {
  ExpensiveToCopyType Orig;
  const auto &Ref = Orig.reference();
  // [CXX-P2005]: "Variable `UnnecessaryCopy`, of an expensive-type, is
  // copy-constructer but only used as const."
  auto UnnecessaryCopy = Ref.reference();
  // CHECK-MESSAGES: [[@LINE-1]]:8: warning: the variable 'UnnecessaryCopy' is
  // CHECK-FIXES: const auto& UnnecessaryCopy = Ref.reference();
  Orig.constMethod();
  UnnecessaryCopy.constMethod();
}

void positiveUnusedReferenceIsRemoved() {
  // clang-format off
  // [CXX-P2005]: "Variable `AutoAssigned`, of an expensive-type, is copy-constructer but never used."
  const auto AutoAssigned = ExpensiveTypeReference(); int i = 0; // Foo bar.
  // [CXX-P2005]: "Variable `TrailingCommentRemoved`, of an expensive-type, is copy-constructer but never used."
  auto TrailingCommentRemoved = ExpensiveTypeReference(); // Trailing comment.
  // CHECK-MESSAGES: [[@LINE-1]]:8: warning: the variable 'TrailingCommentRemoved' is copy-constructed from a const reference but is never used;
  // CHECK-FIXES-NOT: auto TrailingCommentRemoved = ExpensiveTypeReference();
  // CHECK-FIXES-NOT: // Trailing comment.
  // clang-format on

  // [CXX-P2005]: "Variable `UnusedAndUnnecessary`, of an expensive-type, is
  // copy-constructer but never used."
  auto UnusedAndUnnecessary = ExpensiveTypeReference();
  // Comments on a new line should not be deleted.
  // CHECK-MESSAGES: [[@LINE-2]]:8: warning: the variable 'UnusedAndUnnecessary'
  // is copy-constructed CHECK-FIXES-NOT: auto UnusedAndUnnecessary =
  // ExpensiveTypeReference(); CHECK-FIXES: // Comments on a new line should not
  // be deleted.
}

void positiveLoopedOverObjectIsConst() {
  const Container<ExpensiveToCopyType> Orig;
  for (const auto &Element : Orig) {
    // [CXX-P2005]: "Variable `Copy`, of an expensive-type, is copy-constructer
    // but only used as const."
    const auto Copy = Element;
    // CHECK-MESSAGES: [[@LINE-1]]:16: warning: local copy 'Copy'
    // CHECK-FIXES: const auto& Copy = Element;
    Orig.constMethod();
    Copy.constMethod();
  }

  auto Lambda = []() {
    const Container<ExpensiveToCopyType> Orig;
    for (const auto &Element : Orig) {
      // [CXX-P2005]: "Variable `Copy`, of an expensive-type, is
      // copy-constructer but only used as const."
      const auto Copy = Element;
      // CHECK-MESSAGES: [[@LINE-1]]:18: warning: local copy 'Copy'
      // CHECK-FIXES: const auto& Copy = Element;
      Orig.constMethod();
      Copy.constMethod();
    }
  };
}

void negativeLoopedOverObjectIsModified() {
  Container<ExpensiveToCopyType> Orig;
  for (const auto &Element : Orig) {
    const auto Copy = Element;
    Orig.nonConstMethod();
    Copy.constMethod();
  }

  auto Lambda = []() {
    Container<ExpensiveToCopyType> Orig;
    for (const auto &Element : Orig) {
      const auto Copy = Element;
      Orig.nonConstMethod();
      Copy.constMethod();
    }
  };
}

void negativeReferenceIsInitializedOutsideOfBlock() {
  ExpensiveToCopyType Orig;
  const auto &E2 = Orig;
  if (1 != 2 * 3) {
    const auto C2 = E2;
    Orig.nonConstMethod();
    C2.constMethod();
  }

  auto Lambda = []() {
    ExpensiveToCopyType Orig;
    const auto &E2 = Orig;
    if (1 != 2 * 3) {
      const auto C2 = E2;
      Orig.nonConstMethod();
      C2.constMethod();
    }
  };
}

void negativeStructuredBinding() {
  // Structured bindings are not yet supported but can trigger false positives
  // since the DecompositionDecl itself is unused and the check doesn't traverse
  // VarDecls of the BindingDecls.
  struct Pair {
    ExpensiveToCopyType first;
    ExpensiveToCopyType second;
  };

  Pair P;
  const auto [C, D] = P;
  C.constMethod();
  D.constMethod();
}

template <typename A> const A &templatedReference();

template <typename A, typename B> void negativeTemplateTypes() {
  A Orig;
  // Different replaced template type params do not trigger the check. In some
  // template instantiation this might not be a copy but an implicit
  // conversion, so converting this to a reference might not work.
  B AmbiguousCopy = Orig;
  // CHECK-NOT-FIXES: B AmbiguousCopy = Orig;

  B NecessaryCopy = templatedReference<A>();
  // CHECK-NOT-FIXES: B NecessaryCopy = templatedReference<A>();

  B NecessaryCopy2 = Orig.template templatedAccessor<A>();

  // Non-dependent types in template still trigger the check.
  // [CXX-P2005]: "Variable `UnnecessaryCopy`, of an expensive-type, is
  // copy-constructer but only used as const."
  const auto UnnecessaryCopy = ExpensiveTypeReference();
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: the const qualified variable
  // 'UnnecessaryCopy' is copy-constructed CHECK-FIXES: const auto&
  // UnnecessaryCopy = ExpensiveTypeReference();
  UnnecessaryCopy.constMethod();
}

void instantiateNegativeTemplateTypes() {
  negativeTemplateTypes<ExpensiveToCopyType, ExpensiveToCopyType>();
  // This template instantiation would not compile if the `AmbiguousCopy` above
  // was made a reference.
  negativeTemplateTypes<ExpensiveToCopyType, int>();
}

template <typename A> void positiveSingleTemplateType() {
  A Orig;
  // [CXX-P2005]: "Variable `SingleTmplParmTypeCopy`, of an expensive-type, is
  // copy-constructer but only used as const."
  A SingleTmplParmTypeCopy = Orig;
  // CHECK-MESSAGES: [[@LINE-1]]:5: warning: local copy 'SingleTmplParmTypeCopy'
  // of the variable 'Orig' is never modified CHECK-FIXES: const A&
  // SingleTmplParmTypeCopy = Orig;
  SingleTmplParmTypeCopy.constMethod();

  // [CXX-P2005]: "Variable `UnnecessaryCopy2`, of an expensive-type, is
  // copy-constructer but only used as const."
  A UnnecessaryCopy2 = templatedReference<A>();
  // CHECK-MESSAGES: [[@LINE-1]]:5: warning: the variable 'UnnecessaryCopy2' is
  // copy-constructed from a const reference CHECK-FIXES: const A&
  // UnnecessaryCopy2 = templatedReference<A>();
  UnnecessaryCopy2.constMethod();

  // [CXX-P2005]: "Variable `UnnecessaryCopy3`, of an expensive-type, is
  // copy-constructer but only used as const."
  A UnnecessaryCopy3 = Orig.template templatedAccessor<A>();
  // CHECK-MESSAGES: [[@LINE-1]]:5: warning: the variable 'UnnecessaryCopy3' is
  // copy-constructed from a const reference CHECK-FIXES: const A&
  // UnnecessaryCopy3 = Orig.template templatedAccessor<A>();
  UnnecessaryCopy3.constMethod();
}

void instantiatePositiveSingleTemplateType() {
  positiveSingleTemplateType<ExpensiveToCopyType>();
}
