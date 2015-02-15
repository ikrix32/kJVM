CPATH    = ./JAVABOOTCLASSES/classes

JARGS    = -g:none -verbose -extdirs . -bootclasspath $(CPATH) -d $(CPATH) -sourcepath ./JAVABOOTCLASSES/sources


boot:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/io/*.java
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/lang/*.java
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/util/*.java
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/util/concurrent/*.java


graphic:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/graphics/*.java

display:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/display/*.java

all:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/platform/PlatForm.java
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/io/*.java
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/lang/*.java
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/graphics/*.java
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/util/*.java
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/java/util/concurrent/*.java


charon:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/platform/charon/PlatForm.java 

arduinomega:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/platform/arduinomega/PlatForm.java 

xplain:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/platform/xplain/PlatForm.java 


evk1100:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/platform/evk1100/PlatForm.java


evk1104:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/platform/evk1104/*.java

linux:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/platform/linux/PlatForm.java

avr32-linux:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/platform/avr32-linux/PlatForm.java

ngw100:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/platform/ngw100/PlatForm.java

stk1000:
	javac  $(JARGS) ./JAVABOOTCLASSES/sources/platform/stk1000/PlatForm.java

clean:
	rm ./JAVABOOTCLASSES/classes/platform/*.class
	rm ./JAVABOOTCLASSES/classes/java/io/*.class
	rm ./JAVABOOTCLASSES/classes/java/lang/*.class
	rm ./JAVABOOTCLASSES/classes/java/graphics/*.class
	rm ./JAVABOOTCLASSES/classes/java/util/*.class








boot1:
	javac  -verbose -g:none -classpath ./JAVABOOTCLASSES/classes ./JAVABOOTCLASSES/sources/java/io/*.java
	mv ./JAVABOOTCLASSES/sources/java/io/*.class  ./JAVABOOTCLASSES/classes/java/io
	javac  -verbose -classpath ./JAVABOOTCLASSES/classes -g:none ./JAVABOOTCLASSES/sources/java/lang/*.java -extdirs . -bootclasspath .
	mv ./JAVABOOTCLASSES/sources/java/lang/*.class  ./JAVABOOTCLASSES/classes/java/lang
	javac  -verbose -g:none  ./JAVABOOTCLASSES/sources/java/util/*.java -extdirs . -bootclasspath . -classpath ./JAVABOOTCLASSES/classes
	mv ./JAVABOOTCLASSES/sources/java/util/*.class  ./JAVABOOTCLASSES/classes/java/util

graphic1:
	javac  -verbose -classpath ./JAVABOOTCLASSES/classes -g:none ./JAVABOOTCLASSES/sources/java/graphics/*.java
	mv ./JAVABOOTCLASSES/sources/java/graphics/*.class  ./JAVABOOTCLASSES/classes/java/graphics/






