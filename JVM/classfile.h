
#ifndef __KJVM_CLASSFILE__
#define __KJVM_CLASSFILE__
#include "typedefinitions.h"

//#define getU1(classId,pos) *(cs[classId].classFileStartAddress)
//#define getU2(...) 0
//#define getU4(...) 0
//#define getFloat(...) 0
u1 getU1(const u1 classId,const u2 pos);
u2 getU2(const u1 classId,const u2 pos);
u4 getU4(const u1 classId,const u2 pos);
f4 getFloat(const u1 classId,const u2 pos);

void* getAddr(const u1 classId,const u2 pos);

void analyzeClass(const u1 classId);
void analyzeConstantPool(const u1 classId);
void analyzeMethods(const u1 classId);
void analyzeFields(const u1 classId);
#ifdef ENABLE_KCLASS_FORMAT
u2 getClassIndex(u2 classId);
#endif//#else
u1 findClass(const char* name,const u1 len);
//#endif
u1 findSuperClass(const u1 classId);
u1 findNumArgs(const u1 classId,const u2 method);
u2 findMaxLocals(const u1 classId,const u1 methodId);
u1 findFieldByName(const u2 instanceClassId,const u2 fieldClassId,const char* fieldName,const u1 fieldNameLength,
                   const char* fieldDescr,const u1 fieldDescrLength,const u1 isStatic);
//u1 findFieldByRamName(const char* fieldName,const u1 fieldNameLength,
//                      const char* fieldDescr,const u1 fieldDescrLength);
//u1 findStaticFieldByName(const char* fieldName,const u1 fieldNameLength,
//                         const char* fieldDescr,const u1 fieldDescrLength);
u1 findMethodByName(const u1 classId,const char* name, const u1 len, const char* methodDescr,
                    const u1 methodDescrLength);
u1 findClassFlash(const char* className,const u1 classNameLength);
u1* findMethodByMethodNumber(const u1 classId,const u1 methodId);               // mb jf

u2 getStartPC(const u1 classId,const u1 methodId);
//u1 strncmpFlashFlash(const char*, const char*, u1);
//u1 strncmpRamFlash(const char*, const char*, u1);
#endif
