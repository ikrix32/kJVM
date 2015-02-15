#
# HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
# See the file "license.terms" for information on usage and redistribution of this file.
#

# java for 32 Bit uC AVR32AP7000 at NGW100 
#=================================#
# working without os (standalone application) -> see goal avr32-linux for linux application
# make all stk1000 -> generates bajvm.bin, avr32bootpack and  JAVATESTS/STK1000.class
# and burns bajos

ARCH		= ap
# Part: {none|ap7xxx|uc3xxxxx}
PART		= ap7000


PLATFORM	= STK1000  -D BOARD=STK1000
LDFLAGS		= -march=$(ARCH) -mpart=$(PART)

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




# JAVA SOURCES
# add java system sources to basic sources in Makefile
JAVABOOTSOURCES	+= 	\
			$(LANG)/Boolean.java $(LANG)/Byte.java \
			$(UTIL)/Random.java \
			$(LANG)/Error.java \
			$(LANG)/ArithmeticException.java $(LANG)/ClassCastException.java \
			$(LANG)/ArrayIndexOutOfBoundsException.java \
			$(LANG)/RuntimeException.java $(LANG)/InterruptedException.java \
			$(UTIL)/Collection.java $(UTIL)/List.java \
			$(UTIL)/Stack.java $(UTIL)/Vector.java \
			$(LANG)/Runtime.java

JAVABOOTSOURCES+=	${GRAPHICS}/Display.java ${GRAPHICS}/DisplayHSB.java \
			${GRAPHICS}/DisplayZBuffer.java ${GRAPHICS}/DisplayKonstanten.java \
			${GRAPHICS}/Point.java ${GRAPHICS}/Font.java \
			${GRAPHICS}/Line.java ${GRAPHICS}/Polyline.java \
			${GRAPHICS}/AffineMatrix.java ${GRAPHICS}/ProjectionMatrix.java			

JAVAAPPSOURCES		= $(APPCLASSPATH)/STK1000.java



PLATFORMCSOURCES = $(TARGETHWFOLDER)/atmel/pio.c $(TARGETHWFOLDER)/atmel/lcdc.c $(TARGETHWFOLDER)/atmel/usart.c\
		$(TARGETHWFOLDER)/lib2d.c $(TARGETHWFOLDER)/fontlib.c \
		$(TARGETHWFOLDER)/atmel/ltv350qv.c $(TARGETHWFOLDER)/atmel/at32stk1000.c \
		$(TARGETHWFOLDER)/atmel/pm.c $(TARGETHWFOLDER)/atmel/spi.c $(TARGETHWFOLDER)/atmel/intc.c \
		$(TARGETHWFOLDER)/atmel/utils.c $(TARGETHWFOLDER)/atmel/sdram.c \
		$(TARGETHWFOLDER)/atmel/bmplib.c $(TARGETHWFOLDER)/platform.c $(TARGETHWFOLDER)/native.c


OBJFILES		+=	$(PLATFORMASMSOURCES:.S=.o)


# GOALS
stk100: 
	@:

all:	clean compile  bootclasses bootgraphic  compSTK1000 burnbajos burnlogo

$(TARGETFILE): 	$(OBJFILES)
	@echo $(MSG_LINKING)
	$(CC)  $(filter %.o,$+) $(LDFLAGS) -o $(TARGETFILE)
	@echo
	@echo $(MSG_BINARY_IMAGE)
	$(OBJCOPY) -O binary $(TARGETFILE)  $(TARGETFILE).bin
	@echo

bootgraphic:
	make -f ./JAVABOOTCLASSES/makefile graphic

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

burnlogo:
	sleep 2
	$(VERBOSE_CMD) $(PROGRAM)  program -F bin -O 0x700000  -f@0x00700000,512Kb  -e -v -R STK1000/logo.bmp


# java app
compSTK1000:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(JAVAAPPSOURCES)
