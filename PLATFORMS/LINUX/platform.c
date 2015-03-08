#include <stdio.h>
//#include <stdlib.h>
#ifndef NRF51
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include "definitions.h"
#include "kjvm.h"
#include "classfile.h"
#include "heap.h"
#include "platform.h"
#include "classloader.h"

#ifdef USE_MICROKERNEL

extern void loadMicroKernel()
{
    extern const int getNoMicroKernelClasses();
    extern const u1* getMicroKernelBinary();
    extern const u4 getMicroKernelBinarySize(const int i);

    const int  noBins = getNoMicroKernelClasses();
    for (int i = 0; i < noBins; i++)
        classLoader_loadClass(getMicroKernelBinary(i),getMicroKernelBinarySize(i));

    for (int i = 0; i < numClasses; i++)
        classLoader_clinitClass(i);
}
#endif

#ifdef ENABLE_KCLASS_FORMAT
extern u2 getExceptionClassId(const Exception exception){
    extern u2 getMicroKernelExceptionClassId(const int exception);
    return getMicroKernelExceptionClassId(exception);
}
#else
const static char* exceptionClassNames[]=
{
    "java/lang/ArrayIndexOutOfBoundsException",
    "java/lang/NegativeArraySizeException",
    "java/lang/NullPointerException",
    "java/lang/ArithmeticException",
    "java/lang/ClassCastException",
    "java/lang/IllegalMonitorStateException",
};
extern u2 getExceptionClassId(const Exception exception){
    const char* className = exceptionClassNames[exception];
    return FIND_CLASS(className,stringLength(className));
}
#endif

extern const char* getClassName(const u2 classId){
#ifdef USE_MICROKERNEL
    extern const int getNoMicroKernelClasses();
    const int  noMicroClasses = getNoMicroKernelClasses();
    if(classId < noMicroClasses){
#ifdef DEBUG_KCLASS
        extern const char* getMicroKernelClassName(const u2 classId);
        return getMicroKernelClassName(classId);
#else
        return "kernelclass";
#endif
    }else
        return "appClass";//todo
#else
        return "appClass";
#endif
}

void initHW()
{
}

u2 readClassFile(char* fileName, char* addr)
{
#if LINUX||AVR32LINUX
#ifndef NRF51
    int fd;
    u2 classFileLength=-(u2)((long)addr%(1<<16))-1;
    if ((fd=open(fileName,O_RDONLY))==-1)
        perror(fileName);
    while (read(fd,addr++,1));
    return classFileLength+=(long)addr;
#else
    return 0;
#endif
#endif
}

u2 readClassBin(const u1* bin,const u4 binSize, char* addr)
{
#if LINUX||AVR32LINUX
    u2 classFileLength=-(u2)((long)addr%(1<<16))-1;
    for (int i = 0; i < binSize; i++) {
        addr[0] = (char)bin[0];
        addr++;bin++;
    }
    return classFileLength += (long)addr;
#else
    return 0;
#endif
}

void linuxExit(int n)
{
#ifdef DEBUGOPSTACK
    printf(" maxOperandenStack: x%x",maxOpStack);
#endif                                        //DEBUOPSTACK
#ifdef DEBUGMETHODSTACK
    printf(" maxMethodStack: x%x\n",maxMethodStack);
#endif                                        //DEBUGMETHODSTACK
    exit(n);
}
