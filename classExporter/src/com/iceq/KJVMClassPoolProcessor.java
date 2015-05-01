package com.iceq;

import com.iceq.KJVMClassInfo.KRef;

import net.sf.rej.java.ClassFile;
import net.sf.rej.java.Field;
import net.sf.rej.java.Method;
import net.sf.rej.java.constantpool.ClassInfo;
import net.sf.rej.java.constantpool.ConstantPool;
import net.sf.rej.java.constantpool.ConstantPoolInfo;
import net.sf.rej.java.constantpool.NameAndTypeInfo;
import net.sf.rej.java.constantpool.RefInfo;
import net.sf.rej.java.constantpool.UTF8Info;

public class KJVMClassPoolProcessor
{
	public void processClassFile( KJVMPackageInfo pk, ClassFile cf) throws ClassNotFoundException
	{
		final ConstantPool cp = cf.getPool();
		for (int i = 0; i < cp.size(); i++)
		{
			final ConstantPoolInfo cpi = cp.get(i);
			if (cpi != null)
			{
				switch (cpi.getType())
				{
					case ConstantPoolInfo.CLASS:
					{
						convertClassInfoToKClassInfo(cp,(ClassInfo) cpi,pk);
					}
					break;
					case ConstantPoolInfo.INTERFACE_METHOD_REF:	
					case ConstantPoolInfo.METHOD_REF:
					{
						convertMethodRefToKMethodRef(cp, (RefInfo) cpi, pk);
					}
					break;
					case ConstantPoolInfo.FIELD_REF:
					{
						//convertFieldRefToKFieldRef(cp, (RefInfo) cpi, pk);
					}
					break;
					default:
					{
						// System.out.println("Unprocessed Constant Pool element of type:"+cpi.getType());
					}
				}
			}
		}
	}

	private void convertClassInfoToKClassInfo( final ConstantPool cp, final ClassInfo classInfo, final KJVMPackageInfo pk)
			throws ClassNotFoundException
	{
		final int nameIndex = classInfo.getNameIndex();
		final UTF8Info utf8Name = (UTF8Info) cp.get(nameIndex);
		final String className = utf8Name.getValue().replace("/", ".");

		// arrays descriptors are stored as ClassReference
		if (!className.contains("["))
		{ // UTF8 name can be removed
			// cp.removeLast(nameIndex);
			classInfo.forceType(ConstantPoolInfo.KCLASS);
			classInfo.setNameIndex(pk.getClassId(className));
		} else
		{// array of objects
			if (className.contains("L"))
			{
				int index1 = className.indexOf("L");
				int index2 = className.indexOf(";");
				if (index1 >= 0 && index2 >= 0)
				{
					final String objClassName = className.substring(index1 + 1, index2 - index1 + 1);
					final int objClassId = pk.getClassId(objClassName);
					final String nClassName = className.replace(objClassName, "" + objClassId);
					utf8Name.setString(nClassName);
					System.out.println("New Array descriptor:" + utf8Name.getValue());
				}
			}
			System.out.println("Class Name:" + className);
		}
	}

	private void convertMethodRefToKMethodRef( final ConstantPool cp, final RefInfo methodRefInfo, final KJVMPackageInfo pk)
			throws ClassNotFoundException
	{
		final ClassInfo classInfo = methodRefInfo.getClassInfo();
		KJVMClassInfo kClassInfo = pk.getKClassInfo(classInfo);

		NameAndTypeInfo nameAndType = methodRefInfo.getNameAndTypeInfo();
		if(kClassInfo.m_classFileInfo.getFullClassName().equals("java.lang.StringBuilder"))
		{
			System.out.println("");
		}
		KRef methodRef = kClassInfo.getMethodRef(nameAndType.getName(), nameAndType.getDescriptorString());

		if(true){
			KJVMClassInfo origClass = kClassInfo;
			kClassInfo = pk.getKClassInfo(methodRef.getClassId());
			Method meth = kClassInfo.getMethod(methodRef.getItemId());
			String s1 = classInfo.getName() + "." + nameAndType.getName() + " desc:" + nameAndType.getDescriptorString();
			String s2 = kClassInfo.m_classFileInfo.getFullClassName() + "." + meth.getName() + " desc:"
					+ meth.getDescriptor().getRawDesc();
			if(!s1.equals(s2)){
				System.out.println("Orig class:"+origClass.m_classFileInfo.getFullClassName());
				System.out.println("Orig. Method: " + classInfo.getName() + "." + nameAndType.getName() + " desc:" + nameAndType.getDescriptorString());
				System.out.println("New Method: " + kClassInfo.m_classFileInfo.getFullClassName() + "." + meth.getName() + " desc:"
					+ meth.getDescriptor().getRawDesc());
			}
		}
	
		//proccess
		methodRefInfo.forceType(methodRefInfo.getType() == ConstantPoolInfo.METHOD_REF ? ConstantPoolInfo.KMEHOD_REF : ConstantPoolInfo.KINTERFACE_METHOD_REF);
		methodRefInfo.setClassIndex(methodRef.getClassId());
		if(nameAndType.getType() == ConstantPoolInfo.KNAME_AND_TYPE
		&& methodRef.getItemId() != methodRefInfo.getNameAndTypeIndex())
		{
			nameAndType = new NameAndTypeInfo(nameAndType.getOriginalNameIndex(), nameAndType.getDescriptorIndex(),cp);
			System.out.println("Add name for method ref:"+nameAndType.toString());
			int index = cp.forceAdd(nameAndType);
			methodRefInfo.setNameAndTypeIndex(index);
			
		}
		nameAndType.forceType(ConstantPoolInfo.KNAME_AND_TYPE);
		nameAndType.setNameIndex(methodRef.getItemId());
	}

	private void convertFieldRefToKFieldRef( final ConstantPool cp, final RefInfo fieldRefInfo, final KJVMPackageInfo pk)
			throws ClassNotFoundException
	{
		final ClassInfo classInfo = fieldRefInfo.getClassInfo();
		KJVMClassInfo kClassInfo = pk.getKClassInfo(classInfo);

		NameAndTypeInfo nameAndType = fieldRefInfo.getNameAndTypeInfo();
		KRef fieldRef = kClassInfo.getFieldRef(nameAndType.getName(), nameAndType.getDescriptorString());

		if(false)
		{
			kClassInfo = pk.getKClassInfo(fieldRef.getClassId());
			Field meth = kClassInfo.getField(fieldRef.getItemId());
			
			System.out.println("Orig Field: " + classInfo.getName() + "." + nameAndType.getName() + " desc:" + nameAndType.getDescriptorString());
			System.out.println("New Field: " + kClassInfo.m_classFileInfo.getFullClassName() + "." + meth.getName() + " desc:"
					+ meth.getDescriptor().getRawDesc());
		}
		//proccess
		fieldRefInfo.forceType(ConstantPoolInfo.KFIELD_REF);
		fieldRefInfo.setClassIndex(fieldRef.getClassId());
		if(nameAndType.getType() == ConstantPoolInfo.KNAME_AND_TYPE
		&& fieldRef.getItemId() != fieldRefInfo.getNameAndTypeIndex())
		{
			nameAndType = new NameAndTypeInfo(nameAndType.getOriginalNameIndex(), nameAndType.getDescriptorIndex(),cp);
			System.out.println("Add name for field ref:"+nameAndType.toString());
			int index = cp.forceAdd(nameAndType);
			fieldRefInfo.setNameAndTypeIndex(index);	
		}
		nameAndType.forceType(ConstantPoolInfo.KNAME_AND_TYPE);
		nameAndType.setNameIndex(fieldRef.getItemId());
	}
}
