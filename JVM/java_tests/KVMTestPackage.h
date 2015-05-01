#ifdef ENABLE_TESTS

#ifdef ENABLE_KCLASS_FORMAT
#include "tests_HelloWorld_k.h"
#else
#include "tests_HelloWorld.h"
#endif

static const u1* testBinaries[] =
{	tests_HelloWorldBin,
};

static const u4 testBinariesSize[] =
{	sizeof(tests_HelloWorldBin),
};

static const char* testNames[] =
{	"tests/HelloWorld",
};

static const char** testMethodNames[] =
{	tests_HelloWorldMethodNames,
};

extern const char* getClassMethodName(const u2 classId,const u2 methodId){
	return testMethodNames[classId][methodId];
}
#endif
