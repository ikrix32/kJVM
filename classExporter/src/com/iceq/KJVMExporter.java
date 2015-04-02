package com.iceq;

import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.util.Vector;

import net.sf.rej.java.ClassFile;
import net.sf.rej.java.Descriptor;
import net.sf.rej.java.Disassembler;
import net.sf.rej.java.constantpool.ClassInfo;
import net.sf.rej.java.constantpool.ConstantPool;
import net.sf.rej.java.constantpool.ConstantPoolInfo;
import net.sf.rej.java.constantpool.UTF8Info;

public class KJVMExporter extends KJVMPackageHandler {
	String destFolder;
	Vector<KClass> m_microKernelClasses = new Vector<KClass>();
	Vector<KClass> m_applicationClasses = new Vector<KClass>();

	public KJVMExporter() {
	}

	public Vector<KClass> getMicroKernelClassFiles() {
		return m_microKernelClasses;
	}
	
	public Vector<KClass> getProjectClassFiles() {
		return m_applicationClasses;
	}

	public void load(File file,KClass kclass, boolean microkernelFile) {
		if (file.isDirectory()) {
			File[] files = file.listFiles();
			for (int i = 0; i < files.length; i++) {
				load(files[i], null,microkernelFile);
			}
		} else {
			if (file.getName().endsWith(".class")) {
				FileInputStream fis;
				byte[] data = null;
				try {
					fis = new FileInputStream(file);
					data = new byte[fis.available()];
					fis.read(data);
					ClassFile classFile = Disassembler.readClass(data);
					if(kclass != null)
						kclass.setClass(classFile);
					else
						kclass = new KClass(classFile,file.getAbsolutePath());
					if (microkernelFile)
						m_microKernelClasses.add(kclass);
					else
						m_applicationClasses.add(kclass);
					System.out.println("Loaded "
							+ (microkernelFile ? "MicroKernel " : "")
							+ "Class:" + classFile.getFullClassName());
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}

	public void exportMicroKernel(File folder) {
		DbgClassPackageInfo cPK = new DbgClassPackageInfo(m_microKernelClasses, null);

		try {
			String microkernelFilePath = folder.getAbsolutePath() + "/microkernel.h";
			System.out.println("Write microkernel header:" + microkernelFilePath);
			
			File microkernelFile = new File(microkernelFilePath);
			File microkernelBinaries = new File(folder.getAbsolutePath() +"/binaries");
			if(!microkernelBinaries.exists())
				microkernelBinaries.mkdirs();
			
			BufferedWriter fos = new BufferedWriter(new FileWriter(microkernelFile));
			
			fos.write("#ifdef USE_MICROKERNEL\n\n");
			String binariesDef = "static const u1* microkernelBinaries[] =\n{";
			String binariesSizeDef = "static const u4 microKernelBinariesSize[] =\n{";
			String classNames = "static const char* microkernelClassNames[] =\n{";
			for (int i = 0; i < m_microKernelClasses.size(); i++) 
			{
				KClass kclass = m_microKernelClasses.get(i);
				if(kclass.m_export)
				{
					String className = kclass.m_name.replace(".", "_");
					
					binariesDef += "\t"+className+"Bin,\n";
					binariesSizeDef += "\tsizeof("+className+"Bin),\n";
					classNames += "\t\""+className.replace("_", "/")+"\",\n";
					
					fos.write("static const u1 "+className+"Bin[] =\n");
					fos.write("#ifdef ENABLE_KCLASS_FORMAT\n");
					fos.write("#include \""+className+"_k.h\"\n");
					fos.write("#else\n");
					fos.write("#include \""+className+".h\"\n");
					fos.write("#endif\n\n");
					
					ClassFile classFile = kclass.m_class;
					exportClassFile(microkernelBinaries.getAbsolutePath() + "/" + className +".h", classFile);
					replaceClassNameIdWithClassId(cPK, classFile);
					exportClassFile(microkernelBinaries.getAbsolutePath() + "/" + className + "_k.h", classFile);
				}
			}
			binariesDef += "};\n\n";
			binariesSizeDef+= "};\n\n";
			classNames += "};\n\n";
			fos.write(binariesDef);
			fos.write(binariesSizeDef);
			fos.write("#ifdef DEBUG_KCLASS\n");
			fos.write(classNames);
			fos.write(	"extern const char* getMicroKernelClassName(const u2 classId){\n"+
						"\treturn microkernelClassNames[classId];\n}\n");
			fos.write("#endif\n\n");
			
			fos.write("#ifdef ENABLE_KCLASS_FORMAT\n");
			int classId = -1;
			try{
				classId = cPK.getClassId("java.lang.Object");
			}catch(Exception ex){}
			fos.write("extern const u2 JAVA_LANG_OBJECT_CLASS_ID(){ return "+(classId == -1 ? "INVALID_CLASS_ID" : classId)+"; }\n");
			
			classId = -1;
			try{
				classId = cPK.getClassId("java.lang.String");
			}catch(Exception ex){}
			fos.write("extern const u2 JAVA_LANG_STRING_CLASS_ID(){ return "+(classId == -1 ? "INVALID_CLASS_ID" : classId)+"; }\n");
			fos.write("#endif\n\n");
			fos.write(	"extern const int getNoMicroKernelClasses(){\n" +
						"\treturn sizeof(microKernelBinariesSize) / sizeof(microKernelBinariesSize[0]);\n}\n\n"+
						"extern const u1* getMicroKernelBinary(const int i){\n"+
						"\treturn microkernelBinaries[i];\n}\n\n"+
						"extern const u4 getMicroKernelBinarySize(const int i){\n"+
						"\treturn microKernelBinariesSize[i];\n}\n\n");
			
			String exceptions[] = new String[]{
					"java.lang.ArrayIndexOutOfBoundsException",
					"java.lang.NegativeArraySizeException",
					"java.lang.NullPointerException",
					"java.lang.ArithmeticException",
					"java.lang.ClassCastException",
					"java.lang.IllegalMonitorStateException",
			};
			//write exception table
			fos.write("#ifdef ENABLE_KCLASS_FORMAT\n");
			fos.write("static const u2 microkernelExceptions[] =\n{");
				
			for(int i = 0; i < exceptions.length;i++){
				classId = -1;
				try{
					classId = cPK.getClassId(exceptions[i]);
				}catch(Exception ex){}
				if(classId == -1)
					fos.write("\tINVALID_CLASS_ID,//"+exceptions[i]+"\n");
				else
					fos.write("\t"+classId+",//"+exceptions[i]+"\n");
			}
			fos.write("};\n\n");
			fos.write("extern u2 getMicroKernelExceptionClassId(const int exception){\n"+
						"\treturn microkernelExceptions[exception];\n}\n");
			fos.write("#endif\n\n");
			fos.write("#endif\n");
			fos.flush();
			fos.close();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	public void exportClasses(File folder) {
		DbgClassPackageInfo cPK = new DbgClassPackageInfo(m_microKernelClasses, m_applicationClasses);

		try {
			String packageFile = folder.getAbsolutePath() + "/KVMTestPackage.h";
			System.out.println("Write Application header:" + packageFile);
			BufferedWriter fos = new BufferedWriter(new FileWriter(new File(packageFile)));
			////////////////////////////
			fos.write("#ifdef ENABLE_TESTS\n\n");
			String binariesDef = "static const u1* testBinaries[] =\n{";
			String binariesSizeDef = "static const u4 testBinariesSize[] =\n{";
			String classNames = "static const char* testNames[] =\n{";
			for (int i = 0; i < m_applicationClasses.size(); i++) 
			{
				KClass kclass = m_applicationClasses.get(i);
				if(kclass.m_export)
				{
					String className = kclass.m_name.replace(".", "_");
					
					binariesDef += "\t"+className+"Bin,\n";
					binariesSizeDef += "\tsizeof("+className+"Bin),\n";
					classNames += "\t\""+className.replace("_", "/")+"\",\n";
					
					fos.write("static const u1 "+className+"Bin[] =\n");
					fos.write("#ifdef ENABLE_KCLASS_FORMAT\n");
					fos.write("#include \""+className+"_k.h\"\n");
					fos.write("#else\n");
					fos.write("#include \""+className+".h\"\n");
					fos.write("#endif\n\n");
					
					ClassFile classFile = kclass.m_class;
					exportClassFile(folder + "/" + className +".h", classFile);
					replaceClassNameIdWithClassId(cPK, classFile);
					exportClassFile(folder + "/" + className + "_k.h", classFile);
				}
			}
			binariesDef += "};\n\n";
			binariesSizeDef+= "};\n\n";
			classNames += "};\n\n";
			fos.write(binariesDef);
			fos.write(binariesSizeDef);
			fos.write(classNames);
			fos.write("#endif\n");
			fos.flush();
			fos.close();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	private void replaceClassNameIdWithClassId(DbgClassPackageInfo pk,
			ClassFile cf) throws ClassNotFoundException {
		final ConstantPool cp = cf.getPool();

		for (int i = 0; i < cp.size(); i++) {
			final ConstantPoolInfo cpi = cp.get(i);
			if (cpi != null) {
				if (cpi.getType() == ConstantPoolInfo.CLASS) {
					final ClassInfo ci = (ClassInfo) cpi;
					final int nameIndex = ci.getNameIndex();
					final UTF8Info utf8Name = (UTF8Info)cp.get(nameIndex);
					final String className = utf8Name.getValue().replace("/", ".");
					
					// arrays descriptors are stored as ClassReference
					if (!className.contains("["))
					{	//UTF8 name can be removed
						//cp.removeLast(nameIndex);
						ci.forceType(ConstantPoolInfo.KCLASS);
						ci.setNameIndex(pk.getClassId(className));
					} else{//array of objects
						if(className.contains("L"))
						{
							int index1 = className.indexOf("L");
							int index2 = className.indexOf(";");
							if(index1 >= 0 && index2 >= 0)
							{
								final String objClassName = className.substring(index1 + 1, index2 - index1 + 1);
								final int objClassId = pk.getClassId(objClassName);
								final String nClassName = className.replace(objClassName, ""+objClassId); 
								utf8Name.setString(nClassName);
								System.out.println("New Array descriptor:"+utf8Name.getValue());
							}
						}
						System.out.println("ClassName:" + className);
					}
				}
				/*
				 * if (cpi.getType() == ConstantPoolInfo.METHOD_REF) { final
				 * ClassInfo ci = (ClassInfo) cpi; final String className =
				 * ci.getValue(); if(!className.contains("["))//arrays
				 * descriptors are stored as ClassReference {
				 * ci.forceType(ConstantPoolInfo.KCLASS);
				 * ci.setNameIndex(pk.getClassId(className)); }else
				 * System.out.println("ClassName:"+className); }
				 */
			}
		}
	}

	public void exportClassFile(String filePath, ClassFile classFile) {
		try {
			System.out.println("Export file:" + filePath);

			BufferedWriter fos = new BufferedWriter(new FileWriter(new File(
					filePath)));

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
	
	public void savePackage(File file,boolean microkernel){
		String packageFile = file.getAbsolutePath();
		if(microkernel){
			if (!packageFile.endsWith(".kmicrok"))
				packageFile = packageFile + ".kmicrok";
		}else{
			if (!packageFile.endsWith(".kproj"))
				packageFile = packageFile + ".kproj";
		}
		System.out.println("Save "+(microkernel ? "MicroKernel " : "") + packageFile);
		
		savePackage(new File(packageFile), microkernel ? m_microKernelClasses : m_applicationClasses);
	}
}
