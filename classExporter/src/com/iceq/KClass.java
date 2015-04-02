package com.iceq;

import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import net.sf.rej.java.ClassFile;

public class KClass {
	public String m_name;
	public ClassFile m_class;
	public String m_filePath;
	public boolean m_export;

	public KClass(){
		m_export = true;
	}
	public KClass(ClassFile classFile, String filePath) {
		m_filePath = filePath;
		m_export = true;
		setClass(classFile);
	}
	
	public void setClass(ClassFile classFile){
		m_class = classFile;
		m_name = classFile.getFullClassName();
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
}