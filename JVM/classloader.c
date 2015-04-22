#include "definitions.h"
#include <stdlib.h>
#include "kjvm.h"
#include "classfile.h"
#include "heap.h"
#include "stack.h"
#include "interpreter.h"
#include "platform.h"


void classLoader_init(void){
#if (AVR32LINUX || LINUX)
    classFileBase=(char*)malloc((size_t) MAXBYTECODE);
    if (classFileBase==NULL)
        errorExit(-1,"malloc error while trying to allocate %d bytes for class files.\n", MAXBYTECODE);
#endif
    numClasses = 0;
    crtByteCodeSize = 0;
}

/* read, analyze classfiles and fill structures */
u1 classLoader_loadClass(const u1* bin,const u4 binSize)
{
    if (crtByteCodeSize + binSize > MAXBYTECODE){
       ERROREXIT(-1, "MAXBYTECODE reached!\n");
    }
    const u1 classIndex = numClasses;
    numClasses++;
    cs[classIndex].classFileStartAddress = classFileBase + crtByteCodeSize;
    cs[classIndex].classFileLength = readClassBin(bin,binSize,cs[classIndex].classFileStartAddress);
    analyzeClass(classIndex);
    crtByteCodeSize += cs[classIndex].classFileLength;

    PRINTF("Loaded class id:%d bytecode size %d,current loaded bytecode size: %d\n\n",classIndex,binSize,crtByteCodeSize);

    DEBUGPRINTHEAP;
    return classIndex;
}

void unloadLastClass(){
    numClasses--;
    crtByteCodeSize -= cs[numClasses].classFileLength;
}

void classLoader_clinitClass(const u1 classId){
    //initialize class
#ifdef ENABLE_KMETHOD
    extern const u2 getCLInitMethodId(const u2 classId);
    mN = getCLInitMethodId(getClassID(classId));
#else
    mN = FIND_METHOD_BYNAME(classId,"<clinit>", 8, "()V", 3);
#endif
    if (mN != INVALID_METHOD_ID)
    {
        opStackPush(cs[classId].classInfo);
        opStackSetSpPos(findMaxLocals(classId,mN));
        cN = classId;//todo fixme!
        interpreter_run();
    }
}
