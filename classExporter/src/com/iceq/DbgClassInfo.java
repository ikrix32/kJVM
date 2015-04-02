package com.iceq;


public class DbgClassInfo {
	public KClass m_class;
	public String m_fieldNames[];
	public String m_methodNames[];
	
	private DbgClassPackageInfo m_package;
	
	public DbgClassInfo(DbgClassPackageInfo pack,KClass classFile) {
		m_package = pack;
		m_class = classFile;
	}
}
