#include "definitions.h"
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
    const u1 classId = numClasses;
    numClasses++;
    cs[classId].classFileStartAddress = classFileBase + crtByteCodeSize;
    cs[classId].classFileLength = readClassBin(bin,binSize,cs[classId].classFileStartAddress);
    analyzeClass(classId);
    crtByteCodeSize += cs[classId].classFileLength;

    PRINTF("Loaded class id:%d bytecode size %d,current loaded bytecode size: %d\n\n",classId,binSize,crtByteCodeSize);

    DEBUGPRINTHEAP;
    return classId;
}

void classLoader_clinitClass(const u1 classId){
    //initialize class
    if (findMethodByName(classId,"<clinit>", 8, "()V", 3))
    {
        opStackPush(cs[classId].classInfo);
        opStackSetSpPos(findMaxLocals(classId,mN));
        cN = classId;//todo fixme!
        interpreter_run();
    }
}
