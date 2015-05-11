#ifdef ENABLE_TESTS

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_InheritanceA_k.h"
#else
#include "tests_InheritanceA.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_HeapTest_k.h"
#else
#include "tests_HeapTest.h"
#endif

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_MainTests_k.h"
#else
#include "tests_MainTests.h"
#endif

static const u1* testBinaries[] =
{	tests_InheritanceABin,
	tests_HeapTestBin,
	tests_MainTestsBin,
};

static const u4 testBinariesSize[] =
{	sizeof(tests_InheritanceABin),
	sizeof(tests_HeapTestBin),
	sizeof(tests_MainTestsBin),
};

static const char* testNames[] =
{	"tests/InheritanceA",
	"tests/HeapTest",
	"tests/MainTests",
};

static const char** testMethodNames[] =
{	tests_InheritanceAMethodNames,
	tests_HeapTestMethodNames,
	tests_MainTestsMethodNames,
};

extern const char* getClassMethodName(const u2 classId,const u2 methodId){
	return testMethodNames[classId][methodId];
}
#endif
