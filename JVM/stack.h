#ifndef __STACK__
#define __STACK__
void opStackInit(slot**);
#ifndef USE_STACK_MACROS
#ifdef AVR8
extern void opStackPush(const slot val);
extern slot opStackPop(void);
extern slot opStackPeek(void);
extern void opStackPoke(const slot val);
extern slot opStackGetValue(const u2 pos);
extern void opStackSetValue(const u2 pos,const slot val);
extern u2 opStackGetSpPos(void);
extern void opStackSetSpPos(const u2 pos);
#else
void opStackPush(const slot val);
slot opStackPop(void);
slot opStackPeek(void);
void opStackPoke(const slot val);
slot opStackGetValue(const u2 pos);
void opStackSetValue(const u2 pos,const slot val);
u2 opStackGetSpPos(void);
void opStackSetSpPos(const u2 pos);
#endif
#else

#ifndef AVR8
static slot* opSp;
static u2*   methodSp;
#else
slot*   opSp;
u2*     methodSp;
#endif

#define opStackPush(val) *(opSp++) = val
#define opStackPop()    (*(--opSp))
#define opStackPeek()   (*(opSp - 1))
#define opStackPoke(val)(*(opSp - 1)) = val
#define opStackSetValue(pos,val)    (*(opStackBase + pos)) = val
#define opStackGetValue(pos) (*( opStackBase + pos))
#define opStackGetSpPos()   (opSp - opStackBase)
#define opStackSetSpPos(pos) opSp = pos + opStackBase

#endif

void methodStackInit(u2** m);

#ifndef USE_STACK_MACROS
#ifdef AVR8
extern void methodStackPush(const u2 val);
extern u2 methodStackPop(void);
extern u2 methodStackPeek(void);
extern u2 methodStackGetSpPos(void);
extern void methodStackSetSpPos(const u2 pos);
extern u1 methodStackEmpty(void);
#else
void methodStackPush(const u2 val);
u2 methodStackPop(void);
u2 methodStackPeek(void);
u2 methodStackGetSpPos(void);
void methodStackSetSpPos(const u2 pos);
u1 methodStackEmpty(void);
#endif
#else

#define methodStackPush(val) *(methodSp++) = val
#define methodStackPop()    (*(--methodSp))
#define methodStackPeek()   (*(methodSp - 1))
#define methodStackGetSpPos()   (methodSp - methodStackBase)
#define methodStackSetSpPos(pos)   methodSp = pos + methodStackBase
#define methodStackEmpty() (methodSp == methodStackBase) ? 1 : 0

#endif

#endif
