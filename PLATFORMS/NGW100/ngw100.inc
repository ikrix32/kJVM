#
# HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
# See the file "license.terms" for information on usage and redistribution of this file.
#

# java for 32 Bit uC AVR32AP7000 at NGW100 
#=================================#
# working without os (standalone application) -> see goal avr32-linux for linux application
# make all ngw100 -> generates bajvm.bin, avr32bootpack and  JAVATESTS/NGW100.class
# and burns bajos

ARCH		= ap
# Part: {none|ap7xxx|uc3xxxxx}
PART		= ap7000


PLATFORM	= NGW100  -D BOARD=NGW100
LDFLAGS		= -nostartfiles
LDFLAGS+	= -march=$(ARCH) -mpart=$(PART)

#BINDIR =
CC		= $(BINDIR)avr32-gcc
OBJCOPY		= $(BINDIR)avr32-objcopy
UTILDIR		= $(BINDIR)
PROGRAM		= $(UTILDIR)avr32program

MCPU		= ap7000
CC_FLAGS	= -Wall -c  -mcpu=$(MCPU) -O$(OPT)
# -Werror -g


# Optimization level, can be [0, 1, 2, 3, s]. 
OPT		= 2
# Set your target processor

# Link: create ELF output file from object files

# JAVA SOURCES
# add java system sources to basic sources in Makefile
JAVABOOTSOURCES	+= 	$(LANG)/Math.java \
			$(LANG)/Boolean.java $(LANG)/Byte.java \
			$(UTIL)/Random.java \
			$(LANG)/Error.java \
			$(LANG)/ArithmeticException.java $(LANG)/ClassCastException.java \
			$(LANG)/ArrayIndexOutOfBoundsException.java \
			$(LANG)/RuntimeException.java $(LANG)/InterruptedException.java \
			$(UTIL)/Collection.java $(UTIL)/List.java \
			$(UTIL)/Stack.java $(UTIL)/Vector.java \
			$(LANG)/Runtime.java
			
JAVAAPPSOURCES		= $(APPCLASSPATH)/NGW100.java



PLATFORMCSOURCES	= $(TARGETHWFOLDER)/atmel/pio.c $(TARGETHWFOLDER)/atmel/intc.c \
		      $(TARGETHWFOLDER)/atmel/hsdramc.c $(TARGETHWFOLDER)/atmel/pm_at32ap7000.c \
		      $(TARGETHWFOLDER)/atmel/usart.c \
		      $(TARGETHWFOLDER)/platform.c  $(TARGETHWFOLDER)/native.c

PLATFORMASMSOURCES	 = $(TARGETHWFOLDER)/atmel/exception.S $(TARGETHWFOLDER)/atmel/crt0.S


OBJFILES		+=	$(PLATFORMASMSOURCES:.S=.o)


# GOALS
ngw100: 
	@:

all:	clean compile  bootclasses compNGW100 burnbajos


$(TARGETFILE): 	$(OBJFILES)
	@echo $(MSG_LINKING)
	$(CC)  $(filter %.o,$+) $(LDFLAGS) -o $(TARGETFILE)
	@echo
	@echo $(MSG_BINARY_IMAGE)
	$(OBJCOPY) -O binary $(TARGETFILE)  $(TARGETFILE).bin
	@echo

# Compile: create object files from C source files.
%.o: %.c	
	@echo $(MSG_COMPILING)
	$(CC)  $(CC_FLAGS) $(DEBUGGEN) -D$(PLATFORM) -DAVR32AP7000 -o $@ $<
	@echo

%.o: %.S
	@echo $(MSG_COMPILING)
	$(CC)  $(CC_FLAGS) $(DEBUGGEN) -D$(PLATFORM) -DAVR32AP7000 -o $@ $<
	@echo


.PHONY: burnbajos
burnbajos:	
	$(VERBOSE_CMD) $(PROGRAM)   halt
	sleep 3
	$(VERBOSE_CMD) $(PROGRAM)   program  -e -v -f0,8Mb  $(TARGETFILE).bin
	@printf %4d `echo $(BOOTCLASSES)| wc -w` > avr32bootpack
	@for i in $(BOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> avr32bootpack;	cat $$i >> avr32bootpack;	done
	sleep 2
	$(VERBOSE_CMD) $(PROGRAM)  program -F bin -O 0x40000  -f@0x00040000,512Kb  -e -v -R avr32bootpack
	@rm avr32bootpack

# java app
compNGW100:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(JAVAAPPSOURCES)
