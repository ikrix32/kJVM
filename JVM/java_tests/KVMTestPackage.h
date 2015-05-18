#ifdef ENABLE_TESTS

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_HelloWorld_k.h"
#else
#include "tests_HelloWorld.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_Arithmetic_k.h"
#else
#include "tests_Arithmetic.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_ICMP_k.h"
#else
#include "tests_ICMP.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_Fibonacci_k.h"
#else
#include "tests_Fibonacci.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_SelfInstance_k.h"
#else
#include "tests_SelfInstance.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_StaticTest_k.h"
#else
#include "tests_StaticTest.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_StringAndHeapTest_k.h"
#else
#include "tests_StringAndHeapTest.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_Switch_k.h"
#else
#include "tests_Switch.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_ArraySize_k.h"
#else
#include "tests_ArraySize.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_InheritanceA_k.h"
#else
#include "tests_InheritanceA.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_InheritanceB_k.h"
#else
#include "tests_InheritanceB.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_Inheritance_k.h"
#else
#include "tests_Inheritance.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_FloatTest_k.h"
#else
#include "tests_FloatTest.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_QuickSort_k.h"
#else
#include "tests_QuickSort.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_Erathostenes_k.h"
#else
#include "tests_Erathostenes.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_DivByZero_k.h"
#else
#include "tests_DivByZero.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_ThreadTest_k.h"
#else
#include "tests_ThreadTest.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_NativeMethodsTest_k.h"
#else
#include "tests_NativeMethodsTest.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_MainTests_k.h"
#else
#include "tests_MainTests.h"
#endif

static const u1* testBinaries[] =
{	tests_HelloWorldBin,
	tests_ArithmeticBin,
	tests_ICMPBin,
	tests_FibonacciBin,
	tests_SelfInstanceBin,
	tests_StaticTestBin,
	tests_StringAndHeapTestBin,
	tests_SwitchBin,
	tests_ArraySizeBin,
	tests_InheritanceABin,
	tests_InheritanceBBin,
	tests_InheritanceBin,
	tests_FloatTestBin,
	tests_QuickSortBin,
	tests_ErathostenesBin,
	tests_DivByZeroBin,
	tests_ThreadTestBin,
	tests_NativeMethodsTestBin,
	tests_MainTestsBin,
};

static const u4 testBinariesSize[] =
{	sizeof(tests_HelloWorldBin),
	sizeof(tests_ArithmeticBin),
	sizeof(tests_ICMPBin),
	sizeof(tests_FibonacciBin),
	sizeof(tests_SelfInstanceBin),
	sizeof(tests_StaticTestBin),
	sizeof(tests_StringAndHeapTestBin),
	sizeof(tests_SwitchBin),
	sizeof(tests_ArraySizeBin),
	sizeof(tests_InheritanceABin),
	sizeof(tests_InheritanceBBin),
	sizeof(tests_InheritanceBin),
	sizeof(tests_FloatTestBin),
	sizeof(tests_QuickSortBin),
	sizeof(tests_ErathostenesBin),
	sizeof(tests_DivByZeroBin),
	sizeof(tests_ThreadTestBin),
	sizeof(tests_NativeMethodsTestBin),
	sizeof(tests_MainTestsBin),
};

static const char* testNames[] =
{	"tests/HelloWorld",
	"tests/Arithmetic",
	"tests/ICMP",
	"tests/Fibonacci",
	"tests/SelfInstance",
	"tests/StaticTest",
	"tests/StringAndHeapTest",
	"tests/Switch",
	"tests/ArraySize",
	"tests/InheritanceA",
	"tests/InheritanceB",
	"tests/Inheritance",
	"tests/FloatTest",
	"tests/QuickSort",
	"tests/Erathostenes",
	"tests/DivByZero",
	"tests/ThreadTest",
	"tests/NativeMethodsTest",
	"tests/MainTests",
};

static const char** testMethodNames[] =
{	tests_HelloWorldMethodNames,
	tests_ArithmeticMethodNames,
	tests_ICMPMethodNames,
	tests_FibonacciMethodNames,
	tests_SelfInstanceMethodNames,
	tests_StaticTestMethodNames,
	tests_StringAndHeapTestMethodNames,
	tests_SwitchMethodNames,
	tests_ArraySizeMethodNames,
	tests_InheritanceAMethodNames,
	tests_InheritanceBMethodNames,
	tests_InheritanceMethodNames,
	tests_FloatTestMethodNames,
	tests_QuickSortMethodNames,
	tests_ErathostenesMethodNames,
	tests_DivByZeroMethodNames,
	tests_ThreadTestMethodNames,
	tests_NativeMethodsTestMethodNames,
	tests_MainTestsMethodNames,
};

extern const char* getClassMethodName(const u2 classId,const u2 methodId){
	return testMethodNames[classId][methodId];
}
#endif
