package com.iceq.gui;

import java.util.Vector;

import net.sf.rej.java.ClassFile;

public class DbgClassPackageInfo {
	Vector<DbgClassInfo> m_classes = new Vector<DbgClassInfo>();
	
	public DbgClassPackageInfo(Vector<ClassFile> classes) {
		for(int i = 0; i < classes.size();i++){
			m_classes.add(new DbgClassInfo(this,classes.get(i)));
		}
	}
	
	public int getClassId(String classFullName) throws ClassNotFoundException{
		for(int i = 0; i < m_classes.size();i++){
			final String className = m_classes.get(i).name;
			if(className.equals(classFullName))
				return i;
		}
		throw new ClassNotFoundException("Class "+classFullName+" not found.");
	}
}
