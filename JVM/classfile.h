/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
#include "typedefinitions.h"

u1 getU1(u2 pos);
u2 getU2(u2 pos);
u4 getU4(u2 pos);
f4 getFloat(u2 pos);

void* getAddr(u2 pos);

void analyzeClass();
void analyzeConstantPool();
void analyzeMethods();
void analyzeFields();
u1 findClass(const char* name, u1 len);
u1 findSuperClass(void);
u1 findNumArgs(u2 method);
u2 findMaxLocals(void);                           //mb jf
u1 findFieldByName(const char* fieldName, u1 fieldNameLength,
const char* fieldDescr, u1 fieldDescrLength);
u1 findFieldByRamName(const char* fieldName, u1 fieldNameLength,
const char* fieldDescr, u1 fieldDescrLength);
u1 findStaticFieldByName(const char* fieldName, u1 fieldNameLength,
const char* fieldDescr, u1 fieldDescrLength);
u1 findMethod(const char* className, const u1 classNameLength,
const char* methodName, const u1 methodNameLength,
const char* methodDescr, const u1 methodDescrLength);
u1 findMethodByName(const char* name, const u1 len, const char* methodDescr,
const u1 methodDescrLength);
u1 findMethodByNameFlash(const char* name, const u1 len,
const char* methodDescr, const u1 methodDescrLength);
u1 findClassFlash(const char* className, u1 classNameLength);
u1* findMethodByMethodNumber(void);               // mb jf

u2 getStartPC(void);
u1 strncmpFlashFlash(const char*, const char*, u1);
u1 strncmpRamFlash(const char*, const char*, u1);
