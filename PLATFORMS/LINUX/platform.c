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
    extern int getNoMicroKernelClasses();
    extern u1* getMicroKernelBinary();
    extern u4 getMicroKernelBinarySize(const int i);

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
    return findClass(className,stringLength(className));
}
#endif


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
    exit(n);
}
