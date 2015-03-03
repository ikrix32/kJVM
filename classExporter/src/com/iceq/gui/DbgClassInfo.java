package com.iceq.gui;

import net.sf.rej.java.ClassFile;

public class DbgClassInfo {
	public String name;
	public String fieldNames[];
	public String methodNames[];
	
	private DbgClassPackageInfo m_package;
	
	public DbgClassInfo(DbgClassPackageInfo pack,ClassFile classFile) {
		m_package = pack;
		name = classFile.getFullClassName();
	}
}
