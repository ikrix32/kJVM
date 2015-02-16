/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* fuer lehrzwecke,...*/
/* 14.8.06 sie lief das erste mal durch (in Unix)*/
/* april 2007-> threads, scheduling, funtionsüberladung, native-dispatch*/
/* mai 2007 -> garbage collection, exception handling*/
/* jan08 synchronized, wait notify*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "typedefinitions.h"
#include "classfile.h"
#include "definitions.h"
#include "bajvm.h"
#include "stack.h"
#include "heap.h"
#include "interpreter.h"
#include "scheduler.h"

#ifdef AVR8

#include <avr/pgmspace.h>

#define GETSTARTPC(offset)  ((strncmpRamFlash(  "Code",\
getAddr( cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+8+offset)]+3  ),\
4)==0)? (u2)METHODBASE(cN,mN)+8+14+offset :\
GETSTARTPC(offset+getU4(METHODBASE(cN,mN)+8)+6))

#else

#define GETSTARTPC(offset)  ((strncmp(  "Code",\
getAddr( cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+8+offset)]+3  ),\
4)==0)? (u2)METHODBASE(cN,mN)+8+14+offset :\
GETSTARTPC(offset+getU4(METHODBASE(cN,mN)+8)+6))

#endif

#define fieldName       name
#define fieldNameLength     nameLength
#define lengthArray     nameLength
#define methodName      name
#define methodNameLength    nameLength
#define fieldDescr      descr
#define fieldDescrLength    descrLength
#define methodDescr     descr
#define methodDescrLength   descrLength

static slot first;
static slot second;
static slot third;
static slot fourth;
static char* className;
static u2 classNameLength;
static char* name;                                /* field or method*/
static u2 nameLength;
static char* descr;                               /* field or method*/
static u2 descrLength;
/* static u1	numFields; <-- not used?*/
static u2 i, j, k;
static s2 count;

void run()                                        /* in: classNumber,  methodNumber cN, mN*/
{
    u1 code, byte1, byte2;
    u2 heapPos;
    pc = getStartPC();
    for (;;)
    {
        code = getU1(0);
        byte1 = getU1(pc);
        byte2 = getU1(pc + 1);
        DEBUGPRINT("-> ");

#ifndef TINYBAJOS_MULTITASKING
        DEBUGPRINTE(currentThreadCB->tid, x);
#endif
        DEBUGPRINT(", pc: %x ", pc - getStartPC() - 1);
        DEBUGPRINTE(code, 2x);
        DEBUGPRINTE(byte1, 2x);
        DEBUGPRINTE(byte2,2x\t);
        DEBUGPRINT("sp: %d local: %d\n", opStackGetSpPos(), local);
        DEBUGPRINTSTACK;
        DEBUGPRINTLOCALS;
        DEBUGPRINT("\t\t\t\t");

        switch (code)
        {

            case NOP:
            {
                DEBUGPRINTLN("NOP");
            }break;
            case ACONST_NULL:
            {
                DEBUGPRINTLN("aconst_null -> push\t...,=> NULLOBJECT");
                opStackPush(NULLOBJECT);
            }break;
            case ICONST_M1:
            case ICONST_0:
            case ICONST_1:
            case ICONST_2:
            case ICONST_3:
            case ICONST_4:
            case ICONST_5:
            {
                DEBUGPRINTLN("ICONST_%4x -> push\t...,=> %4x",code-ICONST_0,code - ICONST_0);
                opStackPush(toSlot((s4)(code - ICONST_0)));
            }break;
            case FCONST_0:
            case FCONST_1:
            case FCONST_2:
            {
                DEBUGPRINTLN("FCONST_%d  -> push\t...,=> %f",code - FCONST_0,(f4)(code - FCONST_0));
                opStackPush(toSlot((f4)(code - FCONST_0)));
            }break;
            case BIPUSH:
            {
                DEBUGPRINTLN("BIPUSH  -> push\t...,=> %d",(s1)byte1);
                /* BIPUSH is defined as follows:*/
                /* reads a signed 8 bit constant from byte1,*/
                /* extends it to int signed (32bit)*/
                /* and saves it on stack*/
                opStackPush(toSlot(((getU1(pc) & 0x80) >> 7) * 0xffffff80 | (getU1(0)
                                                                             & 0x7f)));
            }break;
            case SIPUSH:
            {
                DEBUGPRINTLN("SIPUSH  -> push\t...,=> %x",(s2)BYTECODEREF);
                opStackPush(toSlot((s4)((s2) getU2(0))));
            }break;
            case LDC:
            {
                DEBUGPRINT("ldc  push\t...");
                if (getU1(CP(cN, getU1(0))) == CONSTANT_String)
                {
                    first.stackObj.magic = CPSTRINGMAGIC;
                    first.stackObj.classNumber = cN;
                    first.stackObj.pos = (u2)(/*((u2)cN <<8)*/+byte1);
                    opStackPush(first);
                } else
                /* int or float const value on stack*/
                    opStackPush(toSlot( getU4(CP(cN, byte1) + 1) ));
                DEBUGPRINTLN(",=> x%x", opStackPeek().UInt);
            }break;
            case LDC_W:
            {
                errorExit(-4, "LDC_W not yet realized\n");
            }break;
            case ILOAD:
            {
                DEBUGPRINTLN("ILOAD -> local(%x) -> push\t...,=>",byte1);
                /*mb jf*/
                opStackPush(opStackGetValue(local + getU1(0)));
            }break;
            case FLOAD:
            {
                DEBUGPRINTLN("FLOAD -> local(%d: %d) -> push\t...,=>",byte1,getU1(byte1));
                opStackPush(opStackGetValue(local + getU1(0)));
            }break;
            case ALOAD:
            {
                DEBUGPRINTLN("aload -> local(%x) -> push\t,=>",byte1);
                /*mb jf changed getU1() --> getU1(0)*/
                opStackPush(opStackGetValue(local + getU1(0)));
            }break;
            case ILOAD_0:
            case ILOAD_1:
            case ILOAD_2:
            case ILOAD_3:
            {
                DEBUGPRINTLN("ILOAD_%d local -> push\t...,=>",code - ILOAD_0);
                opStackPush(opStackGetValue(local + code - ILOAD_0));
            }break;
            case FLOAD_0:
            case FLOAD_1:
            case FLOAD_2:
            case FLOAD_3:
            {
                DEBUGPRINTLN("FLOAD_%d local -> push\t...,=>",code - FLOAD_0);
                opStackPush(opStackGetValue(local + code - FLOAD_0));
            }break;
            case ALOAD_0:
            case ALOAD_1:
            case ALOAD_2:
            case ALOAD_3:
            {
                DEBUGPRINTLN("aload_%d local -> push\t...,=>",code - ALOAD_0);
                opStackPush(opStackGetValue(local + code - ALOAD_0));
            }break;
            case IALOAD:
            case FALOAD:
            case AALOAD:
            case BALOAD:
            case CALOAD:
            case SALOAD:
            {
                count = (s2)opStackPop().Int;     /*mb jf*/
                first = opStackPop();
#ifdef DEBUG
                switch (code)
                {
                    case IALOAD: DEBUGPRINTLN("iaload");break;
                    case FALOAD: DEBUGPRINTLN("faload");break;
                    case AALOAD: DEBUGPRINTLN("aaload");break;
                    case BALOAD: DEBUGPRINTLN("baload");break;
                    case CALOAD: DEBUGPRINTLN("caload");break;
                    case SALOAD: DEBUGPRINTLN("saload");break;
                }
                PRINTF("%x, =>",first.UInt);
#endif
                lengthArray = first.stackObj.arrayLength;
                if (first.UInt == NULLOBJECT.UInt)
                {
                    NULLPOINTEREXCEPTION;
                } else if (count < 0 || lengthArray <= count || count > (MAXHEAPOBJECTLENGTH - 1))
                {
                    ARRAYINDEXOUTOFBOUNDSEXCEPTION;
                }
                else
                {
                    opStackPush(heapGetElement((u2) first.stackObj.pos + count + 1));
                    DEBUGPRINTLN(", %x", opStackPeek().UInt);
                }
            }break;
            case ISTORE:
            case FSTORE:
            case ASTORE:
            {
#ifdef DEBUG
                switch (code)
                {
                    case ISTORE: DEBUGPRINTLN("ISTORE  pop -> local(%d)=>,\n",byte1);break;
                    case FSTORE: DEBUGPRINTLN("FSTORE  pop -> local(%d)=>,\n",byte1);break;
                    case ASTORE: DEBUGPRINTLN("ASTORE  pop -> local(%x)=>,\n",byte1);break;
                }
#endif
                opStackSetValue(local+getU1(0),opStackPop());

            }break;
            case ISTORE_0:
            case ISTORE_1:
            case ISTORE_2:
            case ISTORE_3:
            {
                DEBUGPRINTLN("ISTORE_%d pop -> local   %d=>,",code-ISTORE_0,opStackPeek().Int);
                opStackSetValue(local + code - ISTORE_0, opStackPop());
            }break;
            case FSTORE_0:
            case FSTORE_1:
            case FSTORE_2:
            case FSTORE_3:
            {
                DEBUGPRINTLN("FSTORE_%d pop -> local   =>,",code - FSTORE_0);
                opStackSetValue(local + code - FSTORE_0, opStackPop());
            }break;
            case ASTORE_0:
            case ASTORE_1:
            case ASTORE_2:
            case ASTORE_3:
            {
                DEBUGPRINTLN("ASTORE_%d pop -> local  =>,",code-ASTORE_0);
                opStackSetValue(local + code - ASTORE_0, opStackPop());
            }break;
            case IASTORE:
            case FASTORE:
            case AASTORE:
            case BASTORE:
            case CASTORE:
            case SASTORE:
            {
#ifdef DEBUG
                switch (code)
                {
                        /*mb jf		//int*/
                    case IASTORE: DEBUGPRINTLN("iastore stack -> local");break;
                        /*mb jf		//float*/
                    case FASTORE: DEBUGPRINTLN("fastore");break;
                        /*mb jf		//float*/
                    case AASTORE: DEBUGPRINTLN("fastore");break;
                        /*mb jf		//byte or boolean*/
                    case BASTORE: DEBUGPRINTLN("bastore");break
                        /*mb jf		//char*/
                    case CASTORE: DEBUGPRINTLN("castore");break;
                        /*mb jf		//short*/
                    case SASTORE: DEBUGPRINTLN("sastore");break;
                }
#endif
                second = opStackPop();
                count = (s2)(opStackPop().Int);
                first = opStackPop();
                lengthArray = first.stackObj.arrayLength;
                if (first.UInt == NULLOBJECT.UInt)
                {
                    NULLPOINTEREXCEPTION;
                } else if (count < 0 || lengthArray <= count || count
                           > (MAXHEAPOBJECTLENGTH - 1))
                {
                    ARRAYINDEXOUTOFBOUNDSEXCEPTION;
                }
                else
                {
                    heapSetElement(second, first.stackObj.pos + count + 1);
                }
            }break;
            case POP:
            {
                DEBUGPRINTLN("POP %x",opStackPeek().UInt);
                opStackPop();
            }break;
            case POP2:
            {
                DEBUGPRINTLN("POP2");
                opStackPop();
                opStackPop();
            }break;
            case DUP:
            {
                DEBUGPRINTLN("dup");
                opStackPush(opStackPeek());
            }break;
            case DUP_X1:
            {
                DEBUGPRINTLN("DUP_X1");
                second = opStackPop();
                first = opStackPop();
                opStackPush(second);
                opStackPush(first);
                opStackPush(second);
            }break;
            case DUP_X2:
            {
                DEBUGPRINTLN("DUP_X2");
                second = opStackPop();
                first = opStackPop();
                third = opStackPop();
                opStackPush(second);
                opStackPush(third);
                opStackPush(first);
                opStackPush(second);
            }break;
            case DUP2:
            {
                DEBUGPRINTLN("DUP2");
                second = opStackPop();
                first = opStackPop();
                opStackPush(first);
                opStackPush(second);
                opStackPush(first);
                opStackPush(second);
            }break;
            case DUP2_X1:
            {
                DEBUGPRINTLN("DUP2_X1");
                second = opStackPop();
                first = opStackPop();
                third = opStackPop();
                opStackPush(first);
                opStackPush(second);
                opStackPush(third);
                opStackPush(first);
                opStackPush(second);
            }break;
            case DUP2_X2:
            {
                DEBUGPRINTLN("DUP2_X2");
                second = opStackPop();
                first = opStackPop();
                third = opStackPop();
                fourth = opStackPop();
                opStackPush(first);
                opStackPush(second);
                opStackPush(fourth);
                opStackPush(third);
                opStackPush(first);
                opStackPush(second);
            }break;
            case SWAP:
            {
                DEBUGPRINTLN("SWAP");
                second = opStackPop();
                first = opStackPop();
                opStackPush(second);
                opStackPush(first);
            }break;
            case IADD:
            {
                DEBUGPRINTLN("IADD");
                opStackPoke(toSlot((opStackPop().Int + opStackPeek().Int)));
            }break;
            case FADD:
            {
                DEBUGPRINTLN("FADD");
                opStackPoke(toSlot((opStackPop().Float + opStackPeek().Float)));
            }break;
            case ISUB:
            {
                DEBUGPRINTLN("ISUB");
                first = opStackPop();             /*mb fj changed substraction order*/
                opStackPoke(toSlot((opStackPeek().Int - first.Int)));
            }break;
            case FSUB:
            {
                DEBUGPRINTLN("Fsub");
                first = opStackPop();             /*mb fj changed substraction order*/
                opStackPoke(toSlot((opStackPeek().Float - first.Float)));
            }break;
            case IMUL:
            {
                DEBUGPRINTLN("IMUL");
                opStackPoke(toSlot((opStackPop().Int * opStackPeek().Int)));
            }break;
            case FMUL:
            {
                DEBUGPRINTLN("FMUL");
                opStackPoke(toSlot((opStackPop().Float * opStackPeek().Float)));
            }break;
            case IDIV:
            {
                DEBUGPRINTLN("IDIV");
                first = opStackPop();             /*mb fj changed dividend order*/
                if (first.Int == 0)
                    ARITHMETICEXCEPTION;
                else
                    opStackPush(toSlot((opStackPop().Int / first.Int)));
            }break;
            case FDIV:
            {
                DEBUGPRINTLN("FDIV");
                first = opStackPop();             /*mb fj changed dividend order*/
                if (first.Float == 0.0)
                    ARITHMETICEXCEPTION;
                else
                    opStackPoke(toSlot((opStackPeek().Float / first.Float)));
            }break;
            case IREM:
            {
                DEBUGPRINTLN("IREM");
                if (((first = opStackPop()).Int) == 0)
                    ARITHMETICEXCEPTION;
                else
                    opStackPoke(toSlot((opStackPeek().Int % first.Int)));
            }break;
            case FREM:
            {
                DEBUGPRINTLN("FREM");
                float divisor = opStackPop().Float;
                float dividend = opStackPop().Float;
                int q = dividend / divisor;
                opStackPush(toSlot((f4)(dividend - (divisor * q))));
            }break;
            case INEG:
            {
                DEBUGPRINTLN("INEG");
                opStackPoke(toSlot(-opStackPeek().Int));
            }break;
            case FNEG:
            {
                DEBUGPRINTLN("FNEG");
                opStackPoke(toSlot(-opStackPeek().Float));
            }break;
            case ISHL:
            {
                DEBUGPRINTLN("ISHL");
                opStackPoke(toSlot(opStackPop().UInt << opStackPeek().UInt));
            }break;
            case ISHR:
            {
                DEBUGPRINTLN("ISHR");
                opStackPoke(toSlot(opStackPop().Int >> opStackPeek().Int));
            }break;
            case IUSHR:
            {
                DEBUGPRINTLN("IUSHR");
                first = toSlot(opStackPop().Int & 0x0000001f);
                second = opStackPop();
                if (second.Int < 0)
                    opStackPush(toSlot((second.Int >> first.Int) + (2 << ~first.Int)));
                else
                    opStackPush(toSlot(second.Int >> first.Int));
            }break;
            case IAND:
            {
                DEBUGPRINTLN("IAND");
                opStackPoke(toSlot(opStackPop().UInt & opStackPeek().UInt));
            }break;
            case IOR:
            {
                DEBUGPRINTLN("IOR");
                opStackPoke(toSlot(opStackPop().UInt | opStackPeek().UInt));
            }break;
            case IXOR:
            {
                DEBUGPRINTLN("IXOR");
                opStackPoke(toSlot(opStackPop().UInt ^ opStackPeek().UInt));
            }break;
            case IINC:
            {
                DEBUGPRINTLN("IINC");             /*mb, jf*/
                /* position*/
                opStackSetValue((u2)(local + byte1),
                                /* old value*/
                                toSlot((s4)(opStackGetValue(local + byte1).Int
                                            + (s4) (s1) byte2)));         /* add const*/
                pc += 2;                          /* to skip the index + const*/

            }break;
            case I2F:
            {
                DEBUGPRINTLN("I2F");
                opStackPoke(toSlot((f4) opStackPeek().Int));
            }break;
            case F2I:
            {
                DEBUGPRINTLN("F2I");
                opStackPoke(toSlot((s4)(opStackPeek().Float)));
            }break;
            case I2C:
            {
                DEBUGPRINTLN("I2C");
                opStackPoke(toSlot((opStackPeek().UInt & 0x0000ffff)));
            }break;
            case I2B:
            {
                DEBUGPRINTLN("I2B");
                opStackPoke(toSlot(opStackPeek().UInt & 0x000000ff));
            }break;
            case I2S:
            {
                DEBUGPRINTLN("I2S");
                opStackPoke(toSlot((s4)((s2) opStackPeek().Int)));
            }break;
            case FCMPL:
            case FCMPG:
            {
                DEBUGPRINTLN("fcmpg");
                second = opStackPop();
                first = opStackPop();
                if (first.Float == 0x7fc00000 || second.Float == 0x7fc00000)
                {
                    opStackPush(toSlot((s4)(code == FCMPG ? 1 : -1)));
                }
                else if (first.Float > second.Float)
                {
                    opStackPush(toSlot((s4) 1));
                }                                 /* corrected by al june 08*/
                else if (first.Float == second.Float)
                {
                    opStackPush(toSlot((s4) 0));
                }
                else
                {
                    opStackPush(toSlot((s4) - 1));
                }
            }break;
            case IFEQ:
            {
                DEBUGPRINTLN("ifeq");             /*mb, jf*/
                if (opStackPop().Int == 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IFNULL:
            {
                DEBUGPRINTLN("ifnull");
                if (opStackPop().UInt == NULLOBJECT.UInt)
                    pc += BYTECODEREF - 1;        /* add offset to pc at ifnull-address*/
                else
                    pc += 2;                      /* skip branch bytes*/
            }break;
            case IFNONNULL:
            {
                DEBUGPRINTLN("ifnonnull");        /* mb jf*/
                if (opStackPop().UInt != NULLOBJECT.UInt)
                    pc += BYTECODEREF - 1;        /* add offset to pc at ifnull-address*/
                else
                    pc += 2;                      /* skip branch bytes*/
            }break;
            case IFNE:
            {
                DEBUGPRINTLN("ifne");             /*mb, jf*/
                if (opStackPop().Int != 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IFLT:
            {
                DEBUGPRINTLN("iflt");             /*mb, jf*/
                if (opStackPop().Int < 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IFLE:
            {
                DEBUGPRINTLN("ifle");             /*mb, jf*/
                if (opStackPop().Int <= 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IFGT:
            {
                DEBUGPRINTLN("ifgt");             /*mb, jf*/
                if (opStackPop().Int > 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IFGE:
            {
                DEBUGPRINTLN("ifge");             /*mb, jf*/
                if (opStackPop().Int >= 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IF_ACMPEQ: DEBUGPRINTLN("if_acmpeq");        /*mb, jf*/
            case IF_ICMPEQ:
            {
                DEBUGPRINTLN("if_icmpeq");        /*mb, jf*/
                if (opStackPop().Int == opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IF_ACMPNE: DEBUGPRINTLN("if_acmpne");        /*mb, jf*/
            case IF_ICMPNE:
            {
                DEBUGPRINTLN("if_icmpne");        /*mb, jf*/
                if (opStackPop().Int != opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IF_ICMPLT:
            {
                DEBUGPRINTLN("if_icmplt");        /*mb, jf*/
                /*???*/
                if (opStackPop().Int > opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IF_ICMPGE:
            {
                DEBUGPRINTLN("if_icmpge");        /*mb, jf*/
                if (opStackPop().Int <= opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IF_ICMPGT:
            {
                DEBUGPRINTLN("if_icmpgt");        /*mb, jf*/
                if (opStackPop().Int < opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case IF_ICMPLE:
            {
                DEBUGPRINTLN("if_icmple");        /*mb, jf*/
                if (opStackPop().Int >= opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      /* to skip the jump-adress*/
            }break;
            case GOTO:
            {
                DEBUGPRINTLN("goto");             /* mb, jf*/
                pc += (s2) BYTECODEREF - 1;
            }break;
            case JSR:
            {
                DEBUGPRINTLN("jsr");              /* mb, jf*/
                opStackPush(toSlot((u4)(pc + 2)));
                pc += (s2) BYTECODEREF - 1;
            }break;
            case RET:
            {
                DEBUGPRINTLN("ret");              /* mb, jf*/
                pc = opStackGetValue(local + getU1(0)).UInt;
            }break;
            case TABLESWITCH:
            {
                DEBUGPRINTLN("tableswitch");      /* mb, jf*/
                {
                    /*
                     aa		tableswitch
                     0  0  0 	padding
                     0 0 0 2		DefaultByte
                     0 0 0 1		LowByte
                     0 0 0 3		HighByte
                     0  0  0 1c	Offset für 1
                     0  0  0 22	Offset für 2
                     0  0  0 28	Offset für 3
                     */

                    u2 startPc = --pc;
                    u2 relPc = pc - getStartPC(); /*pcMethodStart;	//calculate relative PC for ByteCode in Method*/
                    u4 windex = opStackPop().Int;
                    /* next pc as multiple of 4 --> skip padding bytes*/
                    relPc = (u2)((relPc + 4) & 0xfffc);
                    pc = relPc + getStartPC();    /*pcMethodStart;	// set pc to begin of default address*/
                    u4 offset = getU4(0);         //(u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));	// default offset
                    u4 lowbyte = getU4(0);        //(u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
                    u4 highbyte = getU4(0);       //(u4)((u4)getU1(pc++)<<24 | (u4)getU1(pc++)<<16 | (u4)getU1(pc++)<<8 | getU1(pc++));
                    if (lowbyte <= windex && windex <= highbyte)
                    {
                        u4 tableoffset = windex - lowbyte;
                        pc += tableoffset * 4;    /* skip 4 byte of previous address(es)*/
                        offset = getU4(0);        //(u4)((u4)getU1(0)<<24 | (u4)getU1(0)<<16 | (u4)getU1(0)<<8 | (u4)getU1(0));
                    }
                    pc = startPc + offset;
                }
            }break;
            case LOOKUPSWITCH:
            {
                DEBUGPRINTLN("lookupswitch");     /* mb, jf*/
                {
                    /*
                     ab          lookupswitch
                     0  0        padding
                     0  0  0 25  defaultByte
                     0  0  0  2  npairs
                     0  0  0  1  match
                     0  0  0 1b  offset
                     0  0  0  2  match
                     0  0  0 20  offset
                     */
                    u2 startPc = --pc;
                    u2 relPc = pc - getStartPC(); /*pcMethodStart;	//calculate relative PC for ByteCode in Method*/
                    u4 key = opStackPop().Int;

                    /* next pc as multiple of 4 from address of 0xab (lookupswitch)*/
                    relPc = (u2)((relPc + 4) & 0xfffc);
                    pc = relPc + getStartPC();    /* pcMethodStart;	// set pc to begin of default address*/
                    u4 offset = getU4(0);         /* default offset*/
                    u4 matches;
                    for (matches = getU4(0); matches > 0; --matches)
                    {
                        u4 match = getU4(0);
                        u4 tmpOffset = getU4(0);
                        if (key == match)
                        {
                            offset = tmpOffset;
                            break;
                        }
                    }
                    pc = startPc + offset;
                }
            }break;
            case GETSTATIC:
            {
                DEBUGPRINTLN("getstatic ");       /*mb jf ... corrected funtion*/
                methodStackPush(cN);
                fieldName = (char*) getAddr(CP(cN,/* utf8*/
                                               getU2(                       /* name-index*/
                                                     /* index to name and type*/
                                                     CP(cN, getU2(CP(cN, BYTECODEREF) + 3))
                                                     + 1)) + 3);                   /* bytes*/
                fieldNameLength = getU2(
                                        /* length*/
                                        CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 1)) + 1);
                /* utf8*/
                fieldDescr = (char*) getAddr(CP(cN,
                                                getU2(                        /* descriptor-index*/
                                                      /* index to name and type*/
                                                      CP(cN, getU2(CP(cN, BYTECODEREF) + 3))
                                                      + 3)) + 3);                   /* bytes*/
                fieldDescrLength = getU2(/* length*/
                                         CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 3)) + 1);

                const char* cclassName = getAddr(CP(cN,getU2( CP(cN, getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 3);
                const u1 cclassNameLength = getU2(CP(cN,getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 1))+ 1)) + 1);
                //printf("Class name: %s,%d\n",cclassName,cclassNameLength);
                if (!FIND_CLASS(cclassName,cclassNameLength))
                {
                    CLASSNOTFOUNDERR(cclassName,cclassNameLength);
                }

                if (!findStaticFieldByName(fieldName, fieldNameLength, fieldDescr,fieldDescrLength))
                {
                    FIELDNOTFOUNDERR(fieldName,
                                     getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 3));
                }
                /* got position in constant pool --> results in position on heap*/
                DEBUGPRINTLNSTRING(fieldName, fieldNameLength);
                opStackPush(heapGetElement(cs[cN].classInfo.stackObj.pos + fN + 1));
                pc += 2;
                cN = methodStackPop();	// end GETSTATIC

            }break;
            case PUTSTATIC:
            {   /*mb jf*/
                DEBUGPRINTLN("putstatic -> stack in static field");
                methodStackPush(cN);
                fieldName = (char*) getAddr(CP(cN,/* utf8*/
                                               getU2(                        /* name-index*/
                                                     /* index to name and type*/
                                                     CP(cN, getU2(CP(cN, BYTECODEREF) + 3))
                                                     + 1)) + 3);                   /* bytes*/
                fieldNameLength = getU2(
                                        /* length*/
                                        CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 1)) + 1);
                /* utf8*/
                fieldDescr = (char*) getAddr(CP(cN,
                                                getU2(                        /* descriptor-index*/
                                                      /* index to name and type*/
                                                      CP(cN, getU2(CP(cN, BYTECODEREF) + 3))
                                                      + 3)) + 3);                   /* bytes*/
                fieldDescrLength = getU2(CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 3)) + 1);

                if (!FIND_CLASS(getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 3),
                                getU2(CP(cN,getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 1)))
                {
                    CLASSNOTFOUNDERR(getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 3),
                                     getU2(CP(cN,getU2(CP(cN,getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 1));
                }

                if (!findStaticFieldByName(fieldName, fieldNameLength, fieldDescr, fieldDescrLength))
                {
                    FIELDNOTFOUNDERR(fieldName,
                                     getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 3));
                }

                heapSetElement(opStackPop(),cs[cN].classInfo.stackObj.pos + fN + 1);
                pc += 2;

                cN = methodStackPop();            /* restore cN*/
                // end PUTSTATIC
            }break;
            case GETFIELD:
            {
                DEBUGPRINTLN("getfield ->   heap to stack:");
                methodStackPush(cN);
                first = opStackPop();
                fieldName = (char*) getAddr(CP(cN,/* utf8*/
                                               getU2(/* name-index*/
                                                     /* index to name and type*/
                                                     CP(cN, getU2(CP(cN, BYTECODEREF) + 3))
                                                     + 1)) + 3);                   /* bytes*/
                fieldNameLength = getU2(/* length*/
                                        CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 1)) + 1);
                /* utf8*/
                fieldDescr = (char*) getAddr(CP(cN,getU2(                        /* descriptor-index*/
                                                         /* index to name and type*/
                                                         CP(cN, getU2(CP(cN, BYTECODEREF) + 3))
                                                         + 3)) + 3);                   /* bytes*/
                fieldDescrLength = getU2(/* length*/
                                         CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 3)) + 1);
                /*
                 #ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
                 printf_P(PSTR("GETFIELD popRef %x von findClass %x numfields von dieser kl.%x"),first,cN,numFields);
                 #else
                 printf("GETFIELD popRef %x von findClass %x numfields von dieser kl.%x",first,cN,numFields);
                 #endif
                 */
                /* findClass liefert nur die Klasse, in der das field verwendet wird*/
                /* nicht die Klasse in der das field definiert wird (kann eine superclass sein)*/
                /* jetzt ist es noch besser, im stackobject die classnummer zu ermitteln*/

                cN = first.stackObj.classNumber;
                if (!findFieldByName(fieldName, fieldNameLength, fieldDescr, fieldDescrLength))
                {
                    FIELDNOTFOUNDERR(fieldName,
                                     getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 3));
                }
                opStackPush(toSlot(heapGetElement(first.stackObj.pos + fN+ 1).Int));
                pc += 2;
                cN = methodStackPop();
                // end GETFIELD
            }break;
            case PUTFIELD:
            {
                DEBUGPRINTLN("putfield -> stack to heap");
                methodStackPush(cN);
                {
                    /* mb jf print name*/
                    fieldName = (char*) getAddr(CP(cN, /* utf8*/
                                                   getU2(                    /* name-index*/
                                                         /* index to name and type*/
                                                         CP(cN, getU2(CP(cN, BYTECODEREF) + 3))
                                                         + 1)) + 3);               /* bytes*/
                    fieldNameLength = getU2(
                                            CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 1))
                                            + 1);                     /* length*/
                    fieldDescr = (char*) getAddr(CP(cN, /* utf8*/
                                                    getU2(                    /* descriptor-index*/
                                                          /* index to name and type*/
                                                          CP(cN, getU2(CP(cN, BYTECODEREF) + 3))
                                                          + 3)) + 3);               /* bytes*/
                    fieldDescrLength = getU2(
                                             CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 3))
                                             + 1);                     /* length*/
                    /*kann weg*/
                    if (!FIND_CLASS(getAddr(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1))+3),
                                    getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1))+1)))
                    {
                        CLASSNOTFOUNDERR(
                                         getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3),
                                         getU2(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1));
                    }

                    first = opStackPop();         /*mb jf doesn't work without variable ?!?!*/
                    second = opStackPop();

                    if (second.UInt == NULLOBJECT.UInt)
                    {
                        pc += 2;
                        cN = methodStackPop();
                        NULLPOINTEREXCEPTION;
                    }
                    else
                    {

                        cN = second.stackObj.classNumber;
                        if (!findFieldByName(fieldName, fieldNameLength, fieldDescr, fieldDescrLength))
                        {
                            FIELDNOTFOUNDERR(
                                             fieldName,
                                             getAddr(CP(cN,getU2(CP(cN,getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 3));
                        }
                        /* jetzt hab ich alles*/
                        /* den Typ*/
                        /* die Stelle auf dem heap*/

                        if (STRNCMPRAMFLASH( "B",fieldDescr, 1) == 0)
                        {
                            /* Truncate Integer input for Byte output */
                            first.Int = first.Int & 0x000000ff;
                        }
                        /*
                         #ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
                         printf_P(PSTR("classnumber: %d nummer %d was von stack %d\n"),cN,i,val);
                         #else
                         printf("classnumber: %d nummer %d was von stack %d\n",cN,i,val);
                         #endif
                         */
                        heapSetElement(first, second.stackObj.pos + fN + 1);
                    }
                    pc += 2;

                    cN = methodStackPop();
                }
                // end PUTFIELD
            }break;
            case INVOKESPECIAL:
            case INVOKEVIRTUAL:
            case INVOKEINTERFACE:
            {
#ifdef DEBUG
                if (code == INVOKEVIRTUAL) DEBUGPRINT("invokevirtual: ");
                if (code == INVOKEINTERFACE) DEBUGPRINT("invokeinterface: ");
                if (code == INVOKESPECIAL) DEBUGPRINT("invoke special: ");
#endif
                methodStackPush(local);
                methodStackPush(cN);
                methodStackPush(mN);
                if (code == INVOKEINTERFACE)
                    pc += 2;
                methodStackPush(pc);
                k = findNumArgs(BYTECODEREF);
                /*(BYTECODEREF)-1));*/
                methodStackPush((opStackGetSpPos() - k - 1));
                /* method resolution*/
                /* nachdenken ->mhrmals benutzt*/
                local = opStackGetSpPos() - findNumArgs(BYTECODEREF) - 1;
                /* get cN from.stackObjRef*/
                /*  get method from cN or superclasses*/
                methodName = (char*) getAddr(
                                             CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 1)) + 3);
                methodNameLength = getU2(
                                         CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 1)) + 1);
                DEBUGPRINTLNSTRING(methodName, methodNameLength);
                methodDescr = (char*) getAddr(
                                              CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 3)) + 3);
                methodDescrLength = getU2(
                                          CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 3)) + 1);
                DEBUGPRINTLNSTRING(methodDescr, methodDescrLength);
                className = NULL;

                if(opStackGetValue(local).UInt == NULLOBJECT.UInt)
                {

                    //cN = methodStackPop();
                    NULLPOINTEREXCEPTION;
                    break;
                }

                if ((code == INVOKEVIRTUAL) || (code == INVOKEINTERFACE))
                {
                    /*bh2008*/
                    if (opStackGetValue(local).stackObj.magic == CPSTRINGMAGIC)
                    {
                        if (!findClass("java/lang/String", 16))
                        {
                            CLASSNOTFOUNDERR("java/lang/String", 16);
                        }
                    } else/*bh2007*/
                        cN = opStackGetValue(local).stackObj.classNumber;

                    className = (char*) getAddr( cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)] + 1)] + 3);
                    classNameLength = getU2(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)] + 1)] + 1);
                }/*INVOKESPECIAL*/
                else
                {
                    className = (char*) getAddr(CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 3);
                    classNameLength = getU2(CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 1);
                }
                //bh DEBUGPRINTLNSTRING(className,classNameLength);
                if (!findMethod(className, classNameLength, methodName,
                                methodNameLength, methodDescr, methodDescrLength))
                {
                    METHODNOTFOUNDERR(methodName, className);
                }

                opStackSetSpPos(opStackGetSpPos() + ((getU2(METHODBASE(cN, mN)) & ACC_NATIVE) ? 0 : findMaxLocals()));

#ifndef TINYBAJOS_MULTITASKING
                if (getU2(METHODBASE(cN, mN)) & ACC_SYNCHRONIZED)
                {
                    if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex == MUTEXNOTBLOCKED)
                    {
                        /* mutex is free, I (the thread) have not the mutex and I can get the mutex for the object*/
                        currentThreadCB->isMutexBlockedOrWaitingForObject = NULLOBJECT;
                        HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex = MUTEXBLOCKED;       /* get the lock*/
                        /* I had not the mutex for this object (but perhaps for others), now I have the look*/
                        for (i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)
                            if (currentThreadCB->hasMutexLockForObject[i].UInt != NULLOBJECT.UInt)
                                continue;
                            else
                                break;

                        if (i == MAXLOCKEDTHREADOBJECTS)
                        {
                            errorExit(-1, "too many locks\n");

                        }
                        /* entry for this object in the array of mutexed objects for the thread*/
                        /* count (before 0)*/
                        currentThreadCB->lockCount[i] = 1;
                        currentThreadCB->hasMutexLockForObject[i] = opStackGetValue(
                                                                                    local);
                    }                             /* mutex is blocked, is it my mutex ? have I always the lock ?*/
                    else
                    {
                        for (i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)
                            if (currentThreadCB->hasMutexLockForObject[i].UInt == opStackGetValue(local).UInt)
                                break;

                        /* another thread has the lock*/
                        if (i == MAXLOCKEDTHREADOBJECTS)
                        {
                            /*mutex blocked*/
                            currentThreadCB->state = THREADMUTEXBLOCKED;
                            currentThreadCB->isMutexBlockedOrWaitingForObject = opStackGetValue(local);
                            /* thread sleeps, try it later*/
                            /*(BYTECODEREF)+1); //native!!!*/
                            opStackSetSpPos(methodStackPop() + k + 1);
                            /* before invoke*/
                            pc = methodStackPop() - 1;
                            mN = methodStackPop();
                            cN = methodStackPop();
                            local = methodStackPop();
                            break;
                        }                         /* let the scheduler work*/
                        else
                        /* yes I have lock*/
                        /* count*/
                            currentThreadCB->lockCount[i]++;
                    }
                }
#endif
                /* no synchronized,or I have the lock*/
                /* now call method*/
                if (getU2(METHODBASE(cN, mN)) & ACC_NATIVE)
                {
                    if ((cs[cN].nativeFunction != NULL) && (cs[cN].nativeFunction[mN]
                                                            != NULL))
                    {
                        if (cs[cN].nativeFunction[mN]())
                            goto nativeValueReturn;
                        else
                            goto nativeVoidReturn;
                    }
                    else
                    {
                        errorExit(-3, "native method not found cN: %d mN: %d", cN, mN);
                    }
                }
                pc = getStartPC();
            }break;
            case INVOKESTATIC:
            {
                DEBUGPRINT("invoke static: ");    /* a static method*/
                methodStackPush(local);
                methodStackPush(cN);
                methodStackPush(mN);
                methodStackPush(pc);
                k = findNumArgs(BYTECODEREF);
                /*(BYTECODEREF));*/
                methodStackPush(opStackGetSpPos() - k);
                /* method resolution*/
                /*bh2007*/
                local = (u2) opStackGetSpPos() - findNumArgs(BYTECODEREF);
                className = (char*) getAddr(CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 3);
                classNameLength = getU2(CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 1)) + 1)) + 1);

                methodName = (char*) getAddr(CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 1)) + 3);
                methodNameLength = getU2(CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 1)) + 1);
                methodDescr = (char*) getAddr(CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 3)) + 3);
                methodDescrLength = getU2(CP(cN, getU2(CP(cN, getU2(CP(cN, BYTECODEREF) + 3)) + 3)) + 1);

                if (!findMethod(className, classNameLength, methodName,
                                methodNameLength, methodDescr, methodDescrLength))
                {
                    METHODNOTFOUNDERR(methodName, className);
                }
                DEBUGPRINTLNSTRING(methodName, methodNameLength);
                DEBUGPRINTLNSTRING(className, classNameLength);
                opStackSetSpPos(opStackGetSpPos() + ((getU2(METHODBASE(cN, mN)) & ACC_NATIVE) ? 0 : findMaxLocals()));

#ifndef TINYBAJOS_MULTITASKING
                if (getU2(METHODBASE(cN, mN)) & ACC_SYNCHRONIZED)
                {
                    /*
                     #ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
                     printf_P(PSTR("invoke sync static %04x %d %d\n"),cs[cN].classInfo,cN,mN);
                     #else
                     printf("invoke sync static %04x %d %d\n",cs[cN].classInfo,cN,mN);
                     #endif
                     */

                    if (HEAPOBJECTMARKER(cs[cN].classInfo.stackObj.pos).mutex
                        == MUTEXNOTBLOCKED)
                    {
                        currentThreadCB->isMutexBlockedOrWaitingForObject = NULLOBJECT;
                        HEAPOBJECTMARKER(cs[cN].classInfo.stackObj.pos).mutex   = MUTEXBLOCKED;       /* get the lock*/
                        for (i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)
                            if (currentThreadCB->hasMutexLockForObject[i].UInt != NULLOBJECT.UInt)
                                continue;
                            else
                                break;
                        if (i == MAXLOCKEDTHREADOBJECTS)
                        {
                            errorExit(-1, "too many locks\n");
                        }
                        /* count*/
                        currentThreadCB->lockCount[i] = 1;
                        currentThreadCB->hasMutexLockForObject[i] = cs[cN].classInfo;
                    }                             /* mutex ==0*/
                    else
                    {
                        /* have I always the lock ?*/
                        for (i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)
                            if (currentThreadCB->hasMutexLockForObject[i].UInt == cs[cN].classInfo.UInt)
                                break;

                        if (i == MAXLOCKEDTHREADOBJECTS)
                        {
                            /*mutex blocked*/
                            currentThreadCB->state = THREADMUTEXBLOCKED;
                            currentThreadCB->isMutexBlockedOrWaitingForObject
                            = cs[cN].classInfo;
                            /* thread sleeps, try it later*/
                            /*(BYTECODEREF));*/
                            opStackSetSpPos(methodStackPop() + k);
                            /* before invoke*/
                            pc = methodStackPop() - 1;
                            if (code == INVOKEINTERFACE)
                                pc -= 2;
                            mN = methodStackPop();
                            cN = methodStackPop();
                            local = methodStackPop();
                            break;                /* let the scheduler work*/
                        } else
                        /* yes I have the lock*/
                        /* count*/
                            currentThreadCB->lockCount[i]++;
                    }
                }
#endif
                /* no synchronized,or I have the lock*/
                /* now call the method*/
                if (getU2(METHODBASE(cN, mN)) & ACC_NATIVE)
                {
                    if ((cs[cN].nativeFunction != NULL) && (cs[cN].nativeFunction[mN]
                                                            != NULL))
                    {
                        if (cs[cN].nativeFunction[mN]())
                            goto nativeValueReturn;
                        else
                            goto nativeVoidReturn;
                    }
                    else
                    {
                        errorExit(-3, "native method not found cN: %d mN: %d", cN, mN);
                    }
                }
                pc = getStartPC();

            }break;
            nativeValueReturn:
            case IRETURN:
            case FRETURN:
            case ARETURN:
            {
                switch (code)
                {
                    case IRETURN:
                    {
                        DEBUGPRINT("i");          /*mb jf*/
                    }break;
                    case  FRETURN:
                    {
                        DEBUGPRINT("f");
                    }break;
                    case ARETURN:
                    {
                        DEBUGPRINT("a");
                    }break;
                }
                code=IRETURN;
            nativeVoidReturn:
                DEBUGPRINT("native ");
            }
            case RETURN:
            {
                DEBUGPRINTLN("return");
#ifndef TINYBAJOS_MULTITASKING
                if (getU2(METHODBASE(cN,mN))&ACC_SYNCHRONIZED)
                {
                    /* have I always the lock ?*/
                    if (getU2(METHODBASE(cN,mN))&ACC_STATIC)
                        first=cs[cN].classInfo;
                    else first=opStackGetValue(local);
                    /* must be in*/
                    for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)
                        if ((currentThreadCB->hasMutexLockForObject[i]).UInt==first.UInt) break;
                    /* fertig*/
                    if (currentThreadCB->lockCount[i]>1) currentThreadCB->lockCount[i]--;
                    else                          /* last lock*/
                    {
                        currentThreadCB->lockCount[i]=0;
                        /* give lock free*/
                        currentThreadCB->hasMutexLockForObject[i]=NULLOBJECT;
                        currentThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
                        HEAPOBJECTMARKER(first.stackObj.pos).mutex=MUTEXNOTBLOCKED;
                        ThreadControlBlock* myTCB=currentThreadCB;

                        u1 k,max;

                        for(i=0;i<(MAXPRIORITY);i++)
                        {
                            max=(threadPriorities[i].count);
                            myTCB=threadPriorities[i].cb;
                            for(k=0;k<max;k++)
                            {
                                if ((myTCB->isMutexBlockedOrWaitingForObject.UInt==first.UInt)&&
                                    (myTCB->state==THREADMUTEXBLOCKED))
                                {
                                    /*!!*/
                                    myTCB->state=THREADNOTBLOCKED;
                                    myTCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
                                }
                                myTCB=myTCB->succ;
                            }
                        }
                    }
                }
#endif
                if (methodStackEmpty())
                {
                    /*
                     #ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
                     printf_P(PSTR("empty method stack\n"));
                     #else
                     printf("empty method stack\n");
                     #endif
                     */
                    /*mb jf if not <clinit> you're done :-D*/
                    if (STRNCMP("<clinit>",(char*)findMethodByMethodNumber(),8) == 0)
                    {
                        DEBUGPRINTLN(" from <clinit>");

                        return;
                    }
                    else
                    {
#ifndef TINYBAJOS_MULTITASKING
                        PRINTEXITTHREAD("normally terminated Thread: %d\n",currentThreadCB->tid);
#else
                        free(methodStackBase);
                        free(opStackBase);
#endif
                        break;
                    }
                }
                first=opStackPop();               /*ret val*/
                opStackSetSpPos(methodStackPop());
                pc = methodStackPop()+2;
                mN = methodStackPop();
                cN = methodStackPop();
                local = methodStackPop();
                if (code== IRETURN) opStackPush(first);
            }break;
            case NEW:
            {
                DEBUGPRINT("new");
                pc += 2;
                methodStackPush(cN);
                methodStackPush(mN);
                if (!FIND_CLASS/* className*/
                    (getAddr(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+3),
                     /* classNameLength*/
                     getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1)) )
                {
                    mN = methodStackPop();
                    cN = methodStackPop();
                    CLASSNOTFOUNDERR((char *) getAddr(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+3),getU2(CP(cN,getU2(CP(cN, getU2(CP(cN,BYTECODEREF) + 1)) + 1)) + 1));
                }
                methodStackPush(cN);
                fN = 0;
                do 	{
                    for (i=0; i < getU2(cs[cN].fields_count); i++)	// count normal fields
                    {
                        u2 fielddescr = cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 4)];
                        u1 isNotObject =
                        STRNCMPRAMFLASH("L",(const char*) getAddr(fielddescr + 3), 1);
                        if ( (getU2(cs[cN].field_info[i]) & ACC_FINAL) && isNotObject) continue; // ignore static and non static primitive finals
                        if ( getU2(cs[cN].field_info[i]) & ACC_STATIC) continue;// ignore static
                        fN++;
                    }

                } while (findSuperClass());
                cN=methodStackPop();
                heapPos=getFreeHeapSpace(fN + 1);/* + marker*/       /* allocate on heap places for stackObject fields*/
                first.stackObj.pos=heapPos;
                first.stackObj.magic=OBJECTMAGIC;
                first.stackObj.classNumber=cN;
                DEBUGPRINTLN(" -> push %x\n",heapPos);
                opStackPush(first);               /* reference to.stackObject on opStack*/
                HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDNEWOBJECT;
                HEAPOBJECTMARKER(heapPos).magic=OBJECTMAGIC;
                HEAPOBJECTMARKER(heapPos).mutex = MUTEXNOTBLOCKED;
                for (i=0; i< fN; i++)        /* initialize the heap elements*/
                    heapSetElement(toSlot((u4)0), heapPos+i+1);

                mN = methodStackPop();
                cN = methodStackPop();
                /* className*/
                DEBUGPRINTLNSTRING(getAddr(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+3),
                                   getU2(CP(cN, getU2(CP(cN,BYTECODEREF)+1))+1));

            }break;
            case NEWARRAY:
            {
                DEBUGPRINTLN("newarray");         /* mb jf*/
                count = (s2)opStackPop().UInt;
                if (count < 0)
                {
                    NEGATIVEARRAYSIZEEXCEPTION;
                }
                if (count > (MAXHEAPOBJECTLENGTH-1))
                {
                    ARRAYINDEXOUTOFBOUNDSEXCEPTION;
                }

                /* + marker*/
                heapPos=getFreeHeapSpace(count+ 1);
                first.stackObj.pos=heapPos;
                first.stackObj.magic=OBJECTMAGIC;
                first.stackObj.arrayLength=(u1)count;
                opStackPush(first);
                HEAPOBJECTMARKER(heapPos).status=HEAPALLOCATEDARRAY;
                HEAPOBJECTMARKER(heapPos).magic=OBJECTMAGIC;
                HEAPOBJECTMARKER(heapPos).mutex = MUTEXNOTBLOCKED;
                heapPos++;
                switch (byte1)                    /* array type, init array with 0 on heap*/
                {
                    case T_BOOLEAN:
                    {
                        for (i=0; i<count; i++)
                            heapSetElement(toSlot((u4)0),heapPos++);
                    }break;
                    case T_CHAR:
                    {
                        for (i=0; i<count; i++)
                            heapSetElement(toSlot((u4)0),heapPos++);
                    }break;
                    case T_FLOAT:
                    {
                        for (i=0; i<count; i++)
                            heapSetElement(toSlot(0.f),heapPos++);
                    }break;
                    case T_DOUBLE:
                    {
                        DNOTSUPPORTED;
                    }break;
                    case T_BYTE:
                    {
                        for (i=0; i<count; i++)
                            heapSetElement(toSlot((u4)0),heapPos++);
                    }break;
                    case T_SHORT:
                    {
                        for (i=0; i<count; i++)
                            heapSetElement(toSlot((u4)0),heapPos++);
                    }break;
                    case T_INT:
                    {
                        for (i=0; i<count; i++)
                            heapSetElement(toSlot((u4)0),heapPos++);
                    }break;
                    case T_LONG:
                    {
                        DNOTSUPPORTED;
                    }break;
                }                                 /* switch*/
                pc++;                             /* skip type*/
            }break;
            case ANEWARRAY:
            {
                DEBUGPRINTLN("anewarray");        /* mb jf*/
                pc+=2;                            /* index into the constant_pool. Bajos performs no verification*/
                s2 *cnt = (s2 *) malloc(sizeof(s2));
                *cnt = 0;
                opStackPush(createDims(1, cnt));  /* call recursive function to allocate heap for arrays*/
                free (cnt);
            }break;
            case ARRAYLENGTH:
            {
                DEBUGPRINTLN("arraylength");      /* mb jf*/
                first = opStackPop();
                if (first.UInt == NULLOBJECT.UInt)
                {
                    NULLPOINTEREXCEPTION;
                }
                else
                {
                    opStackPush(toSlot((u4)first.stackObj.arrayLength));
                }

            }break;
            case MONITORENTER:
            {
#ifndef TINYBAJOS_MULTITASKING
                first=opStackPop();
                if ( HEAPOBJECTMARKER(first.stackObj.pos).mutex==MUTEXNOTBLOCKED)
                {
                    /* get the lock*/
                    HEAPOBJECTMARKER(first.stackObj.pos).mutex=MUTEXBLOCKED;
                    for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)
                        if ((currentThreadCB->hasMutexLockForObject[i]).UInt!=NULLOBJECT.UInt)
                            continue;
                        else break;
                    if (i==MAXLOCKEDTHREADOBJECTS)
                    {
                        errorExit(-1, "too many locks\n");
                    }
                    /* count*/
                    currentThreadCB->lockCount[i]=1;
                    currentThreadCB->hasMutexLockForObject[i]=first;
                }
                else                              /* mutex ==0*/
                {
                    /* have I always the lock ?*/
                    for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)
                        if (currentThreadCB->hasMutexLockForObject[i].UInt==first.UInt) break;
                    if (i==MAXLOCKEDTHREADOBJECTS)
                    {
                        /*mutex blocked*/
                        currentThreadCB->state=THREADMUTEXBLOCKED;
                        currentThreadCB->isMutexBlockedOrWaitingForObject=first;
                        /* thread sleeps, try it later*/
                        opStackSetSpPos(methodStackPop()+findNumArgs(BYTECODEREF)+1);
                        pc = pc-1;                /* before monitorenter*/
                        break;                    /* let the scheduler work*/
                    }
                    else                          /* yes I have lock*/
                    /* count*/
                        currentThreadCB->lockCount[i]++;
                }
#else
                DNOTSUPPORTED;
#endif
            }break;
            case MONITOREXIT:                 /* have I always the lock ?*/
            {
#ifndef TINYBAJOS_MULTITASKING
                first=opStackPop();
                /* must be in*/
                for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)
                    if (currentThreadCB->hasMutexLockForObject[i].UInt==first.UInt) break;
                /* fertig*/
                if (currentThreadCB->lockCount[i]>1) currentThreadCB->lockCount[i]--;
                else
                {
                    currentThreadCB->lockCount[i]=0;
                    /* give lock free*/
                    currentThreadCB->hasMutexLockForObject[i]=NULLOBJECT;
                    HEAPOBJECTMARKER(opStackGetValue(first.stackObj.pos).UInt).mutex=MUTEXNOTBLOCKED;

                    ThreadControlBlock* myTCB;
                    u2 max,k;
                    for(i=0;i<(MAXPRIORITY);i++)
                    {
                        max=(threadPriorities[i].count);
                        myTCB=threadPriorities[i].cb;
                        for(k=0;k<max;k++)
                        {
                            if (myTCB->isMutexBlockedOrWaitingForObject.UInt==opStackGetValue(first.stackObj.pos).UInt)
                            {
                                /*!!*/
                                myTCB->state=THREADNOTBLOCKED;
                                /*						myTCB->isMutexBlockedForObjectOrWaiting=NULLOBJECT.UInt;*/
                            }
                            myTCB=myTCB->succ;
                        }
                    }
                }
#else
                DNOTSUPPORTED;
#endif
            }break;
            case ATHROW:
            {
                DEBUGPRINTLN("athrow");
#ifndef TINYBAJOS_EXCEPTION
                handleException();
#else
                exit(-101);
#endif

            }break;
            case CHECKCAST:
            {
                DEBUGPRINTLN("checkcast");
                /* In general, we try to cast as much as possible.*/
                /* Only if we perfectly know that this cast is invalid, break it.*/
                first = opStackPeek();
                char performcheck = 1;
                char invalidcast = 0;
                /* a nullobject can always be casted */
                if (first.UInt != NULLOBJECT.UInt)
                {
                    /* the cast's target class */
                    u2 targetclass = getU2(0);
                    char *classname = getAddr(CP(cN, getU2(CP(cN,targetclass)+1))+3);
                    int len = getU2(CP(cN, getU2(CP(cN,targetclass)+1))+1);

                    /* we have to make some dirty hacks here
                     since we are not storing typing informations for arrays */
                    if (/*getU1(classname)=='['*/*classname == '[')
                    {
                        while (/*getU1(classname)=='['*/'[' == *classname)
                        {
                            /* we hope to get useful information
                             from the objects stored in the array.
                             this only takes the first object in the array,
                             yet it could be extended to gathering
                             all stored object's typing informations */
                            if (first.stackObj.magic != OBJECTMAGIC || first.UInt == NULLOBJECT.UInt || HEAPOBJECTMARKER(first.stackObj.pos).status != HEAPALLOCATEDARRAY)
                            {
                                invalidcast = 1;
                                performcheck = 0;
                                break;
                            }
                            first = heapGetElement(first.stackObj.pos + 1);
                            /* remove the leading '[' */
                            --len;
                            ++classname;
                        }
                        if (first.UInt == NULLOBJECT.UInt)
                        {
                            performcheck = 0;
                        }
                        /* A class identifier is Lclassname; */
                        if (
                            //getU1(classname)=='L'
                            'L' == *classname
                            )
                        {
                            len -= 2;
                            ++classname;
                        }
                        else
                        {
                            /* a primitive type */
                            performcheck = 0;
                        }
                    }

                    if (performcheck == 1)
                    {
                        methodStackPush(cN);
                        methodStackPush(mN);
                        if (!findClass(classname, len))
                        {
                            CLASSNOTFOUNDERR(classname,len);
                        }
                        u2 target = cN;
                        cN = first.stackObj.classNumber;
                        if (!checkInstance(target))
                        {
                            invalidcast = 1;
                        }
                        mN=methodStackPop();
                        cN=methodStackPop();
                    }
                }
                else
                {
                    pc += 2;
                }
                if (invalidcast == 1)
                {
                    opStackPop();
                    CLASSCASTEXCEPTION;
                }
            }break;
            case INSTANCEOF:
            {
                DEBUGPRINTLN("instanceof");
                first = opStackPop();
                u2 targetclass = getU2(0);
                char performcheck = 1;
                if (first.UInt != NULLOBJECT.UInt)
                {
                    char *classname = getAddr(CP(cN, getU2(CP(cN,targetclass)+1))+3);
                    int len = getU2(CP(cN, getU2(CP(cN,targetclass)+1))+1);

                    /* we have to make some dirty hacks here
                     since we are not storing typing informations for arrays */
                    if (/*getU1(classname)=='['*/*classname == '[')
                    {
                        while (
                               //getU1(classname)=='['
                               '[' == *classname
                               )
                        {
                            /* we hope to get useful information
                             from the objects stored in the array.
                             this only takes the first object in the array,
                             yet it could be extended to gathering
                             all stored object's typing informations */
                            if (first.stackObj.magic != OBJECTMAGIC || first.UInt == NULLOBJECT.UInt || HEAPOBJECTMARKER(first.stackObj.pos).status != HEAPALLOCATEDARRAY)
                            {
                                performcheck = 0;
                                opStackPush(toSlot((u4)0));
                                break;
                            }
                            first = heapGetElement(first.stackObj.pos + 1);
                            /* remove the leading '[' */
                            --len;
                            ++classname;
                        }
                        if (first.UInt == NULLOBJECT.UInt)
                        {
                            performcheck = 0;
                            opStackPush(toSlot((u4)1));
                        }
                        /* A class identifier is Lclassname; */
                        if (
                            //getU1(classname)=='L'
                            'L' == *classname
                            )
                        {
                            len -= 2;
                            ++classname;
                        }
                        else
                        {
                            /* a primitive type */
                            performcheck = 0;
                            opStackPush(toSlot((u4)1));
                        }
                    }

                    if (performcheck == 1)
                    {
                        methodStackPush(cN);
                        methodStackPush(mN);
                        if (!findClass(classname, len))
                        {
                            CLASSNOTFOUNDERR(classname,len);
                        }
                        u2 target = cN;
                        cN = first.stackObj.classNumber;
                        if (checkInstance(target))
                        {
                            opStackPush(toSlot((u4)1));
                        }
                        else
                        {
                            opStackPush(toSlot((u4)0));
                        }
                        mN=methodStackPop();
                        cN=methodStackPop();
                    }
                    else
                    {
                        opStackPush(toSlot((u4)0));
                    }
                }
                else
                {
                    opStackPush(toSlot((u4)0));
                }
            }break;
            case WIDE:
            {
                DEBUGPRINTLN("wide");             /* mb jf*/
                /* not tested because so many locals are hard to implement on purpose  14.12.2006*/
                u2 nextOp = getU1(0);             /* which operation to extend?*/
                count = getU2(0);

                /* if load operation...*/
                if (ILOAD <= nextOp && nextOp <= DLOAD)
                {
                    /* embedded op code for load*/
                    opStackPush(opStackGetValue(local+count));
                }
                /* if store operation...*/
                if (ISTORE <= nextOp && nextOp <= DSTORE)
                {
                    /* embedded op code for store*/
                    opStackSetValue(local+count,opStackPop());
                    /* write into locals (position, value from stack)*/
                }
                if (nextOp == RET)                /* if ret operation...*/
                {
                    /* embedded op code for ret*/
                    /*not tested because no exceptions implemented yet 14.12.2006*/
                    /* the opcode of athrow is required*/
                    u2 addr = opStackGetValue(local+count).UInt;
                    pc = addr+getStartPC();       /*pcMethodStart;	//assumtion: the address is the relative address, absolute address may be required*/
                }
                if (nextOp == IINC)               /* if iinc operation...*/
                {
                    /* embedded op code for load*/
                    u2 constB = getU2(0);         /* constByte - only available with iinc in wide operation*/
                    /* position*/
                    opStackSetValue((u2)(local + count),
                                    /* old value*/
                                    toSlot((u4)(opStackGetValue(local + count).Int
                                                + constB)));               /* add const*/
                }
            }break;
            case MULTIANEWARRAY:
            {
                DEBUGPRINTLN("multianewarray");   /* mb jf*/
                pc+=2;                            /* index into the constant_pool. Bajos performs no verification*/
                u1 dim = getU1(0);                /* dimensions*/

                s2 *local_cnt = (s2 *) malloc(sizeof(s2));
                *local_cnt = 0;
                /* call recursive function to allocate heap for arrays*/
                opStackPush(createDims(dim, local_cnt));
                free (local_cnt);
            }break;
            case GOTO_W:
            {   /* mb jf*/
                DEBUGPRINTLN("goto_w (not tested)");
                /* not tested because wide jumps are hard to implement on purpose  14.12.2006*/
                u4 addr = getU4(0);
                pc = addr +getStartPC();          /*pcMethodStart; //assumtion: the address is the relative address, absolute address may be required*/

            }break;
            case JSR_W:
            {   /* mb jf*/
                DEBUGPRINTLN("jsr_w (not tested)%d %d",byte1, byte2);
                /* not tested because no exceptions implemented yet 14.12.2006*/
                /* the opcode of athrow is required*/
                u4 my_addr = getU4(0);
                opStackPush(toSlot(my_addr));
            }break;
            case LCONST_0:
            case LCONST_1:
            case DCONST_0:
            case DCONST_1:
            case LDC2_W:
            case LLOAD:
            case DLOAD:
            case DLOAD_0:
            case DLOAD_1:
            case DLOAD_2:
            case DLOAD_3:
            case LALOAD:
            case DALOAD:
            case LSTORE:
            case DSTORE:
            case LSTORE_0:
            case LSTORE_1:
            case LSTORE_2:
            case LSTORE_3:
            case DSTORE_0:
            case DSTORE_1:
            case DSTORE_2:
            case DSTORE_3:
            case LASTORE:
            case DASTORE:
            case LADD:
            case DADD:
            case LSUB:
            case DSUB:
            case LMUL:
            case DMUL:
            case LDIV:
            case DDIV:
            case LREM:
            case DREM:
            case LNEG:
            case DNEG:
            case LSHL:
            case LSHR:
            case LUSHR:
            case LAND:
            case LOR:
            case LXOR:
            case I2L:
            case I2D:
            case L2I:
            case L2F:
            case L2D:
            case F2L:
            case F2D:
            case D2I:
            case D2L:
            case D2F:
            case LCMP:
            case DCMPL:
            case DCMPG:
            case DRETURN:
            case LRETURN:
            {
                DNOTSUPPORTED;
            }break;
        }/* switch*/
#ifndef TINYBAJOS_MULTITASKING
        scheduler();
#endif
    }//for (;;)

    verbosePrintf("Termination\n");

}


/* generalized single comparison of target class with class at addr in cN's constant pool.i*/
/* keeps cN unchanged if target is no super class of cN.*/
/* else cN is the super class of former cN which has target as super class.*/
void subCheck(u2 target, u2 addr)
{
    u2 super_class =
    cs[cN].constant_pool[getU2(cs[cN].constant_pool[addr] + 1)];
    methodStackPush(cN);
    FIND_CLASS(getAddr(super_class + 3), getU2(super_class + 1));
    if (!checkInstance(target))
    {
        cN = methodStackPop();
    }
    else
    {
        methodStackPop();
    }
}


/* receives object's class via cN and target class as parameter*/
/* returns true / false*/
u1 checkInstance(const u2 target)
{
    if (cN != 0 && cN != target)
    {
        /* trying the super class.*/
        if (getU2(cs[cN].super_class) > 0)
        {
            subCheck(target, getU2(cs[cN].super_class));
        }
        /* trying the interfaces.*/
        if (cN != 0 && cN != target)
        {
            u2 n = getU2(cs[cN].interfaces_count);
            while (--n && cN != target)
            {
                subCheck(target, getU2(cs[cN].interfaces + n * 2));
            }
        }
    }
    return (target == cN);
}


slot createDims(u4 dimsLeft, s2 *dimSize)
{
    slot act_array = NULLOBJECT;
    if (dimsLeft == 0)
    {
        return act_array;
    }
    if (*dimSize == 0)
    {
        *dimSize = (s2) opStackPop().Int;
    }
    if (*dimSize < 0)
    {
        NEGATIVEARRAYSIZEEXCEPTION;
    }
    else if (*dimSize > (MAXHEAPOBJECTLENGTH - 1))
    {
        ARRAYINDEXOUTOFBOUNDSEXCEPTION;
    }
    else
    {
        /* + marker*/
        u2 heapPos = getFreeHeapSpace(*dimSize + 1);
        act_array.stackObj.pos = heapPos;
        act_array.stackObj.magic = OBJECTMAGIC;
        act_array.stackObj.arrayLength = *dimSize;
        HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDARRAY;
        HEAPOBJECTMARKER(heapPos++).magic = OBJECTMAGIC;
        s2 *cnt = (s2 *) malloc(sizeof(s2));
        *cnt = 0;
        int i;
        for (i = 0; i < *dimSize; ++i)
        {
            heapSetElement(createDims(dimsLeft - 1, cnt), heapPos++);
        }
        free(cnt);
    }
    return act_array;
}


#ifndef TINYBAJOS_EXCEPTION
/*
 ** Realizes an interpreter-raised Exception
 */
//BH AM not tested
void raiseExceptionFromIdentifier(const char *identifier, const u1 length)
{

    methodStackPush(cN);
    methodStackPush(mN);

#ifdef DEBUG
    if (strlen(identifier) != length)
    {
        PRINTF("ERROR: Wrong length for %s\n", identifier);
    }
#endif

    /* Create a class of the given type*/
    if (findClass(identifier, length) == 0)
    {
        CLASSNOTFOUNDERR(identifier, length);
    }

    /* + marker*/
    u2 heapPos = getFreeHeapSpace(getU2(cs[cN].fields_count) + 1);
    first.stackObj.pos = heapPos;
    first.stackObj.magic = OBJECTMAGIC;
    first.stackObj.classNumber = cN;
    opStackPush(first);                           /* reference to stackObject on opStack*/

    HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDNEWOBJECT;
    HEAPOBJECTMARKER(heapPos).magic = OBJECTMAGIC;
    HEAPOBJECTMARKER(heapPos).mutex = MUTEXNOTBLOCKED;
    j = getU2(cs[cN].fields_count);
    for (i = 0; i < j; i++)
    {
        heapSetElement(toSlot((u4) 0), heapPos + i + 1);
    }

    /*	if (!findMethodByName("<init>", 6, "()V", 3)) {
     METHODNOTFOUNDERR("<init>", identifier);
     }

     #ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
     printf_P(PSTR("running <init> of %s\n"), identifier);
     #else
     printf("running <init> of %s\n", identifier);
     #endif

     methodStackPush(pc);
     methodStackPush(local);
     run();
     methodStackPop(local);
     methodStackPop(pc);
     */

    mN = methodStackPop();
    cN = methodStackPop();

    handleException();
}


//BH AM not tested
void handleException()
{
    /* this is actually the class we have to catch*/
    u1 classNumberFromPushedObject = opStackPeek().stackObj.classNumber;

    /* number of catches the try block has*/
    u2 n = getU2(METHODCODEEXCEPTIONBASE(cN, mN));

    DEBUGPRINTLN("trying to catch class number %d", classNumberFromPushedObject);
    DEBUGPRINTLN("%d catch clauses", n);
    for (i = 0; i < n; ++i)
    {
        u2 cur_catch = METHODCODEEXCEPTIONBASE(cN, mN) + 8 * i;

        /* checking if catch range is usable */
        if (pc - getStartPC() - 1 < getU2(cur_catch + 2) || pc - getStartPC()
            - 1 >= getU2(cur_catch + 4))
        {
            DEBUGPRINTLN("pc: %d", pc - getStartPC() - 1);
            DEBUGPRINTLN("start: %d", getU2(cur_catch + 2));
            DEBUGPRINTLN("end: %d", getU2(cur_catch + 4));
            DEBUGPRINTLN("not my range");
            continue;
        }

        /* checking whether the catch's catched class is in the code exception table*/
        methodStackPush(cN);
        if (FIND_CLASS/* className*/(getAddr(CP(cN, getU2(CP(cN, getU2(cur_catch + 8)) + 1)) + 3),
                                     /* classNameLength*/getU2(CP(cN, getU2(CP(cN, getU2(cur_catch + 8)) + 1)) + 1))
            == 0)
        {
            DEBUGPRINTLN("Exception class not found:  %d\n", cN);
            cN = methodStackPop();
            continue;                             /* class is not in the class table - broken code.*/
        }

        /* Ya well, this is the catched class's number in code exception table*/
        u1 classNumberInCodeExceptionTable = cN;
        DEBUGPRINTLN("classNumberInCodeExceptionTable: %d",
                     classNumberInCodeExceptionTable);
        
        cN = classNumberFromPushedObject;
        
        /* start catching */
        if (checkInstance(classNumberInCodeExceptionTable))
        {
            DEBUGPRINTLN("catching!");
            cN = methodStackPop();
            pc = getStartPC() + getU2(cur_catch + 6);
            return;
        }
        cN = methodStackPop();
    }
    
    /* keine catch clause gefunden, also weiter im nächsten stack frame*/
    if (methodStackEmpty())
    {
        DEBUGPRINTLN("we are thru, this was the top frame");
        cN = classNumberFromPushedObject;
        UNHANDLEDEXCEPTIONERR((char *) getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)] + 1)] + 3));
    }
    else
    {
        DEBUGPRINTLN("popping stack frame");
        first = opStackPop();
        opStackSetSpPos(methodStackPop());
        pc = methodStackPop() + 2;
        mN = methodStackPop();
        cN = methodStackPop();
        local = methodStackPop();
        opStackPush(first);
        handleException();
    }
}
#endif// TINYBAJOS_EXCEPTION
