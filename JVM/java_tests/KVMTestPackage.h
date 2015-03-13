#ifdef ENABLE_TESTS

static const u1 tests_NativeMethodsTestBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "tests_NativeMethodsTest_k.h"
#else
#include "tests_NativeMethodsTest.h"
#endif

static const u1* testBinaries[] =
{	tests_NativeMethodsTestBin,
};

static const u4 testBinariesSize[] =
{	sizeof(tests_NativeMethodsTestBin),
};

static const char* testNames[] =
{	"tests/NativeMethodsTest",
};

#endif
