CPATH    = ./TOOLS/CopyAndAppend/classes

JARGS    = -g:none -verbose -d $(CPATH)


all:
	javac  $(JARGS) ./TOOLS/CopyAndAppend/sources/*.java
	jar cfe ./TOOLS/CopyAndAppend/CopyAndAppend.jar  CopyAndAppend -C $(CPATH) .



clean:
	rm ./TOOLS/CopyAndAppend/classes/*.class
	rm ./TOOLS/CopyAndAppend/CopyAndAppend.jar

