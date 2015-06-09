package com.iceq;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import proguard.ClassPath;
import proguard.ClassPathEntry;
import proguard.Configuration;
import proguard.classfile.ClassConstants;

public class KVMProguard
{
	/** Returns the ProGuard configuration.  */
	public static Configuration getProGuardConfiguration(File outputFolder,Vector<KClassFileInfo> microkernel,Vector<KClassFileInfo> application)
    {
        Configuration configuration = new Configuration();

        // Get the input and output jars and directories.
        configuration.programJars = getClassPath(application);
        //set outputFolder - should be temporary folder for microkernel
        configuration.programJars.add(new ClassPathEntry(outputFolder, true));
        
        configuration.libraryJars = getClassPath(microkernel);

        List keep = new ArrayList();

        // Collect the additional keep options.
        List additionalKeep = null;// TBD - investigate what ClassSpecification represents// additionalKeepPanel.getClassSpecifications();
        if (additionalKeep != null)
        {
            keep.addAll(additionalKeep);
        }

        // Collect the additional keep names options.
        List additionalKeepNames = null;// TBD - investigate what ClassSpecification represents// additionalKeepNamesPanel.getClassSpecifications();
        if (additionalKeepNames != null)
        {
            keep.addAll(additionalKeepNames);
        }

        // Collect the boilerplate keep options.
       /* TBD - investigate what ClassSpecification represents
        for (int index = 0; index < boilerplateKeep.length; index++)
        {
            if (boilerplateKeepCheckBoxes[index].isSelected())
            {
                keep.add(classSpecification(boilerplateKeep[index],
                                            boilerplateKeepTextFields[index].getText()));
            }
        }

        // Collect the boilerplate keep names options.
        for (int index = 0; index < boilerplateKeepNames.length; index++)
        {
            if (boilerplateKeepNamesCheckBoxes[index].isSelected())
            {
                keep.add(classSpecification(boilerplateKeepNames[index],
                                            boilerplateKeepNamesTextFields[index].getText()));
            }
        }*/

        // Put the list of keep specifications in the configuration.
        if (keep.size() > 0)
        {
            configuration.keep = keep;
        }


        // Collect the boilerplate "no side effect methods" options.
        List noSideEffectMethods = new ArrayList();

        /*for (int index = 0; index < boilerplateNoSideEffectMethods.length; index++)
        {
            if (boilerplateNoSideEffectMethodCheckBoxes[index].isSelected())
            {
                noSideEffectMethods.add(boilerplateNoSideEffectMethods[index]);
            }
        }

        // Collect the additional "no side effect methods" options.
        List additionalNoSideEffectOptions = additionalNoSideEffectsPanel.getClassSpecifications();
        if (additionalNoSideEffectOptions != null)
        {
            noSideEffectMethods.addAll(additionalNoSideEffectOptions);
        }

        // Put the list of "no side effect methods" options in the configuration.
        if (noSideEffectMethods.size() > 0)
        {
            configuration.assumeNoSideEffects = noSideEffectMethods;
        }


        // Collect the "why are you keeping" options.
        configuration.whyAreYouKeeping = whyAreYouKeepingPanel.getClassSpecifications();
		*/

        // Get the other options.
        configuration.shrink                           = true;//microkernel shuldn't be stripped
        configuration.printUsage                       = null;// new File(printUsageTextField.getText());

        configuration.optimize                         = true;
        configuration.allowAccessModification          = true;
        configuration.mergeInterfacesAggressively      = true;
        configuration.optimizations                    = null;//optimizationsTextField.getText().length() > 1 ?         ListUtil.commaSeparatedList(optimizationsTextField                   .getText()) : null;
        configuration.optimizationPasses               = 1;//((SpinnerNumberModel)optimizationPassesSpinner.getModel()).getNumber().intValue();

        configuration.obfuscate                        = true;
        configuration.printMapping                     = null;// new File(printMappingTextField.getText());
        configuration.applyMapping                     = null;// new File(applyMappingTextField.getText());
        configuration.obfuscationDictionary            = null;// new File(obfuscationDictionaryTextField.getText()) : null;
        configuration.classObfuscationDictionary       = null;//new File(classObfuscationDictionaryTextField.getText()) : null;
        configuration.packageObfuscationDictionary     = null;//new File(packageObfuscationDictionaryTextField.getText()) : null;
        configuration.overloadAggressively             = true;//overloadAggressivelyCheckBox.isSelected();
        configuration.useUniqueClassMemberNames        = false;//useUniqueClassMemberNamesCheckBox.isSelected();
        configuration.useMixedCaseClassNames           = true;//useMixedCaseClassNamesCheckBox          .isSelected();
        configuration.keepPackageNames                 = null;//keepPackageNamesTextField.getText().length()  > 0 ? ListUtil.commaSeparatedList(ClassUtil.internalClassName(keepPackageNamesTextField.getText()))  : new ArrayList() : null;
        configuration.flattenPackageHierarchy          = null;//flattenPackageHierarchyCheckBox         .isSelected() ? ClassUtil.internalClassName(flattenPackageHierarchyTextField         .getText()) : null;
        configuration.repackageClasses                 = null;//repackageClassesCheckBox                .isSelected() ? ClassUtil.internalClassName(repackageClassesTextField                .getText()) : null;
        configuration.keepAttributes                   = null;//keepAttributesCheckBox                  .isSelected() ? ListUtil.commaSeparatedList(keepAttributesTextField                  .getText()) : null;
        configuration.keepParameterNames               = false;//keepParameterNamesCheckBox              .isSelected();
        configuration.newSourceFileAttribute           = null;//newSourceFileAttributeCheckBox.isSelected() ? newSourceFileAttributeTextField.getText()  : null;
        configuration.adaptClassStrings                = null;//adaptClassStringsCheckBox               .isSelected() ? adaptClassStringsTextField.getText().length() > 0 ? ListUtil.commaSeparatedList(ClassUtil.internalClassName(adaptClassStringsTextField.getText())) : new ArrayList() : null;
        configuration.adaptResourceFileNames           = null;//adaptResourceFileNamesCheckBox          .isSelected() ? ListUtil.commaSeparatedList(adaptResourceFileNamesTextField          .getText()) : null;
        configuration.adaptResourceFileContents        = null;//adaptResourceFileContentsCheckBox       .isSelected() ? ListUtil.commaSeparatedList(adaptResourceFileContentsTextField       .getText()) : null;

        configuration.preverify                        = true;//preverifyCheckBox                       .isSelected();
        configuration.microEdition                     = false;//microEditionCheckBox                    .isSelected();
        configuration.targetClassVersion               = ClassConstants.CLASS_VERSION_1_8;//targetCheckBox                          .isSelected() ? ClassUtil.internalClassVersion(targetComboBox.getSelectedItem().toString()) : 0;

        configuration.verbose                          = false;//verboseCheckBox.isSelected();
        configuration.note                             = null; //noteCheckBox                            .isSelected() ? noteTextField.getText().length() > 0 ? ListUtil.commaSeparatedList(ClassUtil.internalClassName(noteTextField.getText())) : null : new ArrayList();
        configuration.warn                             = null; //warnCheckBox                            .isSelected() ? warnTextField.getText().length() > 0 ? ListUtil.commaSeparatedList(ClassUtil.internalClassName(warnTextField.getText())) : null : new ArrayList();
        configuration.ignoreWarnings                   = false;//ignoreWarningsCheckBox                  .isSelected();
        configuration.skipNonPublicLibraryClasses      = false;//skipNonPublicLibraryClassesCheckBox     .isSelected();
        configuration.skipNonPublicLibraryClassMembers = false;//skipNonPublicLibraryClassMembersCheckBox.isSelected();
        configuration.keepDirectories                  = null; //keepDirectoriesCheckBox                 .isSelected() ? keepDirectoriesTextField.getText().length() > 0 ? ListUtil.commaSeparatedList(ClassUtil.internalClassName(keepDirectoriesTextField.getText())) : new ArrayList() : null;
        configuration.lastModified                     = Long.MAX_VALUE;//forceProcessingCheckBox                 .isSelected() ? Long.MAX_VALUE : System.currentTimeMillis();
        configuration.printSeeds                       = null;//printSeedsCheckBox                      .isSelected() ? new File(printSeedsTextField                                         .getText()) : null;
        configuration.printConfiguration               = Configuration.STD_OUT;//printConfigurationCheckBox              .isSelected() ? new File(printConfigurationTextField                                 .getText()) : null;
        configuration.dump                             = null;// dumpCheckBox                            .isSelected() ? new File(dumpTextField                                               .getText()) : null;

        return configuration;
    }

	/** Returns the ClassPath currently represented in this panel. */
	private static ClassPath getClassPath( Vector<KClassFileInfo> classes)
	{
		int size = classes.size();
		if (size == 0)
		{
			return null;
		}

		ClassPath classPath = new ClassPath();
		for (int index = 0; index < size; index++)
		{
			final KClassFileInfo cfi = classes.get(index);
			if(cfi.isExport())
			{
				final File file = new File(cfi.getFilePath());
				final ClassPathEntry entry = new ClassPathEntry(file, false);//isOutput); 
				classPath.add(entry);
			}
		}

		return classPath;
	}
}
