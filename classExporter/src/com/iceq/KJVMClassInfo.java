package com.iceq;

import java.util.List;

import net.sf.rej.java.ClassFile;
import net.sf.rej.java.Field;
import net.sf.rej.java.Method;
import net.sf.rej.java.constantpool.ClassInfo;
import net.sf.rej.java.constantpool.ConstantPoolInfo;

public class KJVMClassInfo
{
	protected class KRef
	{
		private int	m_classId;
		private int	m_itemId;

		public KRef(int classId, int itemId)
		{
			m_classId = classId;
			m_itemId = itemId;
		}

		public int getClassId()
		{
			return m_classId;
		}

		public int getItemId()
		{
			return m_itemId;
		}
	}

	private KJVMPackageInfo	m_package;

	public KClassFileInfo	m_classFileInfo;

	public KJVMClassInfo(KJVMPackageInfo pack, KClassFileInfo classFile)
	{
		m_package = pack;
		m_classFileInfo = classFile;
	}

	public KRef getFieldRef( final String fieldName, final String type) throws ClassNotFoundException
	{
		ClassFile crtClassFile = m_classFileInfo.getClassFile();

		int classId = m_package.getClassId(m_classFileInfo.getFullClassName());
		int fieldId = getFieldId(fieldName, type);

		while (fieldId < 0 && crtClassFile.getSuperClass() != 0)
		{
			
			final ClassInfo superClassInfo = (ClassInfo)crtClassFile.getPool().get(crtClassFile.getSuperClass());
			final KJVMClassInfo superClass = m_package.getKClassInfo(superClassInfo);

			fieldId = superClass.getFieldId(fieldName, type);

			if (fieldId >= 0)
				classId = m_package.getClassId(superClass.m_classFileInfo.getFullClassName());

			crtClassFile = superClass.m_classFileInfo.getClassFile();
		}
		if (fieldId >= 0)
			return new KRef(classId, fieldId);

		return null;
	}

	public KRef getMethodRef( final String methodName, final String type) throws ClassNotFoundException
	{
		ClassFile crtClassFile = m_classFileInfo.getClassFile();

		int classId = m_package.getClassId(m_classFileInfo.getFullClassName());
		int methodId = getMethodId(methodName, type);

		while (methodId < 0 && crtClassFile.getSuperClassName() != null)
		{
			final ClassInfo superClassInfo = (ClassInfo)crtClassFile.getPool().get(crtClassFile.getSuperClass());
			final KJVMClassInfo superClass = m_package.getKClassInfo(superClassInfo);

			methodId = superClass.getMethodId(methodName, type);

			if (methodId >= 0)
				classId = m_package.getClassId(superClass.m_classFileInfo.getFullClassName());

			crtClassFile = superClass.m_classFileInfo.getClassFile();
		}
		if (methodId >= 0)
			return new KRef(classId, methodId);

		return null;
	}

	private int getFieldId( final String fieldName, final String type)
	{
		final ClassFile classFile = m_classFileInfo.getClassFile();
		final List<Field> fields = classFile.getFields();

		for (int i = 0; i < fields.size(); i++)
		{
			final Field field = fields.get(i);
			final String name = field.getName();
			final String description = field.getDescriptor().getRawDesc();
			if (name.equals(fieldName) && description.equals(type))
				return i;
		}
		return -1;
	}
	
	private int getMethodId( final String methodName, final String type)
	{
		final ClassFile classFile = m_classFileInfo.getClassFile();
		final List<Method> methods = classFile.getMethods();

		for (int i = 0; i < methods.size(); i++)
		{
			final Method method = methods.get(i);
			final String name = method.getName();
			final String description = method.getDescriptor().getRawDesc();

			if (name.equals(methodName) && description.equals(type))
				return i;
		}
		return -1;
	}

	public Field getField( int index)
	{
		final ClassFile classFile = m_classFileInfo.getClassFile();
		final List<Field> fields = classFile.getFields();


		return fields.get(index);
	}
	
	public Method getMethod( int index)
	{
		final ClassFile classFile = m_classFileInfo.getClassFile();
		final List<Method> methods = classFile.getMethods();

		return methods.get(index);
	}

}
