package com.iceq;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import net.sf.rej.java.ClassFile;
import net.sf.rej.java.constantpool.ConstantPool;

public class KClassFileInfo 
{
	private ClassFile 	m_classFile;
	private String 	  	m_fullClassName;
	private String 	 	m_filePath;
	private boolean 	m_export;

	public KClassFileInfo(){
		m_export = true;
	}
	public KClassFileInfo(ClassFile classFile, String filePath) {
		m_filePath = filePath;
		m_export = true;
		setClassFile(classFile);
	}
	
	public void serialize(final DataOutputStream dos) throws IOException{
		final String classPath = m_filePath;
		dos.writeUTF(classPath);
		dos.writeBoolean(m_export);
		dos.writeUTF("\n");
	}
	
	public boolean deserialize(final DataInputStream fos,int version) throws IOException{
		String filePath = fos.readUTF();//fos.readLine();
		if(filePath != null && filePath.contains(".class")){
			System.out.println("FilePath:"+filePath);
			m_filePath = filePath;
			m_export = fos.readBoolean();
			fos.readUTF();
			return true;
		}
		return false;
	}
	public ClassFile getClassFile() {
		return m_classFile;
	}
	public String getFullClassName() {
		return m_fullClassName;
	}
	public String getFilePath() {
		return m_filePath;
	}
	public boolean isExport() {
		return m_export;
	}
	
	public void setClassFile(ClassFile classFile) {
		this.m_classFile = classFile;
		this.m_fullClassName = classFile.getFullClassName();
	}
	
	public void setClassFullName(String classFullName) {
		this.m_fullClassName = classFullName;
	}
	
	public void setFilePath(String filePath) {
		this.m_filePath = filePath;
	}
	
	public void setExport(boolean export) {
		this.m_export = export;
	}
	
	public void printClassPool(){
		ConstantPool cp = m_classFile.getPool();
		
		for(int i = 0;i < cp.size();i++){
			if(cp.get(i) != null)
				System.out.println(cp.get(i).toString());
			else
				System.out.println("cp["+i+"] is null");
		}
	}
}