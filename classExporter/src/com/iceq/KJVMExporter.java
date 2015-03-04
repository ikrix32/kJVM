package com.iceq;

import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.util.Vector;

import com.iceq.gui.DbgClassInfo;
import com.iceq.gui.DbgClassPackageInfo;

import net.sf.rej.java.ClassFile;
import net.sf.rej.java.Disassembler;
import net.sf.rej.java.constantpool.ClassInfo;
import net.sf.rej.java.constantpool.ConstantPool;
import net.sf.rej.java.constantpool.ConstantPoolInfo;

public class KJVMExporter {

	String destFolder;
	Vector<ClassFile> m_classes = new Vector<ClassFile>();

	public KJVMExporter() {
	}

	public Vector<ClassFile> getClassFiles() {
		return m_classes;
	}

	public void load(File file) {
		if (file.isDirectory()) {
			File[] files = file.listFiles();
			for (int i = 0; i < files.length; i++) {
				load(files[i]);
			}
		} else {
			if(file.getName().endsWith(".class"))
			{
				FileInputStream fis;
				byte[] data = null;
				try {
					fis = new FileInputStream(file);
					data = new byte[fis.available()];
					fis.read(data);
					ClassFile classFile = Disassembler.readClass(data);
					m_classes.add(classFile);
					System.out.println("Loaded class:" + classFile.getFullClassName());
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}

	public void exportMicroKernel(File folder, Vector<ClassFile> classes) {
		m_classes = classes;
		DbgClassPackageInfo cPK = new DbgClassPackageInfo(classes);

		try {
			String mikrokernelFile = folder.getAbsolutePath() +"/mikrokernel_dbg.h";
			System.out.println("Write microkernel header:"+mikrokernelFile);
			BufferedWriter fos = new BufferedWriter(new FileWriter(new File(mikrokernelFile)));
			fos.write("{\n");
			for (int i = 0; i < m_classes.size(); i++) 
			{
				ClassFile classFile = m_classes.get(i);
				String className = classFile.getFullClassName();
				fos.write("\t\t\t\""+className+"\",\n ");
				replaceClassNameIdWithClassId(cPK, classFile);
				exportClassFile(folder+"/"+className.replace(".", "_")+"_k.h",classFile);
			}
			fos.write("};\n");
			fos.flush();
			fos.close();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	private void replaceClassNameIdWithClassId(DbgClassPackageInfo pk,
			ClassFile cf) throws ClassNotFoundException
	{
		final ConstantPool cp = cf.getPool();
		
		for (int i = 0; i < cp.size(); i++) 
		{
			final ConstantPoolInfo cpi = cp.get(i);
			if (cpi != null){
				if (cpi.getType() == ConstantPoolInfo.CLASS) 
				{
					final ClassInfo ci = (ClassInfo) cpi;
					final String className = ci.getValue();
					if(!className.contains("["))//arrays descriptors are stored as ClassReference
					{
						ci.forceType(ConstantPoolInfo.KCLASS);
						ci.setNameIndex(pk.getClassId(className));
					}else
						System.out.println("ClassName:"+className);
				}
				/*if (cpi.getType() == ConstantPoolInfo.METHOD_REF) 
				{
					final ClassInfo ci = (ClassInfo) cpi;
					final String className = ci.getValue();
					if(!className.contains("["))//arrays descriptors are stored as ClassReference
					{
						ci.forceType(ConstantPoolInfo.KCLASS);
						ci.setNameIndex(pk.getClassId(className));
					}else
						System.out.println("ClassName:"+className);
				}*/
			}
		}
	}

	public void exportClassFile(String filePath,ClassFile classFile) {
		try {
			System.out.println("Export file:" + filePath);

			BufferedWriter fos = new BufferedWriter(new FileWriter(new File(filePath)));

			byte[] data = classFile.getData();
			fos.write("{");
			fos.write(toHexFromBytes(data));
			fos.write("\n};\n");
			fos.flush();
			fos.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private final static String[] hexSymbols = { "0", "1", "2", "3", "4", "5",
			"6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };

	public final static int BITS_PER_HEX_DIGIT = 4;

	public static String toHexFromByte(final byte b) {
		byte leftSymbol = (byte) ((b >>> BITS_PER_HEX_DIGIT) & 0x0f);
		byte rightSymbol = (byte) (b & 0x0f);

		return (hexSymbols[leftSymbol] + hexSymbols[rightSymbol]);
	}

	public static String toHexFromBytes(final byte[] bytes) {
		if (bytes == null || bytes.length == 0) {
			return ("");
		}

		// there are 2 hex digits per byte
		StringBuilder hexBuffer = new StringBuilder(bytes.length * 2);

		// for each byte, convert it to hex and append it to the buffer
		for (int i = 0; i < bytes.length; i++) {
			if (i % 16 == 0)
				hexBuffer.append('\n');
			hexBuffer.append("0x" + toHexFromByte(bytes[i]) + ",");
		}

		return (hexBuffer.toString());
	}
}
