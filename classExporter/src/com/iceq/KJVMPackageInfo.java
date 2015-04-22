package com.iceq;

import java.util.Vector;

import net.sf.rej.java.constantpool.ClassInfo;
import net.sf.rej.java.constantpool.ConstantPoolInfo;

public class KJVMPackageInfo
{
	Vector<KJVMClassInfo> m_classes = new Vector<KJVMClassInfo>();
	
	public KJVMPackageInfo(Vector<KClassFileInfo> microkernelClasses,Vector<KClassFileInfo> classes)
	{
		for(int i = 0; i < microkernelClasses.size();i++)
		{
			if(microkernelClasses.get(i).isExport())
				m_classes.add(new KJVMClassInfo(this,microkernelClasses.get(i)));
		}
		
		if(classes !=  null){
			for(int i = 0; i < classes.size();i++)
			{
				if(classes.get(i).isExport())
					m_classes.add(new KJVMClassInfo(this,classes.get(i)));
			}
		}
	}
	
	public int getClassId(String classFullName) throws ClassNotFoundException
	{
		for(int i = 0;i < m_classes.size();i++)
		{
			if(m_classes.get(i).m_classFileInfo.getFullClassName().equals(classFullName))
					return i;
		}
		throw new ClassNotFoundException("Class " + classFullName + " not found.");
	}
	
	public KJVMClassInfo getKClassInfo(final int classId)
	{
		return m_classes.get(classId);
	}
	
	public KJVMClassInfo getKClassInfo(final String classFullName) throws ClassNotFoundException
	{
		return getKClassInfo(getClassId(classFullName));
	}
	
	public KJVMClassInfo getKClassInfo(final ClassInfo classInfo) throws ClassNotFoundException
	{
		if(classInfo.getType() == ConstantPoolInfo.KCLASS)
			return getKClassInfo(classInfo.getNameIndex());
		else
			return getKClassInfo(classInfo.getName());
	}
}
