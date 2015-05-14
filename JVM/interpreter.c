// For educational purposes, ...
// 08.14.06 it was first shown by (in Unix)
// April 2007-> threads scheduling, funtion süberladung, native-dispatch
// Mai 2007 -> garbage collection, exception handling
// synchronized, wait notify
#include <stdlib.h> //malloc & free
#include "nstring.h"

#include "classfile.h"
#include "kjvm.h"
#include "stack.h"
#include "heap.h"
#include "interpreter.h"
#include "nativedispach.h"

#define GETSTARTPC(offset)  ((strncmp(  "Code",\
getAddr(cN, cs[cN].constant_pool[getU2(METHODBASE(cN,mN) + 8 + offset)] + 3),4) == 0)\
? (u2)METHODBASE(cN,mN) + 8 + 14 + offset \
: GETSTARTPC(offset + getU4(METHODBASE(cN,mN) + 8) + 6))


#ifdef USE_MICROKERNEL
#include "microkernel.h"
#endif

//static slot first;
//static slot second;
//static slot third;
//static slot fourth;


#ifdef ENABLE_KCLASS_FORMAT
extern char* getClassName(const u2 classId);
#endif

extern u2 pc;
extern u1 cN;
extern u1 fN;
extern u1 mN;
extern u1 local;

extern classStructure cs[MAXCLASSES];

extern ThreadControlBlock* currentThreadCB;
extern ThreadPriorityList threadList;

void interpreter_run(const u1 classId,const u1 methodId) // in: classNumber,  methodNumber cN, mN
{   //u1 code, byte1, byte2;
    //u2 heapPos;
    cN = classId;
    mN = methodId;
    opStackSetSpPos(findMaxLocals(cN,mN));

    pc = getStartPC(cN,mN);

    while(1)
    {
              u1 code  = getU1(cN,0);
        const u1 byte1 = getU1(cN,pc);
        const u1 byte2 = getU1(cN,pc + 1);
#ifdef DEBUG_VM
        DEBUGPRINT_OPC("-> ");

#ifndef TINYBAJOS_MULTITASKING
        DEBUGPRINTE(currentThreadCB->tid, x);
#endif
        DEBUGPRINT_OPC(", pc: %x ", pc - getStartPC(cN,mN) - 1);
        DEBUGPRINTE(code, 2x);
        DEBUGPRINTE(byte1,2x);
        DEBUGPRINTE(byte2,2x\t);
        DEBUGPRINT_OPC("sp: %d local: %d\n", opStackGetSpPos(), local);
        DEBUGPRINTSTACK;
        DEBUGPRINTLOCALS;
        DEBUGPRINT_OPC("\t\t\t\t");
#endif

        DISPATCH(code)
        {
            CASE(NOP):
            {
                DEBUGPRINTLN_OPC("NOP");
            }BREAK;
            CASE(ACONST_NULL):
            {
                DEBUGPRINTLN_OPC("aconst_null -> push\t...,=> NULLOBJECT");
                opStackPush(NULLOBJECT);
            }BREAK;
            CASE(ICONST_M1)://???
            CASE(ICONST_0):
            CASE(ICONST_1):
            CASE(ICONST_2):
            CASE(ICONST_3):
            CASE(ICONST_4):
            CASE(ICONST_5):
            {
                DEBUGPRINTLN_OPC("ICONST_%4x -> push\t...,=> %4x",code-ICONST_0,code - ICONST_0);
                opStackPush(toSlot((s4)(code - ICONST_0)));
            }BREAK;
            CASE(FCONST_0):
            CASE(FCONST_1):
            CASE(FCONST_2):
            {
                //u1 base = ICONST_0;
                DEBUGPRINTLN_OPC("FCONST_%d  -> push\t...,=> %f",code - FCONST_0,(f4)(code - FCONST_0));
                opStackPush(toSlot((f4)(code - FCONST_0)));
            }BREAK;
            CASE(BIPUSH):
            {
                DEBUGPRINTLN_OPC("BIPUSH  -> push\t...,=> %d",(s1)byte1);
                // BIPUSH is defined as follows:
                // reads a signed 8 bit constant from byte1,
                // extends it to int signed (32bit)
                // and saves it on stack
                opStackPush(toSlot(((getU1(cN,pc) & 0x80) >> 7) * 0xffffff80 | (getU1(cN,0) & 0x7f)));
            }BREAK;
            CASE(SIPUSH):
            {
                DEBUGPRINTLN_OPC("SIPUSH  -> push\t...,=> %x",(s2)BYTECODEREF);
                opStackPush(toSlot((s4)((s2) getU2(cN,0))));
            }BREAK;
            CASE(LDC):
            {
                DEBUGPRINT_OPC("ldc  push\t...");
                const u1 type = GET_TAG(cN, getU1(cN,0));
                if (type == CONSTANT_String)
                {
                    slot first;
                    first.stackObj.magic = CPSTRINGMAGIC;
                    first.stackObj.classNumber = cN;
                    first.stackObj.pos = (u2)(byte1);//((u2)cN <<8));
                    opStackPush(first);
                } else // int or float const value on stack
                    opStackPush(toSlot( getU4(cN,CP(cN, byte1) + 1) ));
                DEBUGPRINTLN_OPC(",=> x%x", opStackPeek().UInt);
            }BREAK;
            CASE(LDC_W):
            {
                ERROREXIT(-4, "LDC_W not yet realized\n");
            }BREAK;
            CASE(ILOAD):
            CASE(FLOAD):
            CASE(ALOAD):
            {
#ifdef DEBUG
                if(code == ILOAD) DEBUGPRINTLN_OPC("ILOAD -> local(%x) -> push\t...,=>",byte1);
                if(code == FLOAD) DEBUGPRINTLN_OPC("FLOAD -> local(%d: %d) -> push\t...,=>",byte1,getU1(cN,byte1));
                if(code == ALOAD) DEBUGPRINTLN_OPC("aload -> local(%x) -> push\t,=>",byte1);
#endif
                opStackPush(opStackGetValue(local + getU1(cN,0)));
            }BREAK;
            CASE(ILOAD_0):
            CASE(ILOAD_1):
            CASE(ILOAD_2):
            CASE(ILOAD_3):
            {
                DEBUGPRINTLN_OPC("ILOAD_%d local -> push\t...,=>",code - ILOAD_0);
                opStackPush(opStackGetValue(local + code - ILOAD_0));
            }BREAK;
            CASE(FLOAD_0):
            CASE(FLOAD_1):
            CASE(FLOAD_2):
            CASE(FLOAD_3):
            {
                 DEBUGPRINTLN_OPC("FLOAD_%d local -> push\t...,=>",code - FLOAD_0);
                 opStackPush(opStackGetValue(local + code - FLOAD_0));
            }BREAK;
            CASE(ALOAD_0):
            CASE(ALOAD_1):
            CASE(ALOAD_2):
            CASE(ALOAD_3):
            {
                 DEBUGPRINTLN_OPC("aload_%d local -> push\t...,=>",code - ALOAD_0);
                 opStackPush(opStackGetValue(local + code - ALOAD_0));
            }BREAK;
            CASE(IALOAD):
            CASE(FALOAD):
            CASE(AALOAD):
            CASE(BALOAD):
            CASE(CALOAD):
            CASE(SALOAD):
            {
                const s2 count = (s2)opStackPop().Int;
                const slot first = opStackPop();
#ifdef DEBUG
                if(code == IALOAD) DEBUGPRINTLN_OPC("iaload");
                if(code == FALOAD) DEBUGPRINTLN_OPC("faload");
                if(code == AALOAD) DEBUGPRINTLN_OPC("aaload");
                if(code == BALOAD) DEBUGPRINTLN_OPC("baload");
                if(code == CALOAD) DEBUGPRINTLN_OPC("caload");
                if(code == SALOAD) DEBUGPRINTLN_OPC("saload");
                DEBUGPRINT_OPC("%x, =>",first.UInt);
#endif
                const u4 lengthArray = first.stackObj.arrayLength;
                if (first.UInt == NULLOBJECT.UInt)
                {
                    NULLPOINTEREXCEPTION;
                } else if (count < 0 || lengthArray <= count || count > MAX_ARRAY_SIZE)
                {
                    ARRAYINDEXOUTOFBOUNDSEXCEPTION;
                }
                else
                {
                    opStackPush(heapGetElement((u2) first.stackObj.pos + count + 1));
                    DEBUGPRINTLN_OPC(", %x", opStackPeek().UInt);
                }
            }BREAK;
            CASE(ISTORE):
            CASE(FSTORE):
            CASE(ASTORE):
            {
#ifdef DEBUG
                if(code == ISTORE) DEBUGPRINTLN_OPC("ISTORE  pop -> local(%d)=>,\n",byte1);
                if(code == FSTORE) DEBUGPRINTLN_OPC("FSTORE  pop -> local(%d)=>,\n",byte1);
                if(code == ASTORE) DEBUGPRINTLN_OPC("ASTORE  pop -> local(%x)=>,\n",byte1);
#endif
                opStackSetValue(local + getU1(cN,0),opStackPop());

            }BREAK;
            CASE(ISTORE_0):
            CASE(ISTORE_1):
            CASE(ISTORE_2):
            CASE(ISTORE_3):
            {
                DEBUGPRINTLN_OPC("ISTORE_%d pop -> local   %d=>,",code-ISTORE_0,opStackPeek().Int);
                opStackSetValue(local + code - ISTORE_0, opStackPop());
            }BREAK;
            CASE(FSTORE_0):
            CASE(FSTORE_1):
            CASE(FSTORE_2):
            CASE(FSTORE_3):
            {
                DEBUGPRINTLN_OPC("FSTORE_%d pop -> local   =>,",code - FSTORE_0);
                opStackSetValue(local + code - FSTORE_0, opStackPop());
            }BREAK;
            CASE(ASTORE_0):
            CASE(ASTORE_1):
            CASE(ASTORE_2):
            CASE(ASTORE_3):
            {
                DEBUGPRINTLN_OPC("ASTORE_%d pop -> local  =>,",code-ASTORE_0);
                opStackSetValue(local + code - ASTORE_0, opStackPop());
            }BREAK;
            CASE(IASTORE):
            CASE(FASTORE):
            CASE(AASTORE):
            CASE(BASTORE):
            CASE(CASTORE):
            CASE(SASTORE):
            {
#ifdef DEBUG
                if(code == IASTORE) DEBUGPRINTLN_OPC("iastore stack -> local");
                //float
                if(code == FASTORE) DEBUGPRINTLN_OPC("fastore");
                //float
                if(code == AASTORE) DEBUGPRINTLN_OPC("fastore");
                //byte or boolean
                if(code == BASTORE) DEBUGPRINTLN_OPC("bastore");
                //char
                if(code == CASTORE) DEBUGPRINTLN_OPC("castore");
                //short
                if(code == SASTORE) DEBUGPRINTLN_OPC("sastore");
#endif
                const slot second = opStackPop();
                s2 count = (s2)(opStackPop().Int);
                const slot first = opStackPop();
                const u4 lengthArray = first.stackObj.arrayLength;
                if (first.UInt == NULLOBJECT.UInt)
                {
                    NULLPOINTEREXCEPTION;
                } else if (count < 0 || lengthArray <= count || count > MAX_ARRAY_SIZE)
                {
                    ARRAYINDEXOUTOFBOUNDSEXCEPTION;
                }
                else
                {
                    heapSetElement(second, first.stackObj.pos + count + 1);
                }
            }BREAK;
            CASE(POP2):
            {
                DEBUGPRINTLN_OPC("POP2");
                opStackPop();
            }
            CASE(POP):
            {
                DEBUGPRINTLN_OPC("POP %x",opStackPeek().UInt);
                opStackPop();
            }BREAK;

            CASE(DUP):
            {
                DEBUGPRINTLN_OPC("dup");
                opStackPush(opStackPeek());
            }BREAK;
            CASE(DUP_X1):
            {
                DEBUGPRINTLN_OPC("DUP_X1");
                const slot first = opStackPop();
                const slot second = opStackPop();
                opStackPush(first);
                opStackPush(second);
                opStackPush(first);
            }BREAK;
            CASE(DUP_X2):
            {
                DEBUGPRINTLN_OPC("DUP_X2");
                const slot first = opStackPop();
                const slot second = opStackPop();
                const slot third = opStackPop();
                opStackPush(first);
                opStackPush(third);
                opStackPush(second);
                opStackPush(first);
            }BREAK;
            CASE(DUP2):
            {
                DEBUGPRINTLN_OPC("DUP2");
                const slot first = opStackPop();
                const slot second = opStackPop();
                opStackPush(second);
                opStackPush(first);
                opStackPush(second);
                opStackPush(first);
            }BREAK;
            CASE(DUP2_X1):
            {
                DEBUGPRINTLN_OPC("DUP2_X1");
                const slot first = opStackPop();
                const slot second = opStackPop();
                const slot third = opStackPop();
                opStackPush(second);
                opStackPush(first);
                opStackPush(third);
                opStackPush(second);
                opStackPush(first);
            }BREAK;
            CASE(DUP2_X2):
            {
                DEBUGPRINTLN_OPC("DUP2_X2");
                const slot first = opStackPop();
                const slot second = opStackPop();
                const slot third = opStackPop();
                const slot fourth = opStackPop();
                opStackPush(second);
                opStackPush(first);
                opStackPush(fourth);
                opStackPush(third);
                opStackPush(second);
                opStackPush(first);
            }BREAK;
            CASE(SWAP):
            {
                DEBUGPRINTLN_OPC("SWAP");
                const slot first = opStackPop();
                const slot second = opStackPop();
                opStackPush(first);
                opStackPush(second);
            }BREAK;
            CASE(IADD):
            {
                DEBUGPRINTLN_OPC("IADD");
                opStackPoke(toSlot((opStackPop().Int + opStackPeek().Int)));
            }BREAK;
            CASE(FADD):
            {
                DEBUGPRINTLN_OPC("FADD");
                opStackPoke(toSlot((opStackPop().Float + opStackPeek().Float)));
            }BREAK;
            CASE(ISUB):
            {
                DEBUGPRINTLN_OPC("ISUB");
                const slot first = opStackPop(); // mb fj changed substraction order
                opStackPoke(toSlot((opStackPeek().Int - first.Int)));
            }BREAK;
            CASE(FSUB):
            {
                DEBUGPRINTLN_OPC("Fsub");
                const slot first = opStackPop(); // mb fj changed substraction order
                opStackPoke(toSlot((opStackPeek().Float - first.Float)));
            }BREAK;
            CASE(IMUL):
            {
                DEBUGPRINTLN_OPC("IMUL");
                opStackPoke(toSlot((opStackPop().Int * opStackPeek().Int)));
            }BREAK;
            CASE(FMUL):
            {
                DEBUGPRINTLN_OPC("FMUL");
                opStackPoke(toSlot((opStackPop().Float * opStackPeek().Float)));
            }BREAK;
            CASE(IDIV):
            {
                DEBUGPRINTLN_OPC("IDIV");
                const slot first = opStackPop(); // mb fj changed dividend order
                if (first.Int == 0)
                    ARITHMETICEXCEPTION;
                else
                    opStackPush(toSlot((opStackPop().Int / first.Int)));
            }BREAK;
            CASE(FDIV):
            {
                DEBUGPRINTLN_OPC("FDIV");
                const slot first = opStackPop(); //mb fj changed dividend order
                if (first.Float == 0.0)
                    ARITHMETICEXCEPTION;
                else
                    opStackPoke(toSlot((opStackPeek().Float / first.Float)));
            }BREAK;
            CASE(IREM):
            {
                DEBUGPRINTLN_OPC("IREM");
                const slot first = opStackPop();
                if (first.Int == 0)
                    ARITHMETICEXCEPTION;
                else
                    opStackPoke(toSlot((opStackPeek().Int % first.Int)));
            }BREAK;
            CASE(FREM):
            {
                DEBUGPRINTLN_OPC("FREM");
                float divisor = opStackPop().Float;
                float dividend = opStackPop().Float;
                int q = dividend / divisor;
                opStackPush(toSlot((f4)(dividend - (divisor * q))));
            }BREAK;
            CASE(INEG):
            {
                DEBUGPRINTLN_OPC("INEG");
                opStackPoke(toSlot(-opStackPeek().Int));
            }BREAK;
            CASE(FNEG):
            {
                DEBUGPRINTLN_OPC("FNEG");
                opStackPoke(toSlot(-opStackPeek().Float));
            }BREAK;
            CASE(ISHL):
            {
                DEBUGPRINTLN_OPC("ISHL");
                const slot shift = opStackPop();
                const slot val = opStackPop();
                opStackPush(toSlot(val.UInt << shift.UInt));
            }BREAK;
            CASE(ISHR):
            {
                DEBUGPRINTLN_OPC("ISHR");
                const slot shift = opStackPop();
                const slot val = opStackPop();
                opStackPush(toSlot(val.Int >> shift.Int));
            }BREAK;
            CASE(IUSHR):
            {
                DEBUGPRINTLN_OPC("IUSHR");
                const slot shift = toSlot(opStackPop().Int & 0x0000001f);
                const slot val = opStackPop();
                if (val.Int < 0)
                    opStackPush(toSlot((val.Int >> shift.Int) + (2 << ~shift.Int)));
                else
                    opStackPush(toSlot(val.Int >> shift.Int));
            }BREAK;
            CASE(IAND):
            {
                DEBUGPRINTLN_OPC("IAND");
                opStackPoke(toSlot(opStackPop().UInt & opStackPeek().UInt));
            }BREAK;
            CASE(IOR):
            {
                DEBUGPRINTLN_OPC("IOR");
                opStackPoke(toSlot(opStackPop().UInt | opStackPeek().UInt));
            }BREAK;
            CASE(IXOR):
            {
                DEBUGPRINTLN_OPC("IXOR");
                opStackPoke(toSlot(opStackPop().UInt ^ opStackPeek().UInt));
            }BREAK;
            CASE(IINC):
            {
                DEBUGPRINTLN_OPC("IINC");
                // position
                opStackSetValue((u2)(local + byte1),
                                // old value
                                toSlot((s4)(opStackGetValue(local + byte1).Int + (s4) (s1) byte2)));// add const
                pc += 2;        // to skip the index + const

            }BREAK;
            CASE(I2F):
            {
                DEBUGPRINTLN_OPC("I2F");
                opStackPoke(toSlot((f4) opStackPeek().Int));
            }BREAK;
            CASE(F2I):
            {
                DEBUGPRINTLN_OPC("F2I");
                opStackPoke(toSlot((s4)(opStackPeek().Float)));
            }BREAK;
            CASE(I2C):
            {
                DEBUGPRINTLN_OPC("I2C");
                opStackPoke(toSlot((opStackPeek().UInt & 0x0000ffff)));
            }BREAK;
            CASE(I2B):
            {
                DEBUGPRINTLN_OPC("I2B");
                opStackPoke(toSlot(opStackPeek().UInt & 0x000000ff));
            }BREAK;
            CASE(I2S):
            {
                DEBUGPRINTLN_OPC("I2S");
                opStackPoke(toSlot((s4)((s2) opStackPeek().Int)));
            }BREAK;
            CASE(FCMPL):
            CASE(FCMPG):
            {
                DEBUGPRINTLN_OPC("fcmpg");
                const slot second = opStackPop();
                const slot first = opStackPop();
                if (first.Float == 0x7fc00000 || second.Float == 0x7fc00000)
                {
                    opStackPush(toSlot((s4)(code == FCMPG ? 1 : -1)));
                }
                else if (first.Float > second.Float)
                {
                    opStackPush(toSlot((s4) 1));
                }// corrected by al june 08
                else if (first.Float == second.Float)
                {
                    opStackPush(toSlot((s4) 0));
                }
                else
                {
                    opStackPush(toSlot((s4) - 1));
                }
            }BREAK;
            CASE(IFEQ):
            {
                DEBUGPRINTLN_OPC("ifeq");
                if (opStackPop().Int == 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2; // to skip the jump-adress
            }BREAK;
            CASE(IFNULL):
            {
                DEBUGPRINTLN_OPC("ifnull");
                if (opStackPop().UInt == NULLOBJECT.UInt)
                    pc += BYTECODEREF - 1; // add offset to pc at ifnull-address
                else
                    pc += 2;               // skip branch bytes
            }BREAK;
            CASE(IFNONNULL):
            {
                DEBUGPRINTLN_OPC("ifnonnull");
                if (opStackPop().UInt != NULLOBJECT.UInt)
                    pc += BYTECODEREF - 1;  // add offset to pc at ifnull-address
                else
                    pc += 2;                // skip branch bytes
            }BREAK;
            CASE(IFNE):
            {
                DEBUGPRINTLN_OPC("ifne");
                if (opStackPop().Int != 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                 // to skip the jump-adress
            }BREAK;
            CASE(IFLT):
            {
                DEBUGPRINTLN_OPC("iflt");
                if (opStackPop().Int < 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                  // to skip the jump-adress
            }BREAK;
            CASE(IFLE):
            {
                DEBUGPRINTLN_OPC("ifle");
                if (opStackPop().Int <= 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                   // to skip the jump-adress
            }BREAK;
            CASE(IFGT):
            {
                DEBUGPRINTLN_OPC("ifgt");
                if (opStackPop().Int > 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                    // to skip the jump-adress
            }BREAK;
            CASE(IFGE):
            {
                DEBUGPRINTLN_OPC("ifge");
                if (opStackPop().Int >= 0)
                    pc += (s2)((byte1 << 8) | (byte2)) - 1;
                else
                    pc += 2;                     // to skip the jump-adress
            }BREAK;
            CASE(IF_ACMPEQ): DEBUGPRINTLN_OPC("if_acmpeq");
            CASE(IF_ICMPEQ):
            {
                DEBUGPRINTLN_OPC("if_icmpeq");
                if (opStackPop().Int == opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      // to skip the jump-adress
            }BREAK;
                CASE(IF_ACMPNE): DEBUGPRINTLN_OPC("if_acmpne");
            CASE(IF_ICMPNE):
            {
                DEBUGPRINTLN_OPC("if_icmpne");
                if (opStackPop().Int != opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      // to skip the jump-adress
            }BREAK;
            CASE(IF_ICMPLT):
            {
                DEBUGPRINTLN_OPC("if_icmplt");
                if (opStackPop().Int > opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      // to skip the jump-adress
            }BREAK;
            CASE(IF_ICMPGE):
            {
                DEBUGPRINTLN_OPC("if_icmpge");
                if (opStackPop().Int <= opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      // to skip the jump-adress
            }BREAK;
            CASE(IF_ICMPGT):
            {
                DEBUGPRINTLN_OPC("if_icmpgt");
                if (opStackPop().Int < opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      // to skip the jump-adress
            }BREAK;
            CASE(IF_ICMPLE):
            {
                DEBUGPRINTLN_OPC("if_icmple");
                if (opStackPop().Int >= opStackPop().Int)
                    pc += (s2)((u2)((byte1 << 8) | (byte2))) - 1;
                else
                    pc += 2;                      // to skip the jump-adress
            }BREAK;
            CASE(GOTO):
            {
                DEBUGPRINTLN_OPC("goto");
                pc += (s2) BYTECODEREF - 1;
            }BREAK;
            CASE(JSR):
            {
                DEBUGPRINTLN_OPC("jsr");
                opStackPush(toSlot((u4)(pc + 2)));
                pc += (s2) BYTECODEREF - 1;
            }BREAK;
            CASE(RET):
            {
                DEBUGPRINTLN_OPC("ret");
                pc = opStackGetValue(local + getU1(cN,0)).UInt;
            }BREAK;
            CASE(TABLESWITCH):
            CASE(LOOKUPSWITCH):
            {
                DEBUGPRINTLN_OPC(code == LOOKUPSWITCH ? "lookupswitch" : "tableswitch");
                {
                    // aa           tableswitch		ab          lookupswitch
                    // 0  0  0      padding			0  0        padding
                    // 0 0 0 2		DefaultByte	0  0  0 25  defaultByte
                    // 0 0 0 1		LowByte		0  0  0  2  npairs
                    // 0 0 0 3		HighByte	0  0  0  1  match
                    // 0  0  0 1c	Offset für 1		0  0  0 1b  offset
                    // 0  0  0 22	Offset für 2		0  0  0  2  match
                    // 0  0  0 28	Offset für 3		0  0  0 20  offset
                    u2 startPc = --pc;
                    u2 relPc = pc - getStartPC(cN,mN); // pcMethodStart;	//calculate relative PC for ByteCode in Method
                    u4 key = opStackPop().Int;

                    // next pc as multiple of 4 from address of 0xab (lookupswitch)/skip padding bytes(tableswitch)
                    relPc = (u2)((relPc + 4) & 0xfffc);
                    pc = relPc + getStartPC(cN,mN);     // pcMethodStart;	// set pc to begin of default address
                    u4 offset = getU4(cN,0);            // default offset

                    u4 lowbyte = getU4(cN,0);
                    if(code == TABLESWITCH){
                        u4 highbyte = getU4(cN,0);
                        if (lowbyte <= key && key <= highbyte)
                        {
                            u4 tableoffset = key - lowbyte;
                            pc += tableoffset * 4;       // skip 4 byte of previous address(es)
                            offset = getU4(cN,0);
                        }
                    }else{
                        for (; lowbyte > 0; --lowbyte)
                        {
                            u4 match = getU4(cN,0);
                            u4 tmpOffset = getU4(cN,0);
                            if (key == match)
                            {
                                offset = tmpOffset;
                                break;
                            }
                        }
                    }
                    pc = startPc + offset;
                }
            }BREAK;
            CASE(PUTSTATIC):
            CASE(GETSTATIC):
            {
                DEBUGPRINTLN_OPC(code == GETSTATIC ? "getstatic " : "putstatic");       //mb jf ... corrected funtion
                methodStackPush(cN);

                const u2 nameAndTypeId = FIELDINFO_GET_NAME_AND_TYPEID(cN,BYTECODEREF);

                const u2 nameId = NAMEANDTYPE_GET_NAMEID(cN,nameAndTypeId);

                const char* fieldName = UTF8_GET_STRING(cN,nameId);
                const u2 fieldNameLength = UTF8_GET_LENGTH(cN,nameId);

                const u2 fieldDescriptionId = NAMEANDTYPE_GET_DESCRIPTIONID(cN,nameAndTypeId);

                const char* fieldDescr = UTF8_GET_STRING(cN,fieldDescriptionId);
                const u2 fieldDescrLength = UTF8_GET_LENGTH(cN,fieldDescriptionId);


                const u2 classInfo = FIELDINFO_GET_CLASSINFOID(cN,BYTECODEREF);
                const u2 classNameId = CLASSINFO_GET_NAMEID(cN,classInfo);
#ifdef ENABLE_KCLASS_FORMAT
                cN = getClassIndex(classNameId);
#ifdef DEBUG_KCLASS
                const char* className = getClassName(classNameId);
                const u2 classNameLength = stringLength(className);
#endif
#else
                const char* className = UTF8_GET_STRING(cN,classNameId);
                const u2 classNameLength = UTF8_GET_LENGTH(cN,classNameId);

                cN = findClass(className,classNameLength);
                if (cN == INVALID_CLASS_ID)
                {
                    CLASSNOTFOUNDERR(className,classNameLength);
                }
#endif
                refInfo ref = findFieldByName(cN,cN,fieldName, fieldNameLength, fieldDescr,fieldDescrLength,1);
                if (ref.reference == INVALID_FIELD_ID)
                {
                    FIELDNOTFOUNDERR(fieldName,className);
                }

                if(code == GETSTATIC){
                    opStackPush(heapGetElement(cs[ref.classId].classInfo.stackObj.pos + ref.reference + 1));
                }else{
                    heapSetElement(opStackPop(),cs[ref.classId].classInfo.stackObj.pos + ref.reference + 1);
                }
                pc += 2;
                cN = methodStackPop();	// end GETSTATIC/PUTSTATIC
                
            }BREAK;
            CASE(GETFIELD):
            CASE(PUTFIELD):
            {
                DEBUGPRINTLN_OPC("put/get field");
                methodStackPush(cN);
                slot first = opStackPop();

                slot second;
               	if(code == PUTFIELD){
                    second = opStackPop();
                }

                const u2 nameAndTypeId = FIELDINFO_GET_NAME_AND_TYPEID(cN,BYTECODEREF);
                const u2 nameId = NAMEANDTYPE_GET_NAMEID(cN,nameAndTypeId);

                const char* fieldName = UTF8_GET_STRING(cN,nameId);
                const u2 fieldNameLength = UTF8_GET_LENGTH(cN,nameId);

                const u2 fieldDescriptionId = NAMEANDTYPE_GET_DESCRIPTIONID(cN,nameAndTypeId);

                const char* fieldDescr = UTF8_GET_STRING(cN,fieldDescriptionId);
                const u2 fieldDescrLength = UTF8_GET_LENGTH(cN,fieldDescriptionId);


                const u2 classInfo = FIELDINFO_GET_CLASSINFOID(cN,BYTECODEREF);
                const u2 classNameId = CLASSINFO_GET_NAMEID(cN,classInfo);

#ifdef ENABLE_KCLASS_FORMAT
                cN = getClassIndex(classNameId);
#ifdef DEBUG_KCLASS
                const char* className = getClassName(classNameId);
                const u2 classNameLength = stringLength(className);
#endif
#else
                const char* className = UTF8_GET_STRING(cN,classNameId);
                const u2 classNameLength = UTF8_GET_LENGTH(cN,classNameId);

                cN = findClass(className,classNameLength);
                if (cN == INVALID_CLASS_ID)
                {
                    CLASSNOTFOUNDERR(className,classNameLength);
                }
#endif
                if (code == PUTFIELD && second.UInt == NULLOBJECT.UInt)
                {
                    pc += 2;
                    cN = methodStackPop();
                    NULLPOINTEREXCEPTION;
                }
                else
                {
                    const u2 fieldClassId = cN;
                    const u2 instanceClassId = ( code == PUTFIELD ? second.stackObj.classNumber : first.stackObj.classNumber);

                    //printf("PUT/GET Field %s defined in :%s, instance class:%s \n",fieldName,className,(char*)getClassName(instanceClassId));
                    refInfo ref = findFieldByName(instanceClassId,fieldClassId,fieldName, fieldNameLength, fieldDescr, fieldDescrLength,0);
                    if (ref.reference == INVALID_FIELD_ID)
                    {
                        FIELDNOTFOUNDERR(fieldName,className);
                    }

                    if (code == PUTFIELD)
                    {
                        if(STRNCMP( "B",fieldDescr, 1) == 0)
                        {//Truncate Integer input for Byte output
                            first.Int = first.Int & 0x000000ff;
                        }
                        heapSetElement(first, second.stackObj.pos + ref.reference + 1);
                    }else
                        opStackPush(toSlot(heapGetElement(first.stackObj.pos + ref.reference +  1).Int));
                }
                pc += 2;
                cN = methodStackPop();
                // end PUT/GETFIELD
            }BREAK;
            CASE(INVOKESPECIAL):
            CASE(INVOKEVIRTUAL):
            CASE(INVOKEINTERFACE):
            {
                #ifdef DEBUG
                if (code == INVOKEVIRTUAL) printf("invokevirtual: ");//DEBUGPRINT_OPC("invokevirtual: ");
                if (code == INVOKEINTERFACE) printf("invokeinterface: ");//DEBUGPRINT_OPC("invokeinterface: ");
                if (code == INVOKESPECIAL) printf("invoke special: ");//DEBUGPRINT_OPC("invoke special: ");
                #endif
                methodStackPush(local);
                methodStackPush(cN);
                methodStackPush(mN);
                if (code == INVOKEINTERFACE)
                    pc += 2;
                methodStackPush(pc);
                const int k = findNumArgs(cN,BYTECODEREF);
                // (BYTECODEREF)-1));
                methodStackPush((opStackGetSpPos() - k - 1));
                // method resolution
                // nachdenken ->mhrmals benutzt
                local = opStackGetSpPos() - k - 1;
                // get cN from.stackObjRef
                //  get method from cN or superclasses
                //ex: INVOKEVIRTUAL #39;
                const u2 methodNameAndTypeId = METHODREF_GET_NAME_AND_TYPEID(cN,BYTECODEREF);
                const u2 methodNameId = NAMEANDTYPE_GET_NAMEID(cN,methodNameAndTypeId);

#ifndef ENABLE_KMETHOD
                const char* methodName = UTF8_GET_STRING(cN,methodNameId);
                const u2 methodNameLength = UTF8_GET_LENGTH(cN,methodNameId);
                const u2 methodDescrId = NAMEANDTYPE_GET_DESCRIPTIONID(cN,methodNameAndTypeId);

                const char* methodDescr = UTF8_GET_STRING(cN,methodDescrId);
                const u2 methodDescrLength = UTF8_GET_LENGTH(cN,methodDescrId);
#endif

                if(opStackGetValue(local).UInt == NULLOBJECT.UInt)
                {//todo - fix throw of null pointer exception
                    //cN = methodStackPop();
                    NULLPOINTEREXCEPTION;
                    BREAK;
                }
                char* className = NULL;
                u2  classNameLength = 0;

                if ((code == INVOKEVIRTUAL) || (code == INVOKEINTERFACE))
                {
                    if (opStackGetValue(local).stackObj.magic == CPSTRINGMAGIC)
                    {

#ifdef ENABLE_KCLASS_FORMAT
#ifdef USE_MICROKERNEL
                        cN = getClassIndex(JAVA_LANG_STRING_CLASS_ID());
#ifdef DEBUG_KCLASS
                        className = getClassName(JAVA_LANG_STRING_CLASS_ID());
                        classNameLength = stringLength(className);
#endif
#endif
#else
                        className = "java/lang/String";
                        classNameLength = 16;
                        cN = findClass(className, classNameLength);
#endif
                    } else{
                        cN = opStackGetValue(local).stackObj.classNumber;

#ifdef ENABLE_KMETHOD
                        const u2 classInfo = METHODREF_GET_CLASSINFOID(cN,BYTECODEREF);
                        const u2 classId = classInfo;
                        //cN = classId;
                        printf("cN:%d classId:%d ",cN,classId);
#endif
                        //printf("cN:%d nameAndType:%d",cN,methodNameAndTypeId);
                    }
                }// INVOKESPECIAL
                else
                {
                    const u2 classInfo = METHODREF_GET_CLASSINFOID(cN,BYTECODEREF);
#ifdef ENABLE_KCLASS_FORMAT
#ifdef ENABLE_KMETHOD
                    const u2 classId = classInfo;
#else
                    const u2 classId = CLASSINFO_GET_NAMEID(cN,classInfo);
#endif
                    cN = getClassIndex(classId);
#else
                    const u2 classNameId = CLASSINFO_GET_NAMEID(cN,classInfo);
                    className = UTF8_GET_STRING(cN,classNameId);
                    classNameLength = UTF8_GET_LENGTH(cN,classNameId);
                    cN = findClass(className, classNameLength);
#endif
                }
#ifdef ENABLE_KCLASS_FORMAT
#ifdef DEBUG_KCLASS
                className = getClassName(getClassID(cN));
                classNameLength = stringLength(className);
#endif
#else
                if (cN == INVALID_CLASS_ID)
                {
                    CLASSNOTFOUNDERR(className, classNameLength);
                }
#endif
                
#ifdef ENABLE_KMETHOD
                extern const char* getDebugMethodName(const u2 classId,const u2 methodId);
                const char* methodName = getDebugMethodName(getClassID(cN), methodNameId);
                printf("%s->%s:%s \n",className,methodName,methodDescr);
                mN = methodNameId;
#else
                do{
                    mN = findMethodByName(cN,methodName, methodNameLength, methodDescr, methodDescrLength);
                }while ( mN == INVALID_METHOD_ID && (cN = findSuperClass(cN)) != INVALID_CLASS_ID);

                //className = UTF8_GET_STRING(cN,getClassID(cN));
                //printf("%s->%s:%s \n",className,methodName,methodDescr);

                if (mN == INVALID_METHOD_ID)
                {
                    METHODNOTFOUNDERR(methodName, className);
                }
#endif
                //if(STRNCMP(className,"java/lang/StringBuilder",22) == 0
                //&& STRNCMP(methodName,"append",6) == 0
                //&& STRNCMP(methodDescr,"([CII)Ljava/lang/StringBuilder;",30) == 0)
                //    printf("");
                const u2 methodInfo = getU2(cN,METHODBASE(cN, mN));
                opStackSetSpPos(opStackGetSpPos() + (methodInfo & ACC_NATIVE ? 0 : findMaxLocals(cN,mN)));

#ifndef TINYBAJOS_MULTITASKING
                if ( methodInfo & ACC_SYNCHRONIZED)
                {
                    if(isThreadLocked(opStackGetValue(local),1))
                    {   // thread sleeps, try it later
                        // (BYTECODEREF));
                        opStackSetSpPos(methodStackPop() + k + 1);
                        // before invoke
                        pc = methodStackPop() - 1;
                        mN = methodStackPop();
                        cN = methodStackPop();
                        local = methodStackPop();
                        break;
                    }
                }
#endif
                // no synchronized,or I have the lock
                // now call method
                if (methodInfo & ACC_NATIVE)
                {
                    //printf("Invoke native method:%s->%s\n",className,methodName);
                    const u2 classId = getClassID(cN);
                    if(classId == 46){
                        nativeDispath(0,classId,mN,methodName, methodDescr);

                        goto nativeVoidReturn;
                    }else
                    if ( cs[cN].nativeFunction != NULL && cs[cN].nativeFunction[mN] != NULL)
                    {
                        //PRINTF("Calling native method %d\n",mN);
                        if (cs[cN].nativeFunction[mN]())
                            goto nativeValueReturn;
                        else
                            goto nativeVoidReturn;
                    //}else if(nativeDispath(methodName,methodDescr)){
                    //  goto nativeVoidReturn;
                    }else
                    {
                        ERROREXIT(-3, "native method not found cN: %d mN: %d,%s\n", cN, mN,methodName);
                    }
                }
                pc = getStartPC(cN,mN);
            }BREAK;
            CASE(INVOKESTATIC):
            {
                //printf("invoke static: ");//DEBUGPRINT_OPC("invoke static: ");// a static method
                methodStackPush(local);
                methodStackPush(cN);
                methodStackPush(mN);
                methodStackPush(pc);
                const int k = findNumArgs(cN,BYTECODEREF);
                // (BYTECODEREF));
                methodStackPush(opStackGetSpPos() - k);
                // method resolution
                local = (u2) opStackGetSpPos() - k;

                const u2 classInfo = METHODREF_GET_CLASSINFOID(cN,BYTECODEREF);

                const u2 methodNameAndTypeId = METHODREF_GET_NAME_AND_TYPEID(cN,BYTECODEREF);
                const u2 methodNameId = NAMEANDTYPE_GET_NAMEID(cN,methodNameAndTypeId);
#ifndef ENABLE_KMETHOD
                const char* methodName = UTF8_GET_STRING(cN,methodNameId);
                const u2 methodNameLength = UTF8_GET_LENGTH(cN,methodNameId);

                const u2 methodDescrId = NAMEANDTYPE_GET_DESCRIPTIONID(cN,methodNameAndTypeId);
                const char* methodDescr = UTF8_GET_STRING(cN,methodDescrId);;
                const u2 methodDescrLength = UTF8_GET_LENGTH(cN,methodDescrId);
#endif

#ifdef ENABLE_KCLASS_FORMAT
#ifdef ENABLE_KMETHOD
                const u2 classId = classInfo;
#else
                const u2 classId = CLASSINFO_GET_NAMEID(cN,classInfo);
#endif
                cN = getClassIndex(classId);
#ifdef DEBUG_KCLASS
                const char* className = getClassName(classId);
                const u2 classNameLength = stringLength(className);
#endif
#else
                const u2 classNameId = CLASSINFO_GET_NAMEID(cN,classInfo);
                const char* className = UTF8_GET_STRING(cN,classNameId);
                const u2 classNameLength = UTF8_GET_LENGTH(cN,classNameId);

                cN = findClass(className, classNameLength);
                if (cN == INVALID_CLASS_ID)
                {
                    CLASSNOTFOUNDERR((const char*) className, classNameLength);
                }
#endif

#ifdef ENABLE_KMETHOD
                extern const char* getDebugMethodName(const u2 classId,const u2 methodId);
                printf("%s->%s:%s \n",className,getDebugMethodName(getClassID(cN), methodNameId),methodDescr);
                mN = methodNameId;
#else
                do{
                    mN = findMethodByName(cN,methodName, methodNameLength, methodDescr, methodDescrLength);
                }while (mN == INVALID_METHOD_ID && (cN = findSuperClass(cN)) != INVALID_CLASS_ID);

                if (mN == INVALID_METHOD_ID)
                {
                    METHODNOTFOUNDERR(methodName, className);
                }
                //printf("%s->%s:%s \n",UTF8_GET_STRING(cN,getClassID(cN)),methodName,methodDescr);
#endif
                const u2 methodInfo = getU2(cN,METHODBASE(cN, mN));
                opStackSetSpPos(opStackGetSpPos() + ((methodInfo & ACC_NATIVE) ? 0 : findMaxLocals(cN,mN)));

#ifndef TINYBAJOS_MULTITASKING
                if ( methodInfo & ACC_SYNCHRONIZED)
                {
                    if(isThreadLocked(cs[cN].classInfo,1))
                    {   // thread sleeps, try it later
                        // (BYTECODEREF));
                        opStackSetSpPos(methodStackPop() + k);
                        // before invoke
                        pc = methodStackPop() - 1;
                        pc += (code == INVOKEINTERFACE ? -2 : 0);
                        mN = methodStackPop();
                        cN = methodStackPop();
                        local = methodStackPop();
                        break;
                    }
                }
#endif
                // no synchronized,or I have the lock
                // now call the method
                if (methodInfo & ACC_NATIVE)
                {
                    //printf("Invoke native method:%s->%s\n",className,methodName);
                    const u2 classId = getClassID(cN);
                    if(classId == 46){
                        nativeDispath(1,classId,mN,methodName, methodDescr);

                       goto nativeVoidReturn;
                    }else
                    if ((cs[cN].nativeFunction != NULL)
                    && (cs[cN].nativeFunction[mN] != NULL))
                    {
                        const u2 classId = getClassID(cN);
                        if(classId == 46)
                            nativeDispath(1,classId,mN,methodName, methodDescr);

                        //PRINTF("Calling native method %d\n",mN);
                        if (cs[cN].nativeFunction[mN]())
                            goto nativeValueReturn;
                        else
                            goto nativeVoidReturn;
                    }
                    else
                    {
                        ERROREXIT(-3, "native method not found cN: %d mN: %d", cN, mN);
                    }
                }
                pc = getStartPC(cN,mN);

            }BREAK;
            nativeValueReturn:
            CASE(IRETURN):
            CASE(FRETURN):
            CASE(ARETURN):
            {
#ifdef DEBUG
                if(code == IRETURN) DEBUGPRINT_OPC("i");
                if(code == FRETURN) DEBUGPRINT_OPC("f");
                if(code == ARETURN) DEBUGPRINT_OPC("a");
#endif
                code = IRETURN;
            nativeVoidReturn:
                DEBUGPRINT_OPC("native ");
            }
            CASE(RETURN):
            {
                DEBUGPRINTLN_OPC("return");

#ifndef TINYBAJOS_MULTITASKING
                const u2 methodInfo = getU2(cN,METHODBASE(cN,mN));
                if (methodInfo & ACC_SYNCHRONIZED)
                {
                    // have I always the lock ?
                    if (methodInfo & ACC_STATIC)
                       updateThreadLock(cs[cN].classInfo);
                    else
                       updateThreadLock(opStackGetValue(local));
                }
#endif
                if (methodStackEmpty())
                {   //mb jf if not <clinit> you're done :-D
#ifdef ENABLE_KMETHOD
                    if(mN == cs[cN].clinitMethodId)//todo check what should happen for invalid clinitmethod
#else
                    if (STRNCMP("<clinit>",(char*)findMethodByMethodNumber(cN,mN),8) == 0)
#endif
                    {
                        DEBUGPRINTLN_OPC(" from <clinit>");

                        return;
                    }
                    else
                    {
#ifndef TINYBAJOS_MULTITASKING
                        PRINTF("Normally terminated Thread: %d\n",currentThreadCB->tid);
                        if (threadList.count != 1){
                            deleteThread();
                        }else{
                            return;
                        }
#else
                        free(methodStackGetBase());
                        free(opStackGetBase());
#endif
                        break;
                    }
                }
                const slot first = opStackPop();// ret val
                opStackSetSpPos(methodStackPop());
                pc = methodStackPop() + 2;
                mN = methodStackPop();
                cN = methodStackPop();
                local = methodStackPop();
                if (code == IRETURN) opStackPush(first);
            }BREAK;
            CASE(NEW):
            {
                DEBUGPRINT_OPC("new");
                pc += 2;
                methodStackPush(cN);
                methodStackPush(mN);

                const u2 classNameId = CLASSINFO_GET_NAMEID(cN,BYTECODEREF);

#ifdef ENABLE_KCLASS_FORMAT
                cN = getClassIndex(classNameId);
#ifdef DEBUG_KCLASS
                const char* className = getClassName(classNameId);
                const u2 classNameLength = stringLength(className);
#endif
#else
                const char* className = UTF8_GET_STRING(cN,classNameId);
                const u2 classNameLength = UTF8_GET_LENGTH(cN,classNameId);
                
                cN = findClass(className,classNameLength);

                if (cN == INVALID_CLASS_ID)//When using KClass format can't have invalid classes
                {
                    mN = methodStackPop();
                    cN = methodStackPop();
                    CLASSNOTFOUNDERR((char *) getAddr(cN,CP(cN, getU2(cN,CP(cN,BYTECODEREF)+1))+3),getU2(cN,CP(cN,getU2(CP(cN, getU2(cN,CP(cN,BYTECODEREF) + 1)) + 1)) + 1));
                }
#endif
                methodStackPush(cN);
                fN = 0;
                do 	{
                    for (int i = 0; i < getU2(cN,cs[cN].fields_count); i++)	// count normal fields
                    {
                        u2 fielddescr = cs[cN].constant_pool[getU2(cN,cs[cN].field_info[i] + 4)];
                        u1 isNotObject = STRNCMP("L",(const char*) getAddr(cN,fielddescr + 3), 1);

                        if ( (getU2(cN,cs[cN].field_info[i]) & ACC_FINAL) && isNotObject)
                            continue; // ignore static and non static primitive finals
                        if ( getU2(cN,cs[cN].field_info[i]) & ACC_STATIC)
                            continue;// ignore static
                        fN++;
                    }

                } while ((cN = findSuperClass(cN)) != INVALID_CLASS_ID);
                
                cN = methodStackPop();

                slot stackSlot;
                stackSlot.stackObj.classNumber = cN;
                // allocate on heap places for stackObject fields
                const u2 heapPos = heapAllocElement(fN,HEAPALLOCATEDNEWOBJECT,&stackSlot.stackObj,0);

                DEBUGPRINTLN_OPC(" -> push %x\n",heapPos);
                opStackPush(stackSlot);//reference to.stackObject on opStack
                
                for (int i = 0; i < fN; i++)// initialize the heap elements
                    heapSetElement(toSlot((u4)0), heapPos + i);

                mN = methodStackPop();
                cN = methodStackPop();
                // className
                DEBUGPRINTLNSTRING(className,classNameLength);

            }BREAK;
            CASE(NEWARRAY):
            {
                DEBUGPRINTLN_OPC("newarray");
                const s2 count = (s2)opStackPop().UInt;
                if (count < 0)
                {
                    NEGATIVEARRAYSIZEEXCEPTION;
                }
                if (count > MAX_ARRAY_SIZE)
                {
                    ARRAYINDEXOUTOFBOUNDSEXCEPTION;
                }
                if(byte1 == T_DOUBLE || byte1 == T_LONG){
                    DNOTSUPPORTED;
                }

                slot stackSlot;
                stackSlot.stackObj.arrayLength = (u1)count;
                // + marker
                const u2 heapPos = heapAllocElement(count,HEAPALLOCATEDARRAY,&stackSlot.stackObj,0);
                opStackPush(stackSlot);

                const slot zero = byte1 == T_FLOAT ? toSlot(0.f) : toSlot((u4)0);
                for (int i = 0; i < count; i++)
                    heapSetElement(zero,heapPos + i);

                pc++; // skip type
            }BREAK;
            CASE(ANEWARRAY):
            {
                DEBUGPRINTLN_OPC("anewarray");
                pc+=2;
                // index into the constant_pool.no verification
                s2 *cnt = (s2 *) malloc(sizeof(s2));
                *cnt = 0;
                // call recursive function to allocate heap for arrays
                opStackPush(createDims(1, cnt));
                free (cnt);
            }BREAK;
            CASE(ARRAYLENGTH):
            {
                DEBUGPRINTLN_OPC("arraylength");
                const slot first = opStackPop();
                if (first.UInt == NULLOBJECT.UInt)
                {
                    NULLPOINTEREXCEPTION;
                }
                else
                {
                    opStackPush(toSlot((u4)first.stackObj.arrayLength));
                }

            }BREAK;
            CASE(MONITORENTER):
            {
#ifndef TINYBAJOS_MULTITASKING
                const slot first = opStackPop();
                if(isThreadLocked(first,0)){
                    // thread sleeps, try it later
                    opStackSetSpPos(methodStackPop() + findNumArgs(cN,BYTECODEREF) + 1);
                    pc = pc - 1;              // before monitorenter
                    break;
                }
#else
                DNOTSUPPORTED;
#endif
            }BREAK;
            CASE(MONITOREXIT):                 // have I always the lock ?
            {
#ifndef TINYBAJOS_MULTITASKING
                const slot first = opStackPop();

                int i = 0;
                // must be in
                for (i = 0; i < MAXLOCKEDTHREADOBJECTS;i++)
                    if (currentThreadCB->hasMutexLockForObject[i].UInt == first.UInt)
                        break;

                // fertig
                if (currentThreadCB->lockCount[i] > 1)
                    currentThreadCB->lockCount[i]--;
                else
                {
                    currentThreadCB->lockCount[i] = 0;
                    // give lock free
                    currentThreadCB->hasMutexLockForObject[i] = NULLOBJECT;
                    const slot object = opStackGetValue(first.stackObj.pos);
                    HEAPOBJECTMARKER(object.UInt).mutex = MUTEXNOTBLOCKED;

                    updateThreadState(object, THREAD_ANY_STATE, THREADNOTBLOCKED, 0, 0, 0);
                }
#else
                DNOTSUPPORTED;
#endif
            }BREAK;
            CASE(ATHROW):
            {
                DEBUGPRINTLN_OPC("athrow");
#ifndef TINYBAJOS_EXCEPTION
                handleException();
#else
                exit(-101);
#endif

            }BREAK;
            CASE(CHECKCAST):
            CASE(INSTANCEOF):
            {
                DEBUGPRINTLN_OPC(code == INSTANCEOF ? "instanceof" : "checkcast");
                // In general, we try to cast as much as possible.
                // Only if we perfectly know that this cast is invalid, break it.
                slot first = code == INSTANCEOF ? opStackPop() : opStackPeek();

                u1 performcheck = 1;
                u1 invalidcast = 0;
                u1 isInstanceOf = 0;

                if (first.UInt != NULLOBJECT.UInt)
                {   // the cast's target class
                    const u2 targetclass = getU2(cN,0);
                    const u1 typeTag = GET_TAG(cN,targetclass);
                    const u2 classNameId = CLASSINFO_GET_NAMEID(cN,targetclass);

                    u2 target = INVALID_CLASS_ID;
#ifdef ENABLE_KCLASS_FORMAT
                    if(typeTag == CONSTANT_KClass)
                    {//todo - parse [Ljava.lang.String//this implementation won't work as it is now
                        target = getClassIndex(classNameId);
                    }else
#endif
                    {
                        char* classname = UTF8_GET_STRING(cN,classNameId);
                        u2 len = UTF8_GET_LENGTH(cN,classNameId);

                        // we have to make some dirty hacks here
                        //since we are not storing typing informations for arrays
                        if (*classname == '[')//getU1(classname)=='[')
                        {
                            while ('[' == *classname)//getU1(classname)=='[')
                            {
                                //we hope to get useful information
                                //from the objects stored in the array.
                                //this only takes the first object in the array,
                                //yet it could be extended to gathering
                                //all stored object's typing informations
                                if( first.stackObj.magic != OBJECTMAGIC || first.UInt == NULLOBJECT.UInt
                                ||  HEAPOBJECTMARKER(first.stackObj.pos).status != HEAPALLOCATEDARRAY)
                                {
                                    performcheck = 0;
                                    isInstanceOf = 0;
                                    invalidcast = 1;
                                    break;
                                }
                                first = heapGetElement(first.stackObj.pos + 1);
                                // remove the leading '['
                                --len;
                                ++classname;
                            }
                            if (first.UInt == NULLOBJECT.UInt)
                            {
                                performcheck = 0;
                                isInstanceOf = 1;
                            }
                            // A class identifier is Lclassname;
                            if ('L' == *classname)//getU1(classname)=='L')
                            {
                                len -= 2;
                                ++classname;
                            }
                            else
                            {	// a primitive type
                                performcheck = 0;
                                isInstanceOf = 1;
                            }
                        }
#ifndef ENABLE_KCLASS_FORMAT
                        target = findClass(classname, len);
#else
                        target = 0;
                        while (*classname != ';') {
                            target = target * 10 + (*classname - 48);
                            ++classname;
                        }
                        target = getClassIndex(target);
#endif
                    }

                    if (performcheck == 1)
                    {
                        methodStackPush(cN);
                        methodStackPush(mN);
#ifndef ENABLE_KCLASS_FORMAT
                        if (target == INVALID_CLASS_ID)
                        {
                            const char* classname = UTF8_GET_STRING(cN,classNameId);
                            CLASSNOTFOUNDERR(classname,len);
                        }
#endif
                        cN = first.stackObj.classNumber;
                        if (checkInstance(cN,target))
                        {
                            isInstanceOf = 1;
                        }
                        else
                        {
                            isInstanceOf = 0;
                            invalidcast = 1;
                        }
                        mN = methodStackPop();
                        cN = methodStackPop();
                    }
                    else
                    {
                        isInstanceOf = 0;
                    }
                }
                else
                {
                    if(code == INSTANCEOF)
                        isInstanceOf = 0;
                    else
                        pc += 2;
                }
                if(code == INSTANCEOF){
                    opStackPush(toSlot((u4)isInstanceOf));
                }else if (invalidcast == 1)
                {
                    opStackPop();
                    CLASSCASTEXCEPTION;
                }
            }BREAK;
            CASE(WIDE):
            {
                DEBUGPRINTLN_OPC("wide");
                // not tested because so many locals are hard to implement on purpose  14.12.2006
                u2 nextOp = getU1(cN,0); // which operation to extend?
                s2 count = getU2(cN,0);

                // if load operation...
                if (ILOAD <= nextOp && nextOp <= DLOAD)
                {
                    // embedded op code for load
                    opStackPush(opStackGetValue(local+count));
                }
                // if store operation...
                if (ISTORE <= nextOp && nextOp <= DSTORE)
                {
                    // embedded op code for store
                    opStackSetValue(local+count,opStackPop());
                    // write into locals (position, value from stack)
                }
                if (nextOp == RET) // if ret operation...
                {   // embedded op code for ret
                    // not tested because no exceptions implemented yet 14.12.2006
                    // the opcode of athrow is required
                    u2 addr = opStackGetValue(local + count).UInt;
                    pc = addr + getStartPC(cN,mN);  //pcMethodStart;	//assumtion): the address is the relative address, absolute address may be required
                }
                if (nextOp == IINC)                 // if iinc operation...
                {
                    // embedded op code for load
                    u2 constB = getU2(cN,0);        // constByte - only available with iinc in wide operation
                    // position
                    opStackSetValue((u2)(local + count),
                                    // old value
                                    toSlot((u4)(opStackGetValue(local + count).Int
                                                + constB))); // add const
                }
            }BREAK;
            CASE(MULTIANEWARRAY):
            {
                DEBUGPRINTLN_OPC("multianewarray");
                pc+=2;                            // index into the constant_pool. kjvm performs no verification
                u1 dim = getU1(cN,0);             // dimensions

                s2 *local_cnt = (s2 *) malloc(sizeof(s2));
                *local_cnt = 0;
                // call recursive function to allocate heap for arrays
                opStackPush(createDims(dim, local_cnt));
                free (local_cnt);
            }BREAK;
            CASE(GOTO_W):
            {
                DEBUGPRINTLN_OPC("goto_w (not tested)");
                // not tested because wide jumps are hard to implement on purpose  14.12.2006
                u4 addr = getU4(cN,0);
                pc = addr + getStartPC(cN,mN);//pcMethodStart; //assumtion: the address is the relative address, absolute address may be required

            }BREAK;
            CASE(JSR_W):
            {
                DEBUGPRINTLN_OPC("jsr_w (not tested)%d %d",byte1, byte2);
                // not tested because no exceptions implemented yet 14.12.2006
                // the opcode of athrow is required
                u4 my_addr = getU4(cN,0);
                opStackPush(toSlot(my_addr));
            }BREAK;
            CASE(LCONST_0):
            CASE(LCONST_1):
            CASE(DCONST_0):
            CASE(DCONST_1):
            CASE(LDC2_W):
            CASE(LLOAD):
            CASE(DLOAD):
            CASE(DLOAD_0):
            CASE(DLOAD_1):
            CASE(DLOAD_2):
            CASE(DLOAD_3):
            CASE(LALOAD):
            CASE(DALOAD):
            CASE(LSTORE):
            CASE(DSTORE):
            CASE(LSTORE_0):
            CASE(LSTORE_1):
            CASE(LSTORE_2):
            CASE(LSTORE_3):
            CASE(DSTORE_0):
            CASE(DSTORE_1):
            CASE(DSTORE_2):
            CASE(DSTORE_3):
            CASE(LASTORE):
            CASE(DASTORE):
            CASE(LADD):
            CASE(DADD):
            CASE(LSUB):
            CASE(DSUB):
            CASE(LMUL):
            CASE(DMUL):
            CASE(LDIV):
            CASE(DDIV):
            CASE(LREM):
            CASE(DREM):
            CASE(LNEG):
            CASE(DNEG):
            CASE(LSHL):
            CASE(LSHR):
            CASE(LUSHR):
            CASE(LAND):
            CASE(LOR):
            CASE(LXOR):
            CASE(I2L):
            CASE(I2D):
            CASE(L2I):
            CASE(L2F):
            CASE(L2D):
            CASE(F2L):
            CASE(F2D):
            CASE(D2I):
            CASE(D2L):
            CASE(D2F):
            CASE(LCMP):
            CASE(DCMPL):
            CASE(DCMPG):
            CASE(DRETURN):
            CASE(LRETURN):
            CASE(NOT_SUPPORTED):
            //default:
            {
                DEBUGPRINT_OPC("code:%d",code);
                DNOTSUPPORTED;
            }BREAK;
        }// switch

#ifndef TINYBAJOS_MULTITASKING
        scheduler();
#endif
    }

    PRINTF("Termination\n");

}

u1 isThreadLocked(const slot obj_slot,const u1 a)
{
#ifndef TINYBAJOS_MULTITASKING
    if (HEAPOBJECTMARKER(obj_slot.stackObj.pos).mutex == MUTEXNOTBLOCKED)
    {
        // mutex is free, I (the thread) have not the mutex and I can get the mutex for the object
        if(a) currentThreadCB->isMutexBlockedOrWaitingForObject = NULLOBJECT;
        HEAPOBJECTMARKER(obj_slot.stackObj.pos).mutex   = MUTEXBLOCKED;// get the lock
        int i = 0;
        // I had not the mutex for this object (but perhaps for others), now I have the look
        for (i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)
            if (currentThreadCB->hasMutexLockForObject[i].UInt != NULLOBJECT.UInt)
                continue;
            else
                break;
        if (i == MAXLOCKEDTHREADOBJECTS)
        {
            ERROREXIT(-1, "too many locks\n");
        }
        // entry for this object in the array of mutexed objects for the thread
        // count (before 0)
        currentThreadCB->lockCount[i] = 1;
        currentThreadCB->hasMutexLockForObject[i] = obj_slot;
    }// mutex is blocked, is it my mutex ? have I always the lock ?
    else
    {
        int i = 0;
        // have I always the lock ?
        for (i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)
            if (currentThreadCB->hasMutexLockForObject[i].UInt == obj_slot.UInt)
                break;

        // another thread has the lock
        if (i == MAXLOCKEDTHREADOBJECTS)
        {
            // mutex blocked
            currentThreadCB->state = THREADMUTEXBLOCKED;
            currentThreadCB->isMutexBlockedOrWaitingForObject = obj_slot;

            return 1;
        } // let the scheduler work
        else      // yes I have the lock count
            currentThreadCB->lockCount[i]++;
    }
#endif
    return 0;
}

void updateThreadLock(const slot object)
{
#ifndef TINYBAJOS_MULTITASKING
    int i = 0;
    // must be in
    for ( i = 0; i < MAXLOCKEDTHREADOBJECTS;i++)
        if ((currentThreadCB->hasMutexLockForObject[i]).UInt == object.UInt)
            break;
    // fertig
    if (currentThreadCB->lockCount[i] > 1)
        currentThreadCB->lockCount[i]--;
    else // last lock
    {
        currentThreadCB->lockCount[i]=0;
        // give lock free
        currentThreadCB->hasMutexLockForObject[i] = NULLOBJECT;
        currentThreadCB->isMutexBlockedOrWaitingForObject = NULLOBJECT;
        HEAPOBJECTMARKER(object.stackObj.pos).mutex = MUTEXNOTBLOCKED;

        updateThreadState(object, THREADMUTEXBLOCKED, THREADNOTBLOCKED, 1, 0, 0);
    }
#endif
}

// generalized single comparison of target class with class at addr in cN's constant pool.i
// keeps cN unchanged if target is no super class of cN.
// else cN is the super class of former cN which has target as super class.
u2 subCheck(const u2 classId,const u2 target,const u2 superClass)
{
    const u2 classNameId = CLASSINFO_GET_NAMEID(classId,superClass);
#ifdef ENABLE_KCLASS_FORMAT
    const u2 superClassId = getClassIndex(classNameId);
#else
    const char* className = UTF8_GET_STRING(classId,classNameId);
    const u2 classNameLength = UTF8_GET_LENGTH(classId,classNameId);
    const u2 superClassId = findClass(className, classNameLength);
#endif
    if (checkInstance(superClassId,target))
    {
        return superClassId;
    }

    return classId;
}


// receives object's class via classId and target class as parameter
// returns true / false
u1 checkInstance(const u2 classId,const u2 target)
{
    u2 retClassId = classId;
    if (retClassId != 0 && retClassId != target)
    {
        const u2 superClass = getU2(retClassId,cs[retClassId].super_class);
        // trying the super class.
        if (superClass > 0)
        {
            retClassId = subCheck(classId,target, superClass);
        }

        // trying the interfaces.
        if (retClassId != 0 && retClassId != target)
        {
            u2 n = getU2(retClassId,cs[retClassId].interfaces_count);
            if(n > 0){
                while (--n && retClassId != target)
                {
                    const u2 interface = getU2(retClassId,cs[retClassId].interfaces + n * 2);
                    retClassId = subCheck(retClassId,target, interface);
                }
            }
        }
    }

    return (target == retClassId);
}


slot createDims(const u4 dimsLeft, s2 *dimSize)
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
    else if (*dimSize > MAX_ARRAY_SIZE)
    {
        ARRAYINDEXOUTOFBOUNDSEXCEPTION;
    }
    else
    {
        act_array.stackObj.arrayLength = *dimSize;
        // + marker
        const u2 heapPos = heapAllocElement(*dimSize,HEAPALLOCATEDARRAY,&act_array.stackObj,0);
        //why mutex is missing? why status on heapPos and magic on next element
        //TODO fix this,it's is fucked
        s2 *cnt = (s2 *) malloc(sizeof(s2));
        *cnt = 0;
        for (int i = 0; i < *dimSize; ++i)
        {
            heapSetElement(createDims(dimsLeft - 1, cnt), heapPos + i);
        }
        free(cnt);
    }
    return act_array;
}


#ifndef TINYBAJOS_EXCEPTION
// Realizes an interpreter-raised Exception
//BH AM not tested
void raiseExceptionFromIdentifier(const Exception exception)
{
    methodStackPush(cN);
    methodStackPush(mN);

    extern u2 getExceptionClassId(const Exception exception);
#ifdef ENABLE_KCLASS_FORMAT
    cN = getClassIndex(getExceptionClassId(exception));
#else
    cN = getExceptionClassId(exception);
#endif

    if (cN == INVALID_CLASS_ID)
    {
        CLASSNOTFOUNDERR("Undefined Exception",19);
    }

    const u2 fieldsCount = getU2(cN,cs[cN].fields_count);

    slot stackSlot;
    stackSlot.stackObj.classNumber = cN;

    const u2 heapPos = heapAllocElement(fieldsCount,HEAPALLOCATEDNEWOBJECT,&stackSlot.stackObj,0);
    opStackPush(stackSlot);// reference to stackObject on opStack

    for (int i = 0; i < fieldsCount; i++)
    {
        heapSetElement(toSlot((u4) 0), heapPos + i);
    }

    mN = methodStackPop();
    cN = methodStackPop();

    handleException();
}


//BH AM not tested
void handleException()
{
    // this is actually the class we have to catch
    u1 classNumberFromPushedObject = opStackPeek().stackObj.classNumber;

    // number of catches the try block has
    u2 n = getU2(cN,METHODCODEEXCEPTIONBASE(cN, mN));

    DEBUGPRINTLN_OPC("trying to catch class number %d", classNumberFromPushedObject);
    DEBUGPRINTLN_OPC("%d catch clauses", n);
    for (int i = 0; i < n; ++i)
    {
        u2 cur_catch = METHODCODEEXCEPTIONBASE(cN, mN) + 8 * i;

        // checking if catch range is usable
        if (pc - getStartPC(cN,mN) - 1 < getU2(cN,cur_catch + 2) || pc - getStartPC(cN,mN) - 1 >= getU2(cN,cur_catch + 4))
        {
            DEBUGPRINTLN_OPC("pc: %d", pc - getStartPC(cN,mN) - 1);
            DEBUGPRINTLN_OPC("start: %d", getU2(cN,cur_catch + 2));
            DEBUGPRINTLN_OPC("end: %d", getU2(cN,cur_catch + 4));
            DEBUGPRINTLN_OPC("not my range");
            continue;
        }

        // checking whether the catch's catched class is in the code exception table
        methodStackPush(cN);

        const u2 classInfo = getU2(cN,cur_catch + 8);
        const u2 classNameId =  CLASSINFO_GET_NAMEID(cN,classInfo);
#ifdef ENABLE_KCLASS_FORMAT
        cN = getClassIndex(classNameId);
#else
        const char* className = UTF8_GET_STRING(cN,classNameId);
        const u2 classNameLength = UTF8_GET_LENGTH(cN,classNameId);

        cN = findClass(className,classNameLength);

        if (cN == INVALID_CLASS_ID)
        {
            DEBUGPRINTLN_OPC("Exception class not found:  %d\n", cN);
            cN = methodStackPop();
            continue;// class is not in the class table - broken code.
        }
#endif
        // Ya well, this is the catched class's number in code exception table
        u1 classNumberInCodeExceptionTable = cN;
        DEBUGPRINTLN_OPC("classNumberInCodeExceptionTable: %d",classNumberInCodeExceptionTable);

        cN = classNumberFromPushedObject;

        // start catching
        if (checkInstance(cN,classNumberInCodeExceptionTable))
        {
            DEBUGPRINTLN_OPC("catching!");
            cN = methodStackPop();
            pc = getStartPC(cN,mN) + getU2(cN,cur_catch + 6);
            return;
        }
        cN = methodStackPop();
    }
    
    // no catch clause found, so skip to the next stack frame
    if (methodStackEmpty())
    {
        DEBUGPRINTLN_OPC("we are thru, this was the top frame");
        cN = classNumberFromPushedObject;
        const u2 classInfo = getU2(cN,cs[cN].this_class);
        const u2 classNameId =  CLASSINFO_GET_NAMEID(cN,classInfo);
#ifdef ENABLE_KCLASS_FORMAT
        const char* className = getClassName(classNameId);
#else
        const char* className = UTF8_GET_STRING(cN,classNameId);
#endif
        UNHANDLEDEXCEPTIONERR(className);
    }
    else
    {
        DEBUGPRINTLN_OPC("popping stack frame");
        const slot first = opStackPop();
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
