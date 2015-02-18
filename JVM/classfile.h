/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
#include "typedefinitions.h"

u1 getU1(const u1 classId,const u2 pos);
u2 getU2(const u1 classId,const u2 pos);
u4 getU4(const u1 classId,const u2 pos);
f4 getFloat(const u1 classId,const u2 pos);

void* getAddr(const u1 classId,const u2 pos);

void analyzeClass();
void analyzeConstantPool();
void analyzeMethods();
void analyzeFields();
u1 findClass(const char* name,const u1 len);
u1 findSuperClass(const u1 classId);
u1 findNumArgs(const u1 classId,const u2 method);
u2 findMaxLocals(const u1 classId);
u1 findFieldByName(const char* fieldName,const u1 fieldNameLength,
const char* fieldDescr,const u1 fieldDescrLength);
u1 findFieldByRamName(const char* fieldName,const u1 fieldNameLength,
const char* fieldDescr,const u1 fieldDescrLength);
u1 findStaticFieldByName(const char* fieldName,const u1 fieldNameLength,
const char* fieldDescr,const u1 fieldDescrLength);
u1 findMethod(const char* className, const u1 classNameLength,
const char* methodName, const u1 methodNameLength,
const char* methodDescr, const u1 methodDescrLength);
u1 findMethodByName(const u1 classId,const char* name, const u1 len, const char* methodDescr,
const u1 methodDescrLength);
u1 findMethodByNameFlash(const u1 classId,const char* name, const u1 len,
const char* methodDescr, const u1 methodDescrLength);
u1 findClassFlash(const char* className,const u1 classNameLength);
u1* findMethodByMethodNumber(const u1 classId,const u1 methodId);               // mb jf

u2 getStartPC(void);
u1 strncmpFlashFlash(const char*, const char*, u1);
u1 strncmpRamFlash(const char*, const char*, u1);
