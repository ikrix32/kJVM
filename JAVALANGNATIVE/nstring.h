/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
//String
#ifndef NATIVE_STRING
#define NATIVE_STRING
#include "typedefinitions.h"
#ifndef KNATIVE_DISPATCH
char nativeCharAt(void);
char nativeStringLength(void);
#endif
char stringsNotEquals(const char* str1,const char* str2,const u4 len);
int stringLength(const char* str);
#endif
