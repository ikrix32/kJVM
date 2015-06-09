package com.iceq;

import java.util.HashMap;
import java.util.Hashtable;
import java.util.Map;

import proguard.classfile.ClassConstants;
import proguard.classfile.util.ClassUtil;
import proguard.obfuscate.MappingProcessor;

/**
 * This interface specifies methods to process name mappings between original
 * classes and their obfuscated versions. The mappings are typically read
 * from a mapping file.
 */
public class KJVMMapping implements MappingProcessor
{
	protected class ClassMapping{
		private String className;
		private HashMap<String, HashMap<String,String>> methods = new HashMap<String, HashMap<String,String>>();
	
		public ClassMapping(String className)
		{
			this.className = className;
		}
		
		public String getNewClassName(){
			return className;
		}
		
		public void addMethod(String methodName,String description,String newMethodName){
			if(methods.containsKey(methodName)){
				final HashMap<String, String> methodDef = methods.get(methodName);
				if(!methodDef.containsKey(description)){
					methodDef.put(description, newMethodName);
				}
			}else{
				final HashMap<String, String> methodDefinitions = new HashMap<String,String>();
				methodDefinitions.put(description, newMethodName);
				methods.put(methodName, methodDefinitions);
			}
		}
		
		public String getNewMethodName(String oldMethodName,String description){
			final HashMap<String, String> methodDefinitions = methods.get(oldMethodName);
			if(methodDefinitions == null)
				return null;
			
			return methodDefinitions.get(description);
		}
	}
	
	private Hashtable<String, ClassMapping> m_classMapping = new Hashtable<String, ClassMapping>();
	
	public String getNewClassName(String oldClassname){
		return m_classMapping.get(oldClassname).getNewClassName();
	}
	
	public String getNewMethodName(String oldClassName,String oldMethodName,String description)
	{
		return m_classMapping.get(oldClassName).getNewMethodName(oldMethodName, description);
	}
    /**
     * Processes the given class name mapping.
     *
     * @param className    the original class name.
     * @param newClassName the new class name.
     * @return whether the processor is interested in receiving mappings of the
     *         class members of this class.
     */
    public boolean processClassMapping(String className,String newClassName)
    {
    	if(!m_classMapping.containsKey(className)){
    		ClassMapping cMapping = new ClassMapping(newClassName);
    		m_classMapping.put(className, cMapping);
    		System.out.println("Added class map "+className+"->"+newClassName+".");
    	}
    	return true;
    }

    /**
     * Processes the given field name mapping.
     * @param className    the original class name.
     * @param fieldType    the original external field type.
     * @param fieldName    the original field name.
     * @param newClassName the new class name.
     * @param newFieldName the new field name.
     */
    public void processFieldMapping(String className,
                                    String fieldType,
                                    String fieldName,
                                    String newClassName,
                                    String newFieldName)
    {
    	
    }

    /**
     * Processes the given method name mapping.
     * @param className          the original class name.
     * @param firstLineNumber    the first line number of the method, or 0 if
     *                           it is not known.
     * @param lastLineNumber     the last line number of the method, or 0 if
     *                           it is not known.
     * @param methodReturnType   the original external method return type.
     * @param methodName         the original external method name.
     * @param methodArguments    the original external method arguments.
     * @param newClassName       the new class name.
     * @param newFirstLineNumber the new first line number of the method, or 0
     *                           if it is not known.
     * @param newLastLineNumber  the new last line number of the method, or 0
     *                           if it is not known.
     * @param newMethodName      the new method name.
     */
    public void processMethodMapping(String className,
                                     int    firstLineNumber,
                                     int    lastLineNumber,
                                     String methodReturnType,
                                     String methodName,
                                     String methodArguments,
                                     String newClassName,
                                     int    newFirstLineNumber,
                                     int    newLastLineNumber,
                                     String newMethodName)
    {
    	if(!m_classMapping.containsKey(className)){
    		System.out.println("Class "+className+" not found in mappings.");
    	}else{
    		ClassMapping cMapping = m_classMapping.get(className);
    		
    		String internalMethodArguments = ClassUtil.internalMethodDescriptor(methodReturnType,ClassConstants.METHOD_ARGUMENTS_OPEN + methodArguments + ClassConstants.METHOD_ARGUMENTS_CLOSE);
    		
    		cMapping.addMethod(methodName, internalMethodArguments, newMethodName);
    		
    		System.out.println("Added method "+className+"."+methodName+"("+methodArguments+") => "+cMapping.getNewClassName()+"."+newMethodName+internalMethodArguments);
    	}
    }
}
