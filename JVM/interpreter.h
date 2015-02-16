/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07

#ifndef __INTERPRETER__
#define __INTERPRETER__
#define     NOP         0x00
#define     ACONST_NULL 0x01                      // modified by mb jf
#define     ICONST_M1   0x02
#define     ICONST_0    0x03
#define     ICONST_1    0x04
#define     ICONST_2    0x05
#define     ICONST_3    0x06
#define     ICONST_4    0x07
#define     ICONST_5    0x08
#define     LCONST_0    0x09                      //long
#define     LCONST_1    0x0a
#define     FCONST_0    0x0b
#define     FCONST_1    0x0c
#define     FCONST_2    0x0d
#define     DCONST_0    0x0e
#define     DCONST_1    0x0f
#define     BIPUSH      0x10
#define     SIPUSH      0x11
#define     LDC         0x12
#define     LDC_W       0x13
#define     LDC2_W      0x14
#define     ILOAD       0x15
#define     LLOAD       0x16
#define     FLOAD       0x17
#define     DLOAD       0x18
#define     ALOAD       0x19
#define     ILOAD_0     0x1a
#define     ILOAD_1     0x1b
#define     ILOAD_2     0x1c
#define     ILOAD_3     0x1d
#define     LLOAD_0     0x1e
#define     LLOAD_1     0x1f
#define     LLOAD_2     0x20
#define     LLOAD_3     0x21
#define     FLOAD_0     0x22
#define     FLOAD_1     0x23
#define     FLOAD_2     0x24
#define     FLOAD_3     0x25
#define     DLOAD_0     0x26
#define     DLOAD_1     0x27
#define     DLOAD_2     0x28
#define     DLOAD_3     0x29
#define     ALOAD_0     0x2a
#define     ALOAD_1     0x2b
#define     ALOAD_2     0x2c
#define     ALOAD_3     0x2d
#define     IALOAD      0x2e                      //mb jf
#define     LALOAD      0x2f                      //mb jf
#define     FALOAD      0x30                      //mb jf
#define     DALOAD      0x31                      //mb jf
#define     AALOAD      0x32                      //mb jf
#define     BALOAD      0x33                      //mb jf
#define     CALOAD      0x34                      //mb jf
#define     SALOAD      0x35                      //mb jf
#define     ISTORE      0x36
#define     LSTORE      0x37
#define     FSTORE      0x38
#define     DSTORE      0x39
#define     ASTORE      0x3A
#define     ISTORE_0    0x3B
#define     ISTORE_1    0x3C
#define     ISTORE_2    0x3D
#define     ISTORE_3    0x3E
#define     LSTORE_0    0x3F
#define     LSTORE_1    0x40
#define     LSTORE_2    0x41
#define     LSTORE_3    0x42
#define     FSTORE_0    0x43
#define     FSTORE_1    0x44
#define     FSTORE_2    0x45
#define     FSTORE_3    0x46
#define     DSTORE_0    0x47
#define     DSTORE_1    0x48
#define     DSTORE_2    0x49
#define     DSTORE_3    0x4A
#define     ASTORE_0    0x4b
#define     ASTORE_1    0x4c
#define     ASTORE_2    0x4d
#define     ASTORE_3    0x4e
#define     IASTORE     0X4F                      //mb jf
#define     LASTORE     0X50                      //mb jf
#define     FASTORE     0X51                      //mb jf
#define     DASTORE     0X52                      //mb jf
#define     AASTORE     0X53                      //mb jf
#define     BASTORE     0X54                      //mb jf
#define     CASTORE     0X55                      //mb jf
#define     SASTORE     0X56                      //mb jf
#define     POP         0x57
#define     POP2        0x58
#define     DUP         0x59
#define     DUP_X1      0x5a
#define     DUP_X2      0x5b
#define     DUP2        0x5c
#define     DUP2_X1     0x5d
#define     DUP2_X2     0x5e
#define     SWAP        0x5f
#define     IADD        0x60
#define     LADD        0x61
#define     FADD        0x62
#define     DADD        0x63
#define     ISUB        0x64                      // modified by mb jf
#define     LSUB        0x65
#define     FSUB        0x66
#define     DSUB        0x67                      // modified by mb jf
#define     IMUL        0x68
#define     LMUL        0x69
#define     FMUL        0x6A                      // modified by mb jf
#define     DMUL        0x6B
#define     IDIV        0x6C                      // modified by mb jf
#define     LDIV        0x6D
#define     FDIV        0x6E                      // modified by mb jf
#define     DDIV        0x6F
#define     IREM        0x70
#define     LREM        0x71
#define     FREM        0x72
#define     DREM        0x73
#define     INEG        0x74
#define     LNEG        0x75
#define     FNEG        0x76
#define     DNEG        0x77
#define     ISHL        0x78
#define     LSHL        0x79
#define     ISHR        0x7A
#define     LSHR        0x7B
#define     IUSHR       0x7C
#define     LUSHR       0x7D
#define     IAND        0x7E
#define     LAND        0x7F
#define     IOR         0x80
#define     LOR         0x81
#define     IXOR        0x82
#define     LXOR        0x83
#define     IINC        0x84                      //modified: mb, jf
#define     I2L         0x85
#define     I2F         0x86
#define     I2D         0x87
#define     L2I         0x88
#define     L2F         0x89
#define     L2D         0x8A
#define     F2I         0x8B
#define     F2L         0x8C
#define     F2D         0x8D
#define     D2I         0x8E
#define     D2L         0x8F
#define     D2F         0x90
#define     I2B         0x91
#define     I2C         0x92
#define     I2S         0x93
#define     LCMP        0x94
#define     FCMPL       0x95
#define     FCMPG       0x96
#define     DCMPL       0x97
#define     DCMPG       0x98
#define     IFEQ        0x99                      //mb, jf
#define     IFNE        0x9a                      //mb, jf
#define     IFLT        0x9b                      //mb, jf
#define     IFGE        0x9c                      //mb, jf
#define     IFGT        0x9d                      //mb, jf
#define     IFLE        0x9e                      //mb, jf
#define     IF_ICMPEQ   0x9f                      //mb, jf
#define     IF_ICMPNE   0xa0                      //mb, jf
#define     IF_ICMPLT   0xa1                      //mb, jf
#define     IF_ICMPGE   0xa2                      //mb, jf
#define     IF_ICMPGT   0xa3                      //mb, jf
#define     IF_ICMPLE   0xa4                      //mb, jf
#define     IF_ACMPEQ   0xa5                      //mb, jf
#define     IF_ACMPNE   0xa6                      //mb, jf
#define     GOTO        0xa7                      //mb, jf
#define     JSR         0xa8                      //mb, jf
#define     RET         0xa9                      //mb, jf
#define     TABLESWITCH 0xaa                      //mb, jf
#define     LOOKUPSWITCH    0xab                  //mb, jf
#define     IRETURN     0xac                      //mb, jf
#define     LRETURN     0xad                      //mb, jf
#define     FRETURN     0xae                      //mb, jf
#define     DRETURN     0xaf                      //mb, jf
#define     ARETURN     0xb0                      //mb, jf
#define     RETURN      0xb1                      // modified by mb jf
#define     GETSTATIC   0xb2                      //mb jf
#define     PUTSTATIC   0xb3                      //mb jf
#define     GETFIELD    0xb4                      // modified by mb jf
#define     PUTFIELD    0xb5                      // modified by mb jf
#define     INVOKEVIRTUAL   0xb6                  // modified by mb jf
#define     INVOKESPECIAL   0xb7                  // modified by mb jf
#define     INVOKESTATIC    0xb8                  // modified by mb jf
#define     INVOKEINTERFACE 0xb9
/*
 186 (0xba) xxxunusedxxx
 Reserved opcodes:
 202 (0xca) breakpoint
 254 (0xfe) impdep1
 255 (0xff) impdep2
 */
#define     NEW         0xbb                      // modified by mb jf
#define     NEWARRAY    0xbc                      //mb jf
#define     ANEWARRAY   0xbd                      //mb jf
#define     ARRAYLENGTH 0xbe                      //mb jf
#define     ATHROW      0xbf
#define     CHECKCAST   0xc0                      // to do
#define     INSTANCEOF  0xc1                      // to do
#define     MONITORENTER    0xc2
#define     MONITOREXIT     0xc3

#define     WIDE        0xc4                      //mb jf
#define     MULTIANEWARRAY  0xc5                  //mb jf
#define     IFNULL      0xc6                      //mb jf
#define     IFNONNULL   0xc7                      //mb jf
#define     GOTO_W      0xc8                      //mb jf
#define     JSR_W       0xc9                      //mb jf
void run(void);
u1  checkInstance(const u2 target);
void raiseExceptionFromIdentifier(const char identifier[], const u1 length);
void handleException(void);
void subCheck(u2 target, u2 addr);

slot createDims(u4 dimsLeft, s2 *count);
#endif
