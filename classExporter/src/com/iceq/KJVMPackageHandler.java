package com.iceq;

import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.util.Vector;

public abstract class KJVMPackageHandler
{
	final static int VERSION = 1;
	public abstract void load(File file, KClassFileInfo kclass,boolean microkernelFile);
	
	public void savePackage(final File file,final Vector<KClassFileInfo> classes) 
	{
		try {
			DataOutputStream dos = new DataOutputStream(new FileOutputStream(file));
			dos.writeInt(VERSION);
			for (int i = 0; i < classes.size(); i++) {
				final KClassFileInfo kclass = classes.get(i);
				kclass.serialize(dos);
			}
			dos.flush();
			dos.close();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	public void loadPackage(File file,boolean microkernel) 
	{
		try {
			String packageFile = file.getAbsolutePath();
			
			if (file.isDirectory() || (microkernel && !packageFile.endsWith(".kmicrok"))
			|| (!microkernel && !packageFile.endsWith(".kproj")))
					return;

			System.out.println("Load "+(microkernel?"MicroKernel ":"") + packageFile);
			DataInputStream fos = new DataInputStream(new FileInputStream(new File(packageFile)));

			int dataVersion = fos.readInt();
			System.out.println("Data version:"+dataVersion);
			KClassFileInfo kclass = new KClassFileInfo();
			while (fos.available() > 0 && kclass.deserialize(fos,dataVersion)) {
				File crtfile = new File(kclass.getFilePath());
				load(crtfile,kclass, microkernel);
				kclass = new KClassFileInfo();
			}
			fos.close();
		} catch (Exception ex) {
			ex.printStackTrace();
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
