#
# HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
# See the file "license.terms" for information on usage and redistribution of this file.
#

# java for 8 Bit uC test environment for linux
#=================================#
# make all bootclasses (bootpack)
# TOOLS, SOURCES AND FLAGS
ifeq  ($(TARGETHW), linux)
CC		= gcc
CPPFLAGS	= -DLINUX -DDEBUGOPSTACK -DDEBUGMETHODSTACK
all:	clean compile bootclasses
else
CC		= avr32-linux-gcc
CPPFLAGS	= -DAVR32LINUX
all:	clean compile bootclasses bootpack

bootpack:
	@echo -n generate file: avr32bootpack\; numClasses: 
	@printf %4d `echo $(JAVABOOTCLASSES)| wc -w` |tee  avr32bootpack
	@for i in $(JAVABOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> avr32bootpack;	cat $$i >> avr32bootpack;	done
	@echo "  length:" `ls -l avr32bootpack | cut -f5 -d' '`

endif

OPTIMIZATION		= -O3

# JAVA SOURCES
JAVABOOTSOURCES		+= 	\
			$(LANG)/Boolean.java $(LANG)/Byte.java\
			$(UTIL)/Random.java \
			$(LANG)/Exception.java  $(LANG)/Error.java \
			$(LANG)/ArithmeticException.java $(LANG)/ClassCastException.java \
			$(LANG)/RuntimeException.java $(LANG)/InterruptedException.java \
			$(UTIL)/Collection.java $(UTIL)/List.java \
			$(UTIL)/Stack.java $(UTIL)/Vector.java \
			$(LANG)/Runtime.java 

# only for stk1000!!!
ifeq  ($(TARGETHW), avr32-linux)
JAVABOOTSOURCES+=	${GRAPHICS}/Display.java ${GRAPHICS}/DisplayHSB.java \
			${GRAPHICS}/DisplayZBuffer.java ${GRAPHICS}/DisplayKonstanten.java \
			${GRAPHICS}/Point.java ${GRAPHICS}/Font.java \
			${GRAPHICS}/Line.java ${GRAPHICS}/Polyline.java \
			${GRAPHICS}/AffineMatrix.java ${GRAPHICS}/ProjectionMatrix.java
endif

PLATFORMCSOURCES	= $(TARGETHWFOLDER)/platform.c $(TARGETHWFOLDER)/native.c

avr32-linux:
	@:

linux:	
	@:

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD) ${CC}  $(filter %.o,$+)   -o$(TARGETFILE) 



# Preprocess, compile & assemble: create object files from C source files.
%.o: %.c 
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) $(CC) -c $(CPPFLAGS) -Wall $(DEFS) ${DEBUGGEN} -o $@ $<
	@touch $@
	$(VERBOSE_NL)


# javac -g:none B.java		// no debug info (line number table)
# javap -c -l -verbose B


EEE:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/EEE.class

compEEE:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
	$(APPCLASSPATH)/EEE.java

FFF:
	./$(TARGETFILE)   $(JAVABOOTCLASSES) 

compFFF:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
	$(APPCLASSPATH)/FFF.java

GGG:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/GGG.class

compGGG:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
	$(APPCLASSPATH)/GGG.java

HHH:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) 	$(APPCLASSPATH)/HHH.class

compHHH:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
	$(APPCLASSPATH)/HHH.java

My:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/My.class $(APPCLASSPATH)/Ball.class

compMy:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES)  \
	$(APPCLASSPATH)/My.java

Temp:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/Temperature.class

compTemp:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
	$(APPCLASSPATH)/Temperature.java


T:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/T2.class $(APPCLASSPATH)/T1.class

compT:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
	  	$(APPCLASSPATH)/T1.java $(APPCLASSPATH)/T2.java
	
PC:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/Buffer.class \
		$(APPCLASSPATH)/ProducerConsumer.class 

compPC:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
		$(APPCLASSPATH)/ProducerConsumer.java

SchedulerTest:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) \
		$(APPCLASSPATH)/SchedulerTest.class 

compSchedulerTest:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
		$(APPCLASSPATH)/SchedulerTest.java

LockSimple:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) \
		$(APPCLASSPATH)/LockSimple.class 

compLockSimple:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
		$(APPCLASSPATH)/LockSimple.java

LockTest:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) \
		$(APPCLASSPATH)/LockTest.class $(APPCLASSPATH)/SpinLockTest.class 

compLockTest:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
		$(APPCLASSPATH)/LockTest.java $(APPCLASSPATH)/SpinLockTest.java

Interrupt:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) \
		$(APPCLASSPATH)/InterruptTest.class 

compInterrupt:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
		$(APPCLASSPATH)/InterruptTest.java

ADC:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/ADC.class

compADC:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
		$(APPCLASSPATH)/ADC.java

Interpreter:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/InterpreterTest.class $(APPCLASSPATH)/InpTest.class $(APPCLASSPATH)/ParamTest.class

compInterpreter:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
		$(APPCLASSPATH)/InterpreterTest.java


Breed:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/Breed.class

compBreed:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) \
		$(APPCLASSPATH)/Breed.java


Test:	 
	./$(TARGETFILE)   $(JAVABOOTCLASSES) $(APPCLASSPATH)/Test.class

compTest:
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES)  \
	$(APPCLASSPATH)/Test.java