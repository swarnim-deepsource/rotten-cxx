// scatr-check: CXX-P2009
// RUN: %check_clang_tidy %s performance-unnecessary-value-param %t -- -- -fdelayed-template-parsing

struct ExpensiveToCopyType {
    const ExpensiveToCopyType& constReference() const { return *this; }
    void nonConstMethod();
    virtual ~ExpensiveToCopyType();
};

void mutate(ExpensiveToCopyType&);
void mutate(ExpensiveToCopyType*);
void useAsConstReference(const ExpensiveToCopyType&);
void useByValue(ExpensiveToCopyType);

// This class simulates std::pair<>. It is trivially copy constructible
// and trivially destructible, but not trivially copy assignable.
class SomewhatTrivial {
  public:
    SomewhatTrivial();
    SomewhatTrivial(const SomewhatTrivial&) = default;
    ~SomewhatTrivial() = default;
    SomewhatTrivial& operator=(const SomewhatTrivial&);
};

void positiveExpensiveConstValue(const ExpensiveToCopyType Obj);
// [CXX-P2009]
void positiveExpensiveConstValue(const ExpensiveToCopyType Obj) {}

void positiveExpensiveValue(ExpensiveToCopyType Obj);
// [CXX-P2009]
void positiveExpensiveValue(ExpensiveToCopyType Obj) {
    Obj.constReference();
    useAsConstReference(Obj);
    // [CXX-P2005]
    auto Copy = Obj;
    useByValue(Obj);
}

void positiveWithComment(const ExpensiveToCopyType /* important */ S);
// [CXX-P2009]
void positiveWithComment(const ExpensiveToCopyType /* important */ S) {}

// [CXX-P2009]
void positiveUnnamedParam(const ExpensiveToCopyType) {}

void positiveAndNegative(const ExpensiveToCopyType ConstCopy, const ExpensiveToCopyType& ConstRef,
                         ExpensiveToCopyType Copy);

// [CXX-P2009]
void positiveAndNegative(const ExpensiveToCopyType ConstCopy, const ExpensiveToCopyType& ConstRef,
                         // [CXX-P2009]
                         ExpensiveToCopyType Copy) {}

struct PositiveConstValueConstructor {
    // [CXX-P2009]
    PositiveConstValueConstructor(const ExpensiveToCopyType ConstCopy) {}
};

// [CXX-P2009]
template <typename T> void templateWithNonTemplatizedParameter(const ExpensiveToCopyType S, T V) {}

void instantiated() {
    templateWithNonTemplatizedParameter(ExpensiveToCopyType(), ExpensiveToCopyType());
    templateWithNonTemplatizedParameter(ExpensiveToCopyType(), 5);
}

template <typename T> void negativeTemplateType(const T V) {}

void negativeArray(const ExpensiveToCopyType[]) {}

void negativePointer(ExpensiveToCopyType* Obj) {}

void negativeConstPointer(const ExpensiveToCopyType* Obj) {}

void negativeConstReference(const ExpensiveToCopyType& Obj) {}

void negativeReference(ExpensiveToCopyType& Obj) {}

void negativeUniversalReference(ExpensiveToCopyType&& Obj) {}

void negativeSomewhatTrivialConstValue(const SomewhatTrivial Somewhat) {}

void negativeSomewhatTrivialValue(SomewhatTrivial Somewhat) {}

void negativeConstBuiltIn(const int I) {}

void negativeValueBuiltIn(int I) {}

void negativeValueIsMutatedByReference(ExpensiveToCopyType Obj) { mutate(Obj); }

void negativeValueIsMutatatedByPointer(ExpensiveToCopyType Obj) { mutate(&Obj); }

void negativeValueIsReassigned(ExpensiveToCopyType Obj) { Obj = ExpensiveToCopyType(); }

void negativeValueNonConstMethodIsCalled(ExpensiveToCopyType Obj) { Obj.nonConstMethod(); }

struct PositiveValueUnusedConstructor {
    // [CXX-P2009]
    PositiveValueUnusedConstructor(ExpensiveToCopyType Copy) {}
};

struct PositiveValueCopiedConstructor {
    // [CXX-P2009]
    PositiveValueCopiedConstructor(ExpensiveToCopyType Copy) : Field(Copy) {}
    ExpensiveToCopyType Field;
};

template <typename T> struct Container {
    typedef const T& const_reference;
};

void NegativeTypedefParam(const Container<ExpensiveToCopyType>::const_reference Param) {}

#define UNNECESSARY_VALUE_PARAM_IN_MACRO_BODY()                                                    \
    void inMacro(const ExpensiveToCopyType T) {}                                                   \
// Ensure fix is not applied.

UNNECESSARY_VALUE_PARAM_IN_MACRO_BODY()

#define UNNECESSARY_VALUE_PARAM_IN_MACRO_ARGUMENT(ARGUMENT) ARGUMENT

UNNECESSARY_VALUE_PARAM_IN_MACRO_ARGUMENT(
    void inMacroArgument(const ExpensiveToCopyType InMacroArg){})

struct VirtualMethod {
    virtual ~VirtualMethod() {}
    virtual void handle(ExpensiveToCopyType T) const = 0;
};

struct NegativeOverriddenMethod : public VirtualMethod {
    void handle(ExpensiveToCopyType Overridden) const {}
};

struct NegativeDeletedMethod {
    ~NegativeDeletedMethod() {}
    NegativeDeletedMethod& operator=(NegativeDeletedMethod N) = delete;
};
