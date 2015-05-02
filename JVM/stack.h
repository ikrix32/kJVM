#ifndef __STACK__
#define __STACK__

void opStackInit(slot**);
void opStackPush(const slot val);
slot opStackPop(void);
slot opStackPeek(void);
void opStackPoke(const slot val);
slot opStackGetValue(const u2 pos);
void opStackSetValue(const u2 pos,const slot val);
u2 opStackGetSpPos(void);
void opStackSetSpPos(const u2 pos);


void methodStackInit(u2** m);
void methodStackPush(const u2 val);
u2 methodStackPop(void);
u2 methodStackPeek(void);
u2 methodStackGetSpPos(void);
void methodStackSetSpPos(const u2 pos);
u1 methodStackEmpty(void);

#endif
