#
# HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
# See the file "license.terms" for information on usage and redistribution of this file.
#

# java for 32 Bit uC AT32UC3A3256 at EVK1104
#=================================#
# working without monitor (standalone application)
# make all evk1104 -> generates bajvm.bin, avr32bootpack and  javatests/EVK1104.class
#
# working with monitor bamo32  (cs.ba-berlin.de) -> first load bamo32 at 0x80000000
# make all evk1104 withmon -> generates bajvm.bin, avr32bootpack and javatests/EVK1104.class
# load with monitor: u-command bajvm to 0x80002000 and avr32bootpack to 0x80003800
# run with monitor: g 0x80002000 command bajos

#BINDIR = 
UTILDIR= $(BINDIR)
CC		= $(BINDIR)avr32-gcc
OBJCOPY		= $(BINDIR)avr32-objcopy
PROGRAM		= $(UTILDIR)avr32program 

# CPU architecture: {ap|ucr1|ucr2}
ARCH = ucr2
# Part: {none|ap7xxx|uc3xxxxx}
PART = uc3a3256

# Flash memories: [{cfi|internal}@address,size]...
FLASH		= internal@0x80000000,256Kb
# Clock source to use when programming: [{xtal|extclk|int}]
PROG_CLOCK	= xtal
DEFS		= -D BOARD=EVK1104
# Options to request or suppress warnings: [-fsyntax-only] [-pedantic[-errors]] [-w] [-Wwarning...]
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
WARNINGS	= -Wall
# Options for debugging: [-g]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.

# Options that control optimization: [-O[0|1|2|3|s]]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
OPTIMIZATION	=  -ffunction-sections -fdata-sections
#O3, O1
CPPFLAGS	= -march=$(ARCH)  -DEVK1104 -DAVR32UC3A -mpart=$(PART) $(WARNINGS) $(DEFS) \
		$(PLATFORM_INC_PATH:%=-I%) $(INC_PATH:%=-I%) $(CPP_EXTRA_FLAGS)
CFLAGS		= $(DEBUGGEN) $(OPTIMIZATION) $(C_EXTRA_FLAGS) \
		$(PLATFORM_INC_PATH:%=-Wa,-I%)  $(AS_EXTRA_FLAGS)
ASFLAGS		= $(PLATFORM_INC_PATH:%=-Wa,-I%) $(AS_EXTRA_FLAGS)
LDFLAGS		= -march=$(ARCH) -mpart=$(PART) \
		$(LIB_PATH:%=-L%) $(LINKER_SCRIPT:%=-T%) $(LD_EXTRA_FLAGS)
LOADLIBES	= -lc
LDLIBS		= $(LIBS:%=-l%)

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
			$(LANG)/Runtime.java \
			$(JPLATFORM)/Display.java \
			$(JPLATFORM)/Touch.java \
			$(JPLATFORM)/LEDs.java \
			$(JPLATFORM)/ADC.java
			
JAVAAPPSOURCES		= JAVATESTS/EVK1104.java JAVATESTS/EVK1104a.java JAVATESTS/EVK1104b.java

PLATFORMCSOURCES	= $(TARGETHWFOLDER)/platform.c $(TARGETHWFOLDER)/native.c $(TARGETHWFOLDER)/atmel/et024006dhu.c $(TARGETHWFOLDER)/atmel/delay.c $(TARGETHWFOLDER)/atmel/smc.c $(TARGETHWFOLDER)/atmel/qt60168.c $(TARGETHWFOLDER)/atmel/led.c
PLATFORMASMSOURCES	= $(TARGETHWFOLDER)/atmel/crt0.S

OBJFILES		+=	$(PLATFORMASMSOURCES:.S=.o)


# GOALS
CPPFLAGS	+=-MD -MP -MT
ifeq ($(findstring withmon,$(MAKECMDGOALS)),withmon)

DEFS		+= -D WITHMON
LINKER_SCRIPT 	= $(TARGETHWFOLDER)/link_uc3a3256withmon.lds
# Extra flags to use when linking
#LD_EXTRA_FLAGS = -Wl,--gc-sections -nostdlib 
#-Wl,-e,_trampoline
LD_EXTRA_FLAGS	= -Wl,--gc-sections  -nostartfiles

all:	clean compile bootclasses compEVK1104 bootpack
	@echo generating bajvm.bin avr32bootpack JAVATESTS/EVK1104.class

burnbajvm: burn run

burn:
	$(PROGRAM)  -pjtagicemkii  --part $(PART) program -finternal@0x80000000 -cint -F bin -O 0x80020000  -v -e -R bajvm.bin


else # without monitor
PLATFORMCSOURCES	+= $(TARGETHWFOLDER)/atmel/intc.c $(TARGETHWFOLDER)/atmel/pm.c \
			$(TARGETHWFOLDER)/atmel/rtc.c \
			$(TARGETHWFOLDER)/atmel/spi.c $(TARGETHWFOLDER)/atmel/isp.c \
			$(TARGETHWFOLDER)/atmel/adc.c $(TARGETHWFOLDER)/atmel/pm_conf_clocks.c \
			$(TARGETHWFOLDER)/atmel/flashc.c $(TARGETHWFOLDER)/atmel/sdramc.c \
			$(TARGETHWFOLDER)/atmel/gpio.c $(TARGETHWFOLDER)/atmel/usart.c \
			$(TARGETHWFOLDER)/atmel/power_clocks_lib.c

PLATFORMASMSOURCES	+= $(TARGETHWFOLDER)/atmel/exception.S   $(TARGETHWFOLDER)/atmel/trampoline.S

# $(TARGETHWFOLDER)/atmel/pwm.c $(TARGETHWFOLDER)/atmel/dip204.c 
# Extra flags to use when linking
# LD_EXTRA_FLAGS = -Wl,--gc-sections -nostdlib  -Wl,-e,_trampoline
LD_EXTRA_FLAGS	= -Wl,--gc-sections -Wl,-e,_trampoline  -nostartfiles
LINKER_SCRIPT	= $(TARGETHWFOLDER)/link_uc3a3256.lds

all:	clean compile bootclasses compEVK1104 burnbootpack burnbajvm run

# Program MCU memory from ELF output file.
.PHONY: burnbajvm

burnbajvm:
	$(PROGRAM)  -pjtagicemkii  --part $(PART) program -finternal@0x80000000 -cint -F bin -O 0x80000000  -v -e -R bajvm.bin


endif	#monitor

withmon:	
	@:

evk1104:
	@:


$(TARGETFILE): $(OBJFILES)
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)$(CC) $(LDFLAGS) $(filter %.o,$+) $(LOADLIBES) $(LDLIBS) -o $(TARGETFILE)
	@echo $(MSG_BINARY_IMAGE)
	$(OBJCOPY) -O binary $(TARGETFILE)  $(TARGETFILE).bin
	$(VERBOSE_NL)


# Preprocess, compile & assemble: create object files from C source files.
%.o: %.c
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD)$(CC) -c   $(CPPFLAGS)  $(CFLAGS) -o $@ $<
	@touch $@
	$(VERBOSE_NL)

# Preprocess & assemble: create object files from assembler source files.
%.o: %.S
	@echo $(MSG_ASSEMBLING) 
	$(VERBOSE_CMD)$(CC) -c $(CPPFLAGS) -MD -MP -MT '$*.x' $(ASFLAGS) -o $@ $<
	@touch $*.d
	@touch $@
	$(VERBOSE_NL)



# Preprocess & assemble: create object files from assembler source files.
#%.o: %.S %.asm
#	@echo $(MSG_ASSEMBLING)
#	$(VERBOSE_CMD)$(CC) -c $(CPPFLAGS) $(ASFLAGS) -o $@ $<
#	@touch $@
#	$(VERBOSE_NL)

#with and without monitor
bootpack:
	$(VERBOSE_CMD) printf %4d `echo $(JAVABOOTCLASSES)| wc -w` > avr32bootpack
	$(VERBOSE_CMD) for i in $(JAVABOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> avr32bootpack;	cat $$i >> avr32bootpack;	done
	@echo generating avr32bootpack



burnbootpack:	bootpack
	$(PROGRAM)  -pjtagicemkii  --part UC3A3256 program -finternal@0x80000000 -cint -F bin -O 0x80038000  -v -e -R avr32bootpack


# java app
compEVK1104:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(JAVAAPPSOURCES)

