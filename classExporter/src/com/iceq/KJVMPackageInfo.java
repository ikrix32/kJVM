package com.iceq;

import java.util.Vector;

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
	
	public KJVMClassInfo getClassInfo(final int classId)
	{
		return m_classes.get(classId);
	}
	
	public KJVMClassInfo getClassInfo(final String classFullName) throws ClassNotFoundException
	{
		return getClassInfo(getClassId(classFullName));
	}
	
}
