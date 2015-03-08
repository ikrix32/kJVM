#ifdef ENABLE_TESTS

static const u1 tests_HelloWorldBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_HelloWorld_k.h"
#else
#include "tests_HelloWorld.h"
#endif

static const u1 tests_ArithmeticBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_Arithmetic_k.h"
#else
#include "tests_Arithmetic.h"
#endif

static const u1 tests_ICMPBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_ICMP_k.h"
#else
#include "tests_ICMP.h"
#endif

static const u1 tests_FibonacciBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_Fibonacci_k.h"
#else
#include "tests_Fibonacci.h"
#endif

static const u1 tests_SelfInstanceBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_SelfInstance_k.h"
#else
#include "tests_SelfInstance.h"
#endif

static const u1 tests_StaticTestBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_StaticTest_k.h"
#else
#include "tests_StaticTest.h"
#endif

static const u1 tests_StringAndHeapTestBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_StringAndHeapTest_k.h"
#else
#include "tests_StringAndHeapTest.h"
#endif

static const u1 tests_SwitchBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_Switch_k.h"
#else
#include "tests_Switch.h"
#endif

static const u1 tests_ArraySizeBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_ArraySize_k.h"
#else
#include "tests_ArraySize.h"
#endif

static const u1 tests_InheritanceABin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_InheritanceA_k.h"
#else
#include "tests_InheritanceA.h"
#endif

static const u1 tests_InheritanceBBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_InheritanceB_k.h"
#else
#include "tests_InheritanceB.h"
#endif

static const u1 tests_InheritanceBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_Inheritance_k.h"
#else
#include "tests_Inheritance.h"
#endif

static const u1 tests_FloatTestBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_FloatTest_k.h"
#else
#include "tests_FloatTest.h"
#endif

static const u1 tests_QuickSortBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_QuickSort_k.h"
#else
#include "tests_QuickSort.h"
#endif

static const u1 tests_ErathostenesBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_Erathostenes_k.h"
#else
#include "tests_Erathostenes.h"
#endif

static const u1 tests_DivByZeroBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_DivByZero_k.h"
#else
#include "tests_DivByZero.h"
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
};

#endif
