#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../JVM/definitions.h"
#include "../../JVM/bajvm.h"
#include "../../JVM/classfile.h"
#include "../../JVM/heap.h"
#include "platform.h"

void initHW()
{
}


/* all class files stored for linux in DS (malloc)*/

void initVM(int argc, char* argv[])               /* read, analyze classfiles and fill structures*/
{
    u4 length;

#if (AVR32LINUX||LINUX)
    classFileBase=(char*)malloc((size_t) MAXBYTECODE);
    if (classFileBase==NULL)
        errorExit(-1,"malloc error while trying to allocate %d bytes for class files.\n", MAXBYTECODE);
#endif
    heapInit();                                   /* linux avr8 malloc , others hard coded!*/
    length = 0;
#if LINUX|| AVR32LINUX
    if (argc > MAXCLASSES)
        errorExit(-1,"ERROR: trying to load %d classes, MAXCLASSES is %d\n", argc, MAXCLASSES);
    for (cN=0; cN < argc; cN++)
    {
        cs[cN].classFileStartAddress=classFileBase+length;
        cs[cN].classFileLength=readClassFile(argv[cN+1], cs[cN].classFileStartAddress);
        analyzeClass(&cs[cN]);
        length+=cs[cN].classFileLength;
        if (length > MAXBYTECODE)
        {
            printf("MAXBYTECODE reached!\n"); exit(-1);
        }
        numClasses=cN+1;
    }
#endif
    DEBUGPRINTHEAP;
}


u2 readClassFile(char* fileName, char* addr)
{
#if LINUX||AVR32LINUX
    int fd;
    u2 classFileLength=-(u2)((long)addr%(1<<16))-1;
    if ((fd=open(fileName,O_RDONLY))==-1)
        perror(fileName);
    while (read(fd,addr++,1));
    return classFileLength+=(long)addr;
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
