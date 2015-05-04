
#include "stdlib.h" //malloc
#include "definitions.h"
#include "kjvm.h"
#include "heap.h"
#include "nstring.h"

#include "classfile.h"

extern const char* nativeClassNames[];
extern u2 numNativeClassNames;
extern const functionForNativeMethodType* funcArray[];
extern functionForNativeMethodType functionForNativePlatFormMethod[];

/* classSTA and pc are global variables for actual class and method*/
/* parameter != 0 -> value at parameter-pos*/
/* parameter ==0 -> value at global var pc and automatic increment */
u1 getU1(const u1 classId,const u2 pos)
{
    return *((cs[classId].classFileStartAddress) + ((pos == 0) ? (pc++) : pos));
}

u2 getU2(const u1 classId,const u2 pos)
{
    //if (pos==0) {pc+=2;return (*(CLASSSTA + pc-2))*256+*(CLASSSTA + pc-1);}
    //else return (*(CLASSSTA + pos)*256)+*(CLASSSTA + pos+1);
    //}
    return (((u2) getU1(classId,pos) << 8) | (u2) getU1(classId,(pos == 0) ? 0 : pos + 1));
}


u4 getU4(const u1 classId,const u2 pos)
{
    return (((u4) getU2(classId,pos) << 16) | (u4) getU2(classId,(pos == 0) ? 0 : pos + 2));
}


f4 getFloat(const u1 classId,const u2 pos)
{
    return getU4(classId, pos);
}


//BH#define	getAddr(m)	(CLASSSTA+m)
void* getAddr(const u1 classId,const u2 pos)
{
    return (cs[classId].classFileStartAddress) + pos;
}


u1 findNumArgs(const u1 classId,const u2 methodRef)
{
    u2 n = 0;
    u1 object = 0;
    const u2 methodNameAndTypeId = METHODREF_GET_NAME_AND_TYPEID(cN,methodRef);
    const u2 methodDescrId = NAMEANDTYPE_GET_DESCRIPTIONID(cN,methodNameAndTypeId);

    const char* methodDescr = UTF8_GET_STRING(cN,methodDescrId);
    const u2    methodDescrLength = UTF8_GET_LENGTH(cN,methodDescrId);

    for (u2 i = 0; i < methodDescrLength; i++)
    {
        const u1 c = methodDescr[i];
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
            } else  if((c == 'B') || (c == 'C') || (c == 'F') || (c == 'I')
                       || (c == 'S') || (c == 'Z'))
            {
                n++;
            }
        }
    }
    return n;
}


u2 findMaxLocals(const u1 classId,const u1 methodId)                                /*cN,mN*/
{
    return getU2(classId,getStartPC(classId,methodId) - 6);
}


/* in cN fieldName fieldDescr*/
/* out cN, fN of normal field in object (non static, non final primitive fields)*/
/* return 1 -> found */
//instanceClassId is current intance class,fieldClassId represents class where is defined the field, class fieldId = superClass.fieldsCount+field index
u1 findFieldByName(const u2 instanceClassId,const u2 fieldClassId,const char* fieldName,const u1 fieldNameLength,
                   const char* fieldDescr, const u1 fieldDescrLength,const u1 isStatic)
{
    fN = INVALID_FIELD_ID;
    u1  class = instanceClassId;
    u1 f = 0;
    do
    {
        const u1 numFields = getU2(class,cs[class].fields_count);
        for (u1 i = 0; i < numFields; ++i)
        {
            const u2 crtFieldInfo = getU2(class,cs[class].field_info[i]);
            const u2 crtFieldDescrId = getU2(class,cs[class].field_info[i] + 4);

            //const u2 fielddescr = cs[cN].constant_pool[fieldDescrId];

            if (crtFieldInfo & ACC_FINAL){
                const char* crtFieldDescr = UTF8_GET_STRING(class, crtFieldDescrId);
                const u1 isNotObject= STRNCMPRAMFLASH ("L",crtFieldDescr, 1);
                if(isNotObject)
                    continue; // ignore static and non static primitive finals
            }

            if ((!isStatic && crtFieldInfo & ACC_STATIC) || (isStatic && !(crtFieldInfo & ACC_STATIC)))
                continue;// ignore static/non static depends what kind of file are we searching

            const u2 crtFieldNameId = getU2(class,cs[class].field_info[i] + 2);
            const u2 crtFieldNameLen = UTF8_GET_LENGTH(class, crtFieldNameId);// getU2(cN,fieldname + 1);
            const u2 crtFielsDescLen = UTF8_GET_LENGTH(class, crtFieldDescrId);//getU2(cN,fielddescr + 1);

            if(fieldNameLength == crtFieldNameLen && fieldDescrLength == crtFielsDescLen)
            {
                const char* crtFieldName = UTF8_GET_STRING(class, crtFieldNameId);
                if(STRNCMPFLASHFLASH(fieldName,crtFieldName, fieldNameLength) == 0)
                {
                    const char* crtFieldDescr = UTF8_GET_STRING(class, crtFieldDescrId);//(const char*) getAddr(cN,fielddescr + 3);
                    if( STRNCMPFLASHFLASH(fieldDescr, crtFieldDescr, fieldDescrLength) == 0)
                    {
                        fN = f;
                    }
                }
            }
            f++;
        }
        if (fN != INVALID_FIELD_ID && class == fieldClassId)
            break;
    } while ((class = findSuperClass(class)) != INVALID_CLASS_ID);
    if(fN != INVALID_FIELD_ID){
        cN = class;
    }
    return fN != INVALID_FIELD_ID;
}

//Returns methodId(mN)
u1 findMethodByName(const u1 classId,const char* name, const u1 len, const char* methodDescr,const u1 methodDescrLength)
{
    // in: classNumber cN, out: methodNumber mN
    // non recursiv
    const u2 methodsCount = getU2(classId,cs[classId].methods_count);
    for (u1 methodId = 0; methodId < methodsCount; methodId++)
    {
        const u2 methodNameId = getU2(classId,METHODBASE(classId, methodId) + 2);
        const u2 methodNameLen = UTF8_GET_LENGTH(classId, methodNameId);
        if (len == methodNameLen)
        {
            const char* methodName = UTF8_GET_STRING(classId, methodNameId);
            if (STRNCMPRAMFLASH(name,methodName,methodNameLen) == 0)
            {
                if (methodDescr != NULL)
                {
                    const u2 methodDescriptorId = getU2(classId,METHODBASE(classId, methodId) + 4);
                    const u2 methodDescriptorLength = UTF8_GET_LENGTH(classId, methodDescriptorId);
                    if (methodDescrLength == methodDescriptorLength)
                    {
                        const char* methodDescriptorStr = UTF8_GET_STRING(classId, methodDescriptorId);
                        if (STRNCMPRAMFLASH(methodDescr,methodDescriptorStr,methodDescriptorLength) == 0)
                            return methodId;
                    }
                } else
                    return methodId;
            }
        }
    }
    return INVALID_METHOD_ID;
}


u1* findMethodByMethodNumber(const u1 classId,const u1 methodId)/*mb jf  in: methodNumber, out: methodName*/
{
    const u2 methAddr = METHODBASE(classId, methodId);       /* get start address in class file of method #methodNumber*/
    const u2 methodNameAddr = getU2(classId,methAddr + 2);   /* 2 ... offset from begin of method in class file to method's name*/
    return getAddr(classId,CP(classId, methodNameAddr) + 3); /* return pointer to field value in class cN at address methodNameAddress*/
}

// Returns super class id, param: class id
u1 findSuperClass(const u1 classId)
{
    const u2 supperClassInfoId = getU2(classId,cs[classId].super_class);

    if (supperClassInfoId == 0)
        return INVALID_CLASS_ID;

    const u2 classNameId = CLASSINFO_GET_NAMEID(classId,supperClassInfoId);

#ifdef ENABLE_KCLASS_FORMAT
    return getClassIndex(classNameId);//in kclass format nameId was replaced with classId
#else
    const u2 classNameLength = UTF8_GET_LENGTH(classId, classNameId);
    const char* className = UTF8_GET_STRING(classId, classNameId);

    return findClass(className,classNameLength);
#endif
}

#ifndef ENABLE_KCLASS_FORMAT
u1 findClass(const char* className,const u1 classNameLength)
{
    for (int classId = 0; classId < numClasses; classId++)
    {
        const u2 classInfoId = getU2(classId,cs[classId].this_class);
        const u2 classNameId = CLASSINFO_GET_NAMEID(classId,classInfoId);
        const u2 cclassnameLength = UTF8_GET_LENGTH(classId,classNameId);
        if (classNameLength != (u2) cclassnameLength)
            continue;

        const char* cclassName = UTF8_GET_STRING(classId, classNameId);
        if (STRNCMPRAMFLASH(className,cclassName, classNameLength) == 0)
        {
            //printf("\nclass:%s id:%d \n",className,cN);
            return classId;
        }
    }
    return INVALID_CLASS_ID;
}
#else
u2 getClassIndex(u2 classId)
{
    for (int i = 0; i < numClasses; i++) {
        const u2 classInfoId = getU2(i,cs[i].this_class);
        const u2 classNameId = CLASSINFO_GET_NAMEID(i,classInfoId);
        if(classId == 0 )
        {
            // printf("classNameId:%d classInfoId:%d i:%d this:%d\n",classNameId,classInfoId,i,cs[i].this_class);
        }
        if(classNameId == classId)
            return i;
    }
    CLASSNOTFOUNDERR("aa",2);
    return INVALID_CLASS_ID;
}
u2 getClassID(u2 classIndex){
    if(classIndex < numClasses) {
        const u2 classInfoId = getU2(classIndex,cs[classIndex].this_class);
        const u2 classNameId = CLASSINFO_GET_NAMEID(classIndex,classInfoId);
        return classNameId;
    }
    return INVALID_CLASS_ID;
}
#endif

void analyzeClass(const u1 classId)//3956bytes all analyze methods
{
    pc = 0;

    DEBUG_CL_PRINTF("class number:\t \t\t%X   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", classId);
    cs[classId].magic = pc;                                // relative position in classfile
    DEBUG_CL_PRINTF("cf\tmagic:\t %X\t", getU4(classId,pc));
    pc = 4;// NOT +=4, because getU*(pc) increases pc when pc = 0

    cs[classId].minor_version = pc++;                 // 4
    cs[classId].major_version = ++pc;                  // 6
    if (getU2(classId,cs[classId].major_version) > 52)
        ERROREXIT(1,"this java version is not supported yet: cs[classId].major_version %d\n",
                  getU2(classId,cs[classId].major_version));
    DEBUG_CL_PRINTF("version:\t%d.%d\n", getU2(classId,pc),getU2(classId,pc-2));

    pc += 2;
    cs[classId].constant_pool_count = pc;
    DEBUG_CL_PRINTF("cf\tconstant_pool_count:\t%d\n", getU2(classId,pc));
    pc += 2;
    const int constantPoolSize = sizeof(u2) * getU2(classId,cs[classId].constant_pool_count);
    DEBUG_CL_PRINTF("cf\tconstant_pool_size:\t%d\n", constantPoolSize);
    if ((cs[classId].constant_pool = (u2*) malloc(constantPoolSize)) == NULL)
        ERROREXIT(2,"malloc error\n");

    analyzeConstantPool(classId);

    cs[classId].access_flags = pc;
    DEBUG_CL_PRINTF("cf\taccess_flags: %x\n",getU2(classId,pc));

    pc += 2;
    cs[classId].this_class = pc;
    DEBUG_CL_PRINTF("cf\tthis_class: %d\n",getU2(classId,pc));

    pc += 2;
    cs[classId].super_class = pc;
    DEBUG_CL_PRINTF("cf\tsuper_class: %d\n",getU2(classId,pc));

    pc += 2;
    cs[classId].interfaces_count = pc;
    DEBUG_CL_PRINTF("cf\tinterfaces_count: %d\n",getU2(classId,pc));

    pc += 2;
    cs[classId].interfaces = pc;
    pc += getU2(classId,cs[classId].interfaces_count) * 2;

    cs[classId].fields_count = pc;
    DEBUG_CL_PRINTF("cf\tfields_count: %d\n",getU2(classId,pc));

    pc += 2;
    cs[classId].field_info = NULL;
    if (getU2(classId,cs[classId].fields_count) != 0)
    {
        if ((cs[classId].field_info = (u2*) malloc(sizeof(u2) * getU2(classId,cs[classId].fields_count))) == NULL)
            ERROREXIT(3,"malloc error");
        analyzeFields(classId);
    }
    cs[classId].methods_count = pc;
    DEBUG_CL_PRINTF("cf\tmethods_count: %d\n",getU2(classId,pc));

    pc += 2;
    cs[classId].method_info = NULL;
    if (getU2(classId,cs[classId].methods_count) != 0)
    {
        if ((cs[classId].method_info = (u2*) malloc( 2 * sizeof(u2) * getU2(classId,cs[classId].methods_count))) == NULL)
            ERROREXIT(4,"malloc error");
        analyzeMethods(classId);
    }

#ifdef ENABLE_KMETHOD
    //todo - should be last
    cs[classId].clinitMethodId = getU2(classId, pc);
    pc += 2;
    cs[classId].mainMethodId = getU2(classId, pc);
#endif
    const u2 ln = getU2(classId,0);                                // class file attribute count
    DEBUG_CL_PRINTF("cf-attributes: count: %d ",ln);

    // SourceFile (6), Deprecated (4),InnerClasses,EnclosingMethod,Synthetic,Signature
}


void analyzeConstantPool(const u1 classId)
{
    //int constantsSize = 0;
    const int anz = getU2(classId,cs[classId].constant_pool_count);
    for (int n = 1; n < anz; n++)
    {
        cs[classId].constant_pool[n] = pc;
        const u1 type = getU1(classId,0);
        
        switch (type)
        {
            case CONSTANT_KClass:
            {
                DEBUG_CL_PRINTF("\tcp %d\t:Class\t\t-> ID:\t%d\n",n,getU2(classId,pc));
                pc += 2;
            }break;
            case CONSTANT_Class:
            {
                DEBUG_CL_PRINTF("\tcp %d\t:Class\t\t-> name:\t%d\n",n,getU2(classId,pc));
                pc += 2;
            }break;
            case CONSTANT_String:
            {
                DEBUG_CL_PRINTF("\tcp %d\t:String\t\t-> string:\t%d\n",n,getU2(classId,pc));
                pc += 2;
            }break;
            case CONSTANT_KFIELD_REF:
            case CONSTANT_Fieldref:
            {
                DEBUG_CL_PRINTF("\tcp %d\t:Fieldref\t-> class: %d ", n,getU2(classId,pc));
                DEBUG_CL_PRINTF("name_and_type:\t%d\n", getU2(classId,pc+2));
                pc += 4;
            }break;
            case  CONSTANT_KINTERFACE_MEHOD_REF:
            case CONSTANT_InterfaceMethodref:
            {
                DEBUG_CL_PRINTF("\tcp %d\t:InterfaceMethodref->class: %d ",n,getU2(classId,pc));
                DEBUG_CL_PRINTF("name_and_type_index\t%d\n",getU2(classId,pc+2));
                pc += 4;
            }break;
            case CONSTANT_KMEHOD_REF:
            case CONSTANT_Methodref:
            {
                DEBUG_CL_PRINTF("\tcp %d\t:Methodref\t-> class: %d ",n,getU2(classId,pc));
                DEBUG_CL_PRINTF("name_and_type:\t%d\n", getU2(classId,pc+2));
                pc += 4;
            }break;
            case CONSTANT_Integer:
            {
                DEBUG_CL_PRINTF("cp %d\t: Integer -> name:\t%d\n",n,getU4(classId,pc));
                pc += 4;
            }break;
            case CONSTANT_KNAME_AND_TYPE:
            case CONSTANT_NameAndType:
            {
                DEBUG_CL_PRINTF("\tcp %d\t:nameAndType\t-> name:\t%d ",n,getU2(classId,pc));
                DEBUG_CL_PRINTF("descriptor:\t %d\n",getU2(classId,pc+2));
                pc += 4;
            }break;
            case CONSTANT_Float:
            {
#ifdef DEBUG_CLASS_LOADING
                DEBUG_CL_PRINTF("\tcp %d\t:Float:\t%f  \n",n,getFloat(classId,pc));
#endif
                pc += 4;
            }break;
            case CONSTANT_Long:
            {    DNOTSUPPORTED;
            }break;
            case CONSTANT_Double:
            {   DNOTSUPPORTED;
            }break;
            case CONSTANT_Utf8:
            {
                const u2 length = getU2(classId,0);
                //constantsSize += length;
                DEBUG_CL_PRINTF("\tcp %d\t:Utf8:\t\t-> ",n);
#ifdef DEBUG_CLASS_LOADING
                for (int i = 0; i < length; i++)
                    DEBUG_CL_PRINTF("%c",getU1(classId, pc + i));
                
                DEBUG_CL_PRINTF("\n");
#endif
                pc += length;
            }break;
            default: ERROREXIT( 5, "invalid constant pool identifier\n");
        }
    }
    //printf("Constants Size: %d\n",constantsSize);
}


void analyzeMethods(const u1 classId)//2900bytes
{
    //int i, n, m, a;
    //u2 etl;
    cs[classId].nativeFunction = NULL;
    const u2 methodsCount = getU2(classId,cs[classId].methods_count);
    for (int n = 0; n < methodsCount; ++n) //methods
    {
        cs[classId].method_info[n] = pc;                   // absolute in classfile
        DEBUG_CL_PRINTF("\tmethod %d\taccess_flags: %04x",n,getU2(classId,pc));
        DEBUG_CL_PRINTF(" name: %04x ",getU2(classId,pc + 2));
        DEBUG_CL_PRINTF(" descriptor: %04x ",getU2(classId,pc + 4));
        DEBUG_CL_PRINTF(" \tattribute_count: %04x\n",getU2(classId,pc + 6));
        //Signature
        //BHDEBUGPRINTSTRING(METHODDESCRSTR(classId,n),METHODDESCRSTRLENGTH(classId,n));
        const u2 a = getU2(classId,pc + 6);
        pc += 8;
        if (a == 0)
        {
            const u2 classInfoId = getU2(classId,cs[classId].this_class);
            const u2 classNameId = CLASSINFO_GET_NAMEID(classId, classInfoId);
#ifdef ENABLE_KCLASS_FORMAT
            //todo - implement new native method dispach
            extern char* getClassName(const u2 classId);
            const char* className = getClassName(classNameId);
            const u2 classNameLength=stringLength(className);
#else
            const u2 classNameLength = UTF8_GET_LENGTH(classId,classNameId);
            const char* className = UTF8_GET_STRING(classId,classNameId);
#endif
            for (int i = 0; i < numNativeClassNames; i++)
            {
                if (!STRNCMPRAMFLASH(nativeClassNames[i],className,classNameLength))
                {
                    cs[classId].nativeFunction = (functionForNativeMethodType*) funcArray[i];
                    break;
                }
            }
            continue;//native method
        }
        //Code(var), Exception(var),Synthetic (4),Signature,Deprecated(4)
        for (int m = 0; m < a; m++)// attributes of method
        {
            const char* adr = getAddr(classId,CP(classId,getU2(classId,0)) + 1 + 2);
            if (STRNCMPRAMFLASH("Code", adr, 4) == 0)
            {
                DEBUG_CL_PRINTF("\t\tCode: attribute_length: %d\n",getU4(classId,pc));
                DEBUG_CL_PRINTF("\t\tCode: max_stack: %d\n", getU2(classId,pc + 4));
                DEBUG_CL_PRINTF("\t\tCode: max_locals: %d\n",getU2(classId,pc + 6));
                DEBUG_CL_PRINTF("\t\tCode: code_length: %d pc: %d\n\t\t",getU4(classId,pc + 8),pc);
                pc += 12;
#ifdef DEBUG_CLASS_LOADING
                for (int i = 0; i < getU4(classId,pc - 4); i++)
                    DEBUG_CL_PRINTF("%2x ",getU1(classId,pc + i));
                //length
#endif
                pc += getU4(classId,pc - 4);
                const u2 etl = getU2(classId,0);
                DEBUG_CL_PRINTF("\n\t\tCode: exception_table_length: %d\n",etl);
#ifdef DEBUG_CLASS_LOADING // exception_table
                for (int i = 0;i < etl;i++)
                {
                    DEBUG_CL_PRINTF("\t\t\texception: nr: %d startPC: %d\n",i,getU2(classId,pc + 8 * i));
                    DEBUG_CL_PRINTF("\t\t\texception: nr: %d endPC: %d\n",i,getU2(classId,8 * i + 2));
                    DEBUG_CL_PRINTF("\t\t\texception: nr: %d catchType: %d\n",i,getU2(classId,8 * i + 6));
                    DEBUG_CL_PRINTF("\t\t\texception: nr: %d catchType: %d\n",i,getU2(classId,8 * i + 6));
                }
#endif
                pc += etl * 8;
                const u2 h = getU2(classId,0);
                DEBUG_CL_PRINTF("\t\tCode: attributes_count: %d\n", h);
                //LineNumberTable(var),LocalVariableTable(var),StackMapTable
                for (int i = 0; i < h; i++)
                {
                    const char* addr = getAddr(classId,CP(classId,getU2(classId,0)) + 3);
                    if (STRNCMPRAMFLASH("LineNumberTable", addr, 15) == 0)
                    {
                        pc = getU4(classId,0) + pc;
                        continue;
                    }

                    if (STRNCMPRAMFLASH("StackMapTable", addr, 13) == 0)
                    {
                        pc = getU4(classId,0) + pc;
                        continue;
                    }

                    if (STRNCMPRAMFLASH("LocalVariableTable", addr, 18) == 0)
                    {
                        pc = getU4(classId,0) + pc;
                        continue;
                    }
                    ERROREXIT(6, "unsupported code attribute");
                }                                 // code attributes
                continue;
            }                                     // code
            if (STRNCMPRAMFLASH("Exceptions", adr, 10) == 0)
            {
                DEBUG_CL_PRINTF("exception object\n");
                mN = n;

                u4 n2 = getU4(classId,0);
                //attribute_length. don't need that.
                n2 = getU2(classId,0);
#ifdef DEBUG_CLASS_LOADING
                for (int i = 0;i < n2;i++)
                    DEBUG_CL_PRINTF("\t\t\texception: nr: %d class: %d\n",i,getU2(classId,pc + 2 * i));
#endif
                pc += 2 * n2;
                //pc=(u2)getU4(0)+pc;
                continue;
            }//Exceptions
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
                pc = (u2) getU4(classId,0) + pc;
                continue;
            }
            ERROREXIT(7, "unsupported method attribute");
        }                                         // method attributes
     }                                            // methods_count
}


void analyzeFields(const u1 classId)//600bytes
{
    fN = 0; // count static fields
    for (int n = 0 ; n < getU2(classId,cs[classId].fields_count); n++)  //num fields
    {
     cs[classId].field_info[n] = pc;                    // absolute in classfile
        DEBUG_CL_PRINTF("\tfield %x\taccess_flags: %d\n",n,getU2(classId,pc));
        DEBUG_CL_PRINTF("\tfield %d\tname: %d\n",n,getU2(classId,pc+2));
        DEBUG_CL_PRINTF("\tfield %d\tdescriptor: %d\n",n,getU2(classId,pc+4));
        DEBUG_CL_PRINTF("\tfield %d\tattribute_count: %d\n",n,getU2(classId,pc+6));
        const u2 fielddescr = CP(classId,getU2(classId,cs[classId].field_info[n] + 4));
        const u1 isNotObject= STRNCMPRAMFLASH("L",(const char*) getAddr(classId,fielddescr + 3), 1);

        //printf("classId %d n %d A %c fN %d \n",cN,n,*(const char*)getAddr(classId,fielddescr + 3),fN);
        if ((ACC_STATIC & getU2(classId,pc)) && !((ACC_FINAL & getU2(classId,pc)) && isNotObject))
            // count only normal static fields and final static fields in  class object
            fN++;

        pc += 6;
        const u2 a = getU2(classId,0);           // num field attribute
                                                 // ConstantValue(6),Synthetic(4),Signature(6) ,Deprecated(4)
        for (int cur_a = 0; cur_a < a; ++cur_a)  // field attributes
        {
            const u2 attribute_name_index = getU2(classId,0);
            const u1 attribute_name = CP(classId,attribute_name_index);
            const u4 attribute_length = getU4(classId,0);

            if (STRNCMPRAMFLASH("ConstantValue", getAddr(classId,attribute_name + 3), 13) == 0)	// nothing to do for jvm
            {
                pc += attribute_length;           // continue
                continue;                         // next attribute test
            }
            if (STRNCMPRAMFLASH("Synthetic", getAddr(classId,attribute_name + 3), 9) == 0)
            {
                pc += 4;
                continue;
            }
            if (STRNCMPRAMFLASH("Deprecated", getAddr(classId,attribute_name + 3), 10) == 0)
            {
                pc += 4;
                continue;
            }
            if (STRNCMPRAMFLASH("Signature", getAddr(classId,attribute_name + 3), 9) == 0)
            {
                pc += 6;
                continue;
            }
            ERROREXIT(8, "unsupported field attribute");
        }                                         // field attribute count
    }                                             // numfields

     const u2 heapPos = getFreeHeapSpace(fN + 1);// allocate on heap places for stackObject fields
     for (int n = 0; n < fN;n++)         // initialize the heap elements
        heapSetElement(toSlot( (u4) 0), heapPos + n + 1);

    HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDSTATICCLASSOBJECT;
    HEAPOBJECTMARKER(heapPos).mutex = MUTEXNOTBLOCKED;
    HEAPOBJECTMARKER(heapPos).rootCheck = 1;
    HEAPOBJECTMARKER(heapPos).magic= OBJECTMAGIC;

    cs[classId].classInfo.stackObj.pos = heapPos;
    cs[classId].classInfo.stackObj.magic = OBJECTMAGIC;
    cs[classId].classInfo.stackObj.classNumber = classId;
}                                                 // end analyze fields

u2 getStartPC(const u1 classId,const u1 methodId)
{
     u2 attrLength = 0;  
     // search code-position
     // number of attributes
    for (u2 i = 0; i < getU2(classId,METHODBASE(classId, methodId) + 6); i++)
    {
        if (STRNCMPRAMFLASH("Code",getAddr(classId,CP(classId,getU2(classId,METHODBASE(classId, methodId) + 8 + attrLength)) + 3), 4) == 0)
            return (u2) METHODBASE(classId, methodId) + 8 + 14 + attrLength;
     //+attrLength;		????
        attrLength = getU4(classId,METHODBASE(classId, methodId) + 8) + 6;
     } // < 64K
    return 0;
}
