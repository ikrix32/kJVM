package com.iceq;

import java.util.Vector;

public class DbgClassPackageInfo {
	Vector<DbgClassInfo> m_classes = new Vector<DbgClassInfo>();
	
	public DbgClassPackageInfo(Vector<KClass> microkernelClasses,Vector<KClass> classes) {
		for(int i = 0; i < microkernelClasses.size();i++)
		{
			if(microkernelClasses.get(i).m_export)
				m_classes.add(new DbgClassInfo(this,microkernelClasses.get(i)));
		}
		
		if(classes !=  null){
			for(int i = 0; i < classes.size();i++)
			{
				if(classes.get(i).m_export)
					m_classes.add(new DbgClassInfo(this,classes.get(i)));
			}
		}
	}
	
	public int getClassId(String classFullName) throws ClassNotFoundException{
		for(int i = 0;i < m_classes.size();i++)
		{
			if(m_classes.get(i).m_class.m_name.equals(classFullName))
					return i;
		}
		throw new ClassNotFoundException("Class "+classFullName+" not found.");
	}
}
