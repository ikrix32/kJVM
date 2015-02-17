/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
#if LINUX || AVR32LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "kjvm.h"
#include "heap.h"
#include <string.h>
#ifdef AVR8
#include <avr/pgmspace.h>
#endif
extern void printStringFromFlash(u1*, u1);
#include "classfile.h"

extern const char* nativeClassNames[];
extern u2 numNativeClassNames;
extern const functionForNativeMethodType* funcArray[];
extern functionForNativeMethodType functionForNativePlatFormMethod[];

#ifdef AVR8
/* ARDUINOMEGA holds the classfiles in instruction flash!*/
/* RAMPZ = 1 (ever) -> classfiles are in te upper half of atmega128 flash */
/* address of classfile byte in flash 0x0000 to 0xffff */
/* eg. address 0x2345 means byte in flash at address (binary):1001000110100101 */
extern u1 getU1Flash(u1* pos);
u1 getU1(const u2 pos)
{
    return getU1Flash((u1*)(CLASSSTA + ((pos==0) ? (pc++) : pos)));
}


#else
/* classSTA and pc are global variables for actual class and method*/
/* parameter != 0 -> value at parameter-pos*/
/* parameter ==0 -> value at global var pc and automatic increment */
u1 getU1(const u2 pos)
{
    return *(CLASSSTA + ((pos == 0) ? (pc++) : pos));
}
#endif

u2 getU2(const u2 pos)
{
    //if (pos==0) {pc+=2;return (*(CLASSSTA + pc-2))*256+*(CLASSSTA + pc-1);}
    //else return (*(CLASSSTA + pos)*256)+*(CLASSSTA + pos+1);
    //}
    return (((u2) getU1(pos) << 8) | (u2) getU1((pos == 0) ? 0 : pos + 1));
}


u4 getU4(const u2 pos)
{
    return (((u4) getU2(pos) << 16) | (u4) getU2((pos == 0) ? 0 : pos + 2));
}


f4 getFloat(const u2 pos)
{
    return getU4(pos);
}


//BH#define	getAddr(m)	(CLASSSTA+m)
void* getAddr(const u2 pos)
{
    return CLASSSTA + pos;
}


u1 findNumArgs(const u2 methodRef)                      /*  count BCDFIJLSZ in runden Klammern!!*/
{
    u2 i;
    u2 n = 0;
    u1 object = 0;
    u2 meth = CP(cN, getU2(CP(cN, getU2(CP(cN, methodRef) + 3)) + 3));
    for (i = 0; i < getU2(meth + 1); i++)
    {
        u1 c = getU1(meth + 3 + i);               //BH*(u1*)(getAddr(meth+3)+i);
        if (c == '(')
            continue;
        if (c == ')')
            break;
        if (c == ';')
            object = 0;
        if (!object)
        {
            if (c == 'L')
            {
                object = 1;
                n++;
            } else if ((c == 'B') || (c == 'C') || (c == 'F') || (c == 'I')
                       || (c == 'S') || (c == 'Z'))
            {
                n++;
            }
        }
    }
    return n;
}


u2 findMaxLocals()                                /*cN,mN*/
{
    return getU2(getStartPC() - 6);
}


/* in cN fieldName fieldDescr*/
/* out cN, fN of normal field in object (non static, non final primitive fields)*/
/* return 1 -> found */
u1 findFieldByName(const char* fieldName,const u1 fieldNameLength,
                   const char* fieldDescr, const u1 fieldDescrLength)
{
    u1	i;
    u1	found = 0;
    fN = 0;
    do
    {
        u1 numFields = getU2(cs[cN].fields_count);
        for (i = 0; i < numFields; ++i)
        {
            u2 fielddescr = cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 4)];
            u1 isNotObject =  	STRNCMPRAMFLASH ("L",(const char*) getAddr(fielddescr + 3), 1);
            if ( (getU2(cs[cN].field_info[i]) & ACC_FINAL) && isNotObject) continue; // ignore static and non static primitive finals
            if ( getU2(cs[cN].field_info[i]) & ACC_STATIC) continue;// ignore static
            u2 fieldname = cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)];

            if (fieldNameLength == getU2(fieldname + 1) &&
                STRNCMPFLASHFLASH
                (fieldName, (const char*) getAddr(fieldname + 3),getU2(fieldname + 1)) == 0 &&
                fieldDescrLength == getU2(fielddescr + 1) &&
                STRNCMPFLASHFLASH
                (fieldDescr, (const char*) getAddr(fielddescr + 3),
                 getU2(fielddescr + 1)) == 0)
            {
                found=1; break;
            }

            fN++;
        }
        if (found )            return 1;
    } while (findSuperClass());
    return 0;
}


u1 findFieldByRamName(const char* fieldName,const u1 fieldNameLength, // for normal fields
                      const char* fieldDescr,const u1 fieldDescrLength) // only use in scheduler
{
    u1 i;
    u1 found = 0;
    fN = 0;
    do
    {
        u1 numFields = getU2(cs[cN].fields_count);
        for (i = 0; i < numFields; ++i)
        {
            u2 fielddescr = cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 4)];
            u1 isNotObject =  STRNCMPRAMFLASH ("L",(const char*) getAddr(fielddescr + 3), 1);
            if ( (getU2(cs[cN].field_info[i]) & ACC_FINAL)&& isNotObject) continue; // ignore static and non static primitive finals
            if ( getU2(cs[cN].field_info[i]) & ACC_STATIC) continue;// ignore static
            u2 fieldname = cs[cN].constant_pool[getU2(
                                                      cs[cN].field_info[i] + 2)];
            if (fieldNameLength == getU2(fieldname + 1) &&
                STRNCMPRAMFLASH
                (fieldName, (const char*) getAddr(fieldname + 3),
                 getU2(fieldname + 1)) == 0 && fieldDescrLength
                == getU2(fielddescr + 1) &&
                STRNCMPRAMFLASH
                (fieldDescr, (const char*) getAddr(fielddescr + 3),
                 getU2(fielddescr + 1)) == 0)
            {
                found=1; break;
            }
            fN++;
        }
        if (found )            return 1;
    } while (findSuperClass());
    return 0;
}

u1 findStaticFieldByName(const char* fieldName,const u1 fieldNameLength,
                         const char* fieldDescr,const u1 fieldDescrLength)	{
    u1	i;
    u1	found = 0;
    fN = 0;
    u1 numFields = getU2(cs[cN].fields_count);
    for (i = 0; i < numFields; ++i)
    {
        u2 fielddescr = cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 4)];
        u1 isNotObject =  STRNCMPRAMFLASH  ("L",(const char*) getAddr(fielddescr + 3), 1);
        if (! ( getU2(cs[cN].field_info[i]) & ACC_STATIC)) continue; // ignore non static
        if ( (getU2(cs[cN].field_info[i]) & ACC_FINAL) &&  isNotObject) continue; // non object finals
        u2 fieldname = cs[cN].constant_pool[getU2(cs[cN].field_info[i] + 2)];
        if (fieldNameLength == getU2(fieldname + 1) &&
            STRNCMPFLASHFLASH
            (fieldName, (const char*) getAddr(fieldname + 3),
             getU2(fieldname + 1)) == 0 && fieldDescrLength
            == getU2(fielddescr + 1) &&
            STRNCMPFLASHFLASH
            (fieldDescr, (const char*) getAddr(fielddescr + 3),
             getU2(fielddescr + 1)) == 0)
        {
            found=1; break;
        }
        fN++;
    }
    if (found )            return 1;
    return 0;

}


u1 findMethod(const char* className, const u1 classNameLength,
              const char* methodName, const u1 methodNameLength,
              const char* methodDescr, const u1 methodDescrLength)
{
    // all args in flash for arduinomega
    /*in cN, out: cN, mN*/
    /* recursive down to object*/
    //printf("%4x %4x %4x %4x\n", 0x8000+(unsigned int)className/2,classNameLength,0x8000+ (unsigned int)methodName/2,methodNameLength);
    if (!FIND_CLASS(className, classNameLength))
    {
        CLASSNOTFOUNDERR((const char*) className, classNameLength);
    }                                             /* out: cN*/
    if (
#ifdef AVR8
        findMethodByNameFlash
#else
        findMethodByName
#endif
        (methodName, methodNameLength, methodDescr, methodDescrLength))
        return 1;
    else if (classNameLength == 16 &&
             STRNCMPRAMFLASH
             ("java/lang/Object", className, classNameLength) == 0)
    {
        return 0;                                 /* not found*/
    } else
        return findMethod(
                          getAddr(
                                  CP(cN, getU2(CP(cN, getU2(cs[cN].super_class)) + 1))
                                  + 3),
                          getU2(CP(cN, getU2(CP(cN, getU2(cs[cN].super_class)) + 1)) + 1),
                          methodName, methodNameLength, methodDescr, methodDescrLength);
}


#ifdef AVR8
u1 findMethodByNameFlash(const char* name, const u1 len, const char* methodDescr, const u1 methodDescrLength)
{
    /*  in: classNumber cN, out: methodNumber mN*/
    /* non recursive */
    for (mN=0; mN < getU2(cs[cN].methods_count); mN++)
        if (len==getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+1))
            if(strncmpFlashFlash(name,(char*)getAddr(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+3),
                                 getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+2)]+1))==0)
            {
                if (methodDescr!=NULL)
                {
                    if (methodDescrLength==getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+1))
                        if(strncmpFlashFlash(methodDescr,(char*)getAddr(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+3),
                                             getU2(cs[cN].constant_pool[getU2(METHODBASE(cN,mN)+4)]+1))==0) return 1;
                }
                else return 1;
            }
    return 0;
}
#endif

u1 findMethodByName(const char* name, const u1 len, const char* methodDescr,
                    const u1 methodDescrLength)
{
    /*  in: classNumber cN, out: methodNumber mN*/
    /* non recursiv*/
    for (mN = 0; mN < getU2(cs[cN].methods_count); mN++)
        if (len == getU2(
                         cs[cN].constant_pool[getU2(METHODBASE(cN, mN) + 2)] + 1))
            if (
#ifdef AVR8
                strncmpRamFlash
#else
                strncmp
#endif
                (
                 name,
                 (char*) getAddr(
                                 cs[cN].constant_pool[getU2(METHODBASE(cN, mN) + 2)]
                                 + 3),
                 getU2(
                       cs[cN].constant_pool[getU2(METHODBASE(cN, mN) + 2)]
                       + 1)) == 0)
            {
                if (methodDescr != NULL)
                {
                    if (methodDescrLength == getU2(
                                                   cs[cN].constant_pool[getU2(METHODBASE(cN, mN) + 4)]
                                                   + 1))
                        if (
                            STRNCMPRAMFLASH
                            (
                             methodDescr,
                             (char*) getAddr(
                                             cs[cN].constant_pool[getU2(
                                                                        METHODBASE(cN, mN) + 4)] + 3),
                             getU2(
                                   cs[cN].constant_pool[getU2(
                                                              METHODBASE(cN, mN) + 4)] + 1))
                            == 0)
                            return 1;
                } else
                    return 1;
            }
    return 0;
}


u1* findMethodByMethodNumber()                    /*mb jf  in: methodNumber, out: methodName*/
{
    u2 methAddr = METHODBASE(cN, mN);             /* get start address in class file of method #methodNumber*/
    u2 methodNameAddr = getU2(methAddr + 2);      /* 2 ... offset from begin of method in class file to method's name*/
    return getAddr(CP(cN, methodNameAddr) + 3);   /* return pointer to field value in class cN at address methodNameAddress*/
}


/* return 1 found, 0 -> super class ist object*/
/* in cN out cN*/
u1 findSuperClass()
{
    if (16 == getU2(
                    cs[cN].constant_pool[getU2(
                                               cs[cN].constant_pool[getU2(cs[cN].this_class)] + 1)] + 1) &&
        STRNCMPRAMFLASH
        (
         "java/lang/Object",
         getAddr(
                 cs[cN].constant_pool[getU2(
                                            cs[cN].constant_pool[getU2(
                                                                       cs[cN].this_class)] + 1)] + 3), 16)
        == 0)
        return 0;                                 /* cN is class Object */
    FIND_CLASS(
               getAddr(
                       cs[cN].constant_pool[getU2(
                                                  cs[cN].constant_pool[getU2(cs[cN].super_class)] + 1)]
                       + 3),
               getU2(
                     cs[cN].constant_pool[getU2(
                                                cs[cN].constant_pool[getU2(cs[cN].super_class)] + 1)]
                     + 1));
    return 1;
}


#ifdef AVR8
/* out: cN*/
u1 findClassFlash(const char* className,const u1 classNameLength)
{
    for (cN=0; cN < numClasses;cN++)
    {
        if (classNameLength !=(u2)getU2(
                                        cs[cN].constant_pool[
                                                             getU2(cs[cN].constant_pool[
                                                                                        getU2(cs[cN].this_class)]+1
                                                                   )]+1))
        {
            continue;
        }
        if (strncmpFlashFlash(className,(const char*)getAddr(cs[cN].constant_pool[
                                                                                  getU2( cs[cN].constant_pool[
                                                                                                              getU2(cs[cN].this_class)]+1
                                                                                        )
                                                                                  ]+3),classNameLength)==0)
        {
            return 1;
        }
    }
    return 0;
}
#endif

/* out: cN*/
u1 findClass(const char* className,const u1 classNameLength)
{
    for (cN = 0; cN < numClasses; cN++)
    {

        const u2 c_pool = cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)]+ 1)];
        const u1    cclassnameLength=getU2(c_pool + 1);

        //printf("Searching: %s,%d - crt %s,%d\n",className,classNameLength,cclassName,cclassnameLength);

        if (classNameLength != (u2) cclassnameLength)
        {
            continue;
        }
        const char* cclassName = getAddr(c_pool + 3);
        if (STRNCMPRAMFLASH(className,cclassName, classNameLength) == 0)
        {
            return 1;
        }
    }
    return 0;
}


void analyzeClass()	// input: cN
{
    u2 ln;
    pc = 0;

    DEBUGPRINTF("class number:\t \t\t%X   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", cN);
    cs[cN].magic = pc;                                /* relative position in classfile*/
    DEBUGPRINTF("cf\tmagic:\t %X\t", getU4(pc));
    pc = 4;                                       /* NOT +=4, because getU*(pc) increases pc when pc = 0 */
    cs[cN].minor_version = pc++;                      /* 4*/
    cs[cN].major_version = ++pc;                      /* 6*/
    if (getU2(cs[cN].major_version) > 52)
        ERROREXIT( 1,
                  "this java version is not supported yet: cs[cN].major_version %d\n",
                  getU2(cs[cN].major_version));
    DEBUGPRINTF("version:\t%d.%d\n", getU2(pc),getU2(pc-2));
    pc += 2;
    cs[cN].constant_pool_count = pc;
    DEBUGPRINTF("cf\tconstant_pool_count:\t%d\n", getU2(pc));
    pc += 2;
    if ((cs[cN].constant_pool = (u2*) malloc(
                                             /* -2 möglich wegen 0!!*/
                                             sizeof(u2) * getU2(cs[cN].constant_pool_count))) == NULL)
        ERROREXIT(2,"malloc error\n");

    analyzeConstantPool();
    cs[cN].access_flags = pc;
    DEBUGPRINTF("cf\taccess_flags: %x\n",getU2(pc));
    pc += 2;
    cs[cN].this_class = pc;
    DEBUGPRINTF("cf\tthis_class: %d\n",getU2(pc));
    pc += 2;
    cs[cN].super_class = pc;
    DEBUGPRINTF("cf\tsuper_class: %d\n",getU2(pc));
    pc += 2;
    cs[cN].interfaces_count = pc;
    DEBUGPRINTF("cf\tinterfaces_count: %d\n",getU2(pc));
    pc += 2;
    cs[cN].interfaces = pc;
    pc += getU2(cs[cN].interfaces_count) * 2;
    cs[cN].fields_count = pc;
    DEBUGPRINTF("cf\tfields_count: %d\n",getU2(pc));
    pc += 2;
    cs[cN].field_info = NULL;
    if (getU2(cs[cN].fields_count) != 0)
    {
        if ((cs[cN].field_info = (u2*) malloc(sizeof(u2) * getU2(cs[cN].fields_count))) == NULL)
            ERROREXIT(3,"malloc error");
        analyzeFields();
    }
    cs[cN].methods_count = pc;
    DEBUGPRINTF("cf\tmethods_count: %d\n",getU2(pc));
    pc += 2;
    cs[cN].method_info = NULL;
    if (getU2(cs[cN].methods_count) != 0)
    {
        if ((cs[cN].method_info = (u2*) malloc(
                                               2 * sizeof(u2) * getU2(cs[cN].methods_count))) == NULL)
            ERROREXIT(4,"malloc error");
        analyzeMethods();
    }
    ln = getU2(0);                                // class file attribute count
    DEBUGPRINTF("cf-attributes: count: %d ",ln);
    // SourceFile (6), Deprecated (4),InnerClasses,EnclosingMethod,Synthetic,Signature
}


void analyzeConstantPool()
{
    u2 n, anz, length;
    anz = getU2(cs[cN].constant_pool_count);
    for (n = 1; n < anz; n++)
    {
        cs[cN].constant_pool[n] = pc;
        switch (getU1(0))
        {
            case CONSTANT_Class:/*    7 */
            {
                DEBUGPRINTF("\tcp %d\t:Class\t\t-> name:\t%d\n",n,getU2(pc));
                pc += 2;
            }break;
            case CONSTANT_String:             /*    8 */
            {
                DEBUGPRINTF("\tcp %d\t:String\t\t-> string:\t%d\n",n,getU2(pc));
                pc+=2;
            }break;
            case CONSTANT_Fieldref:           /*    9 */
            {
                DEBUGPRINTF("\tcp %d\t:Fieldref\t-> class: %d ", n,getU2(pc));
                DEBUGPRINTF("name_and_type:\t%d\n", getU2(pc+2));
                pc+=4;
            }break;
            case CONSTANT_InterfaceMethodref: /*   11 */
            {
                DEBUGPRINTF("\tcp %d\t:InterfaceMethodref->class: %d ",n,getU2(pc));
                DEBUGPRINTF("name_and_type_index\t%d\n",getU2(pc+2));
                pc+=4;
            }break;
            case CONSTANT_Methodref:          /*    10  nur Methoden, die aufgerufen werden!!!*/
            {
                DEBUGPRINTF("\tcp %d\t:Methodref\t-> class: %d ",n,getU2(pc));
                DEBUGPRINTF("name_and_type:\t%d\n", getU2(pc+2));
                pc+=4;
            }break;
            case CONSTANT_Integer:            /*    3*/
            {
                DEBUGPRINTF("cp %d\t: Integer -> name:\t%d\n",n,getU4(pc));
                pc+=4;
            }break;
            case CONSTANT_NameAndType:        /*    12*/
            {
                DEBUGPRINTF("\tcp %d\t:nameAndType\t-> name:\t%d ",n,getU2(pc));
                DEBUGPRINTF("descriptor:\t %d\n",getU2(pc+2));
                pc+=4;
            }break;
            case CONSTANT_Float:              /*    4 */
            {
#ifdef DEBUG
#ifdef AVR8                           // change all avr8 string to flash strings gives more data ram space for java!!
                {
                    u1 float_str[7];
                    dtostrf( getFloat(pc), 7, 3, float_str );
                    printf_P(PSTR("\tcp %d\t:Float:\t%s  \n"),n,float_str);
                }
#else
                printf("\tcp %d\t:Float:\t%f  \n",n,getFloat(pc));
#endif
#endif
                pc+=4;
            }break;
            case CONSTANT_Long:               /*    5 */
            {    DNOTSUPPORTED;
            }break;
            case CONSTANT_Double:             /*    6 */
            {   DNOTSUPPORTED;
            }break;
            case CONSTANT_Utf8:               /*    1*/
            {
                length=getU2(0);
                DEBUGPRINTF("\tcp %d\t:Utf8:\t\t-> ",n);
#ifdef DEBUG
                u2 i;
                for (i=0; i < length;i++)
                    DEBUGPRINTF("%c",getU1(pc+i));
                /* utf8 chars ???*/
                DEBUGPRINTF("\n");
#endif
                pc += length;
            }break;
            default: ERROREXIT(5, "invalid constant pool identifier\n");
        }
    }
}


void analyzeMethods()            /* jan 08 not good tested*/
{
    int i, n, m, a;
    u2 etl;
    cs[cN].nativeFunction = NULL;
    for (n = 0; n < getU2(cs[cN].methods_count); n++) /*methods*/
    {
        cs[cN].method_info[n] = pc;                   /* absolute in classfile*/
        DEBUGPRINTF("\tmethod %d\taccess_flags: %04x",n,getU2(pc));
        DEBUGPRINTF(" name: %04x ",getU2(pc+2));
        DEBUGPRINTF(" descriptor: %04x ",getU2(pc+4));
        DEBUGPRINTF(" \tattribute_count: %04x\n",a=getU2(pc+6));
        /*Signature*/
        //BHDEBUGPRINTSTRING(METHODDESCRSTR(cN,n),METHODDESCRSTRLENGTH(cN,n));
        a = getU2(pc + 6);
        pc += 8;
        if (a == 0)
        {
            for (i = 0; i
                 < (/*sizeof(nativeClassNames)/sizeof(char*)*/numNativeClassNames); i++)
                if (!
                    STRNCMPRAMFLASH
                    (
                     nativeClassNames[i],
                     (char*) getAddr(
                                     cs[cN].constant_pool[getU2(
                                                                cs[cN].constant_pool[getU2(cs[cN].this_class)]
                                                                + 1)] + 3),
                     getU2(
                           cs[cN].constant_pool[getU2(
                                                      cs[cN].constant_pool[getU2(cs[cN].this_class)]
                                                      + 1)] + 1)))
                {
                    cs[cN].nativeFunction
                    = (functionForNativeMethodType*) funcArray[i];
                    break;
                }
            continue;                             /* native method*/
        }
        //Code(var), Exception(var),Synthetic (4),Signature,Deprecated(4)
        for (m = 0; m < a; m++)                   /* attributes of method*/
        {
            const char* adr = getAddr(cs[cN].constant_pool[getU2(0)] + 1 + 2);
            if (STRNCMPRAMFLASH("Code", adr, 4) == 0)
            {
                DEBUGPRINTF("\t\tCode: attribute_length: %d\n",getU4(pc));
                DEBUGPRINTF("\t\tCode: max_stack: %d\n", getU2(pc+4));
                DEBUGPRINTF("\t\tCode: max_locals: %d\n",getU2(pc+6));
                DEBUGPRINTF("\t\tCode: code_length: %d pc: %d\n\t\t",getU4(pc+8),pc);
                pc += 12;
#ifdef DEBUG
                for (i = 0; i < getU4( pc - 4); i++)
                    DEBUGPRINTF("%2x ",getU1(pc + i));
                /*length*/
#endif
                pc += getU4(pc - 4);
                etl = getU2(0);
                DEBUGPRINTF("\n\t\tCode: exception_table_length: %d\n",etl);
#ifdef DEBUG
                /* exception_table	*/

                for (i = 0;i < etl;i++)
                {
                    DEBUGPRINTF("\t\t\texception: nr: %d startPC: %d\n",i,getU2(pc+8*i));
                    DEBUGPRINTF("\t\t\texception: nr: %d endPC: %d\n",i,getU2(8*i+2));
                    DEBUGPRINTF("\t\t\texception: nr: %d catchType: %d\n",i,getU2(8*i+6));
                    DEBUGPRINTF("\t\t\texception: nr: %d catchType: %d\n",i,getU2(8*i+6));
                }
#endif
                pc += etl * 8;
                u2 h = getU2(0);
                DEBUGPRINTF("\t\tCode: attributes_count: %d\n", h);
                //LineNumberTable(var),LocalVariableTable(var),StackMapTable
                for (i = 0; i < h; i++)
                {
                    const char*addr = getAddr(cs[cN].constant_pool[getU2(0)] + 3);
                    if (STRNCMPRAMFLASH("LineNumberTable", addr, 15) == 0)
                    {
                        pc = getU4(0) + pc;
                        continue;
                    }

                    if (STRNCMPRAMFLASH("StackMapTable", addr, 13) == 0)
                    {
                        pc = getU4(0) + pc;
                        continue;
                    }

                    if (STRNCMPRAMFLASH("LocalVariableTable", addr, 18) == 0)
                    {
                        pc = getU4(0) + pc;
                        continue;
                    }
                    ERROREXIT(6, "unsupported code attribute");
                }                                 // code attributes
                continue;
            }                                     // code
            if (STRNCMPRAMFLASH("Exceptions", adr, 10) == 0)
            {
                DEBUGPRINTF("exception object\n");
                mN = n;
                u4 n2 = getU4(0);                 /*attribute_length. don't need that.*/
                n2 = getU2(0);
#ifdef DEBUG
                for (i=0;i<n2;i++)
                    DEBUGPRINTF("\t\t\texception: nr: %d class: %d\n",i,getU2(pc+2*i));
#endif
                pc += 2 * n2;
                /*pc=(u2)getU4(0)+pc;*/
                continue;
            }                                     //Exceptions
            if (STRNCMPRAMFLASH("Synthetic", adr, 9) == 0)
            {
                pc += 4;
                continue;
            }
            if (STRNCMPRAMFLASH("Deprecated", adr, 10) == 0)
            {
                pc += 4;
                continue;
            }
            if (STRNCMPRAMFLASH("Signature", adr, 9) == 0)
            {
                pc = (u2) getU4(0) + pc;
                continue;
            }
            ERROREXIT(7, "unsupported method attribute");
        }                                         // method attributes
    }                                             /* methods_count*/
}


void analyzeFields()
{
    u2 n, a, cur_a;
    fN = 0; // count static fields
    for (n = 0 ;n < getU2(cs[cN].fields_count); n++)  /*num fields*/
    {
        cs[cN].field_info[n] = pc;                    /* absolute in classfile*/
        DEBUGPRINTF("\tfield %x\taccess_flags: %d\n",n,getU2(pc));
        DEBUGPRINTF("\tfield %d\tname: %d\n",n,getU2(pc+2));
        DEBUGPRINTF("\tfield %d\tdescriptor: %d\n",n,getU2(pc+4));
        DEBUGPRINTF("\tfield %d\tattribute_count: %d\n",n,getU2(pc+6));
        u2 fielddescr = cs[cN].constant_pool[getU2(cs[cN].field_info[n] + 4)];
        u1 isNotObject =
        STRNCMPRAMFLASH
        ("L",(const char*) getAddr(fielddescr + 3), 1);

        //printf("cN %d n %d A %c fN %d \n",cN,n,*(const char*)getAddr(fielddescr + 3),fN);
        if ((ACC_STATIC & getU2(pc)) && !((ACC_FINAL & getU2(pc)) && isNotObject))
            // count only normal static fields and final static fields in  class object
            fN++;
        pc += 6;

        a = getU2(0);                             // num field attribute
                                                  // ConstantValue(6),Synthetic(4),Signature(6) ,Deprecated(4)
        for (cur_a = 0; cur_a < a; ++cur_a)  // field attributes
        {
            u2 attribute_name_index = getU2(0);
            u1 attribute_name = cs[cN].constant_pool[attribute_name_index];
            u4 attribute_length = getU4(0);

            if (
                STRNCMPRAMFLASH
                ("ConstantValue", getAddr(attribute_name + 3), 13) == 0)	// nothing to do for jvm
            {
                pc += attribute_length;           /* continue*/
                continue;                         // next attribute test
            }
            if (
                STRNCMPRAMFLASH
                ("Synthetic", getAddr(attribute_name + 3), 9) == 0)
            {
                pc += 4;
                continue;
            }
            if (
                STRNCMPRAMFLASH
                ("Deprecated", getAddr(attribute_name + 3), 10) == 0)
            {
                pc += 4;
                continue;
            }
            if (
                STRNCMPRAMFLASH
                ("Signature", getAddr(attribute_name + 3), 9) == 0)
            {
                pc += 6;
                continue;
            }
            ERROREXIT(8, "unsupported field attribute");
        }                                         // field attribute count
    }                                             // numfields

    u2 heapPos=getFreeHeapSpace(fN + 1);/* allocate on heap places for stackObject fields*/
    for (n=0; n < fN;n++)               /* initialize the heap elements*/
        heapSetElement(toSlot( (u4) 0), heapPos+n+1);

    HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDSTATICCLASSOBJECT;
    HEAPOBJECTMARKER(heapPos).mutex=MUTEXNOTBLOCKED;
    HEAPOBJECTMARKER(heapPos).rootCheck=1;
    HEAPOBJECTMARKER(heapPos).magic= OBJECTMAGIC;

    cs[cN].classInfo.stackObj.pos = heapPos;
    cs[cN].classInfo.stackObj.magic = OBJECTMAGIC;
    cs[cN].classInfo.stackObj.classNumber = cN;
}                                                 // end analyze fields


u2 getStartPC()                                   /*cN,mN*/
{
    u2 attrLength = 0;                            /* search code-position*/
    u2 i;
    /* number of attributes*/
    for (i = 0; i < getU2(METHODBASE(cN, mN) + 6); i++)
    {
        if (
            STRNCMPRAMFLASH
            (
             "Code",
             getAddr(
                     cs[cN].constant_pool[getU2(
                                                METHODBASE(cN, mN) + 8 + attrLength)] + 3), 4)
            == 0)
            return (u2) METHODBASE(cN, mN) + 8 + 14 + attrLength;
        /*+attrLength;		????*/
        attrLength = getU4(METHODBASE(cN, mN) + 8) + 6;
    }                                             /* < 64K	*/
    return -1;
}
