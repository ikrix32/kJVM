/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07

#ifndef __STACK__
#define __STACK__
void opStackInit(slot**);
#ifdef AVR8
extern void opStackPush( slot val);
extern slot opStackPop(void);
extern slot opStackPeek(void);
extern void opStackPoke(slot val);
extern slot opStackGetValue(u2 pos);
extern void opStackSetValue(u2 pos,slot val);
extern u2 opStackGetSpPos(void);
extern void opStackSetSpPos(u2 pos);
#else
void opStackPush(slot val);
slot opStackPop(void);
slot opStackPeek(void);
void opStackPoke(slot val);
slot opStackGetValue(u2 pos);
void opStackSetValue(u2 pos, slot val);
u2 opStackGetSpPos(void);
void opStackSetSpPos(u2 pos);
#endif

void methodStackInit(u2** m);

#ifdef AVR8
extern void methodStackPush(u2 val);
extern u2 methodStackPop(void);
extern u2 methodStackPeek(void);
extern u2 methodStackGetSpPos(void);
extern void methodStackSetSpPos(u2 pos);
extern u1 methodStackEmpty(void);
#else
void methodStackPush(u2 val);
u2 methodStackPop(void);
u2 methodStackPeek(void);
u2 methodStackGetSpPos(void);
void methodStackSetSpPos(u2 pos);
u1 methodStackEmpty(void);
#endif
#endif
