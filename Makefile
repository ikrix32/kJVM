# Hey -  this is a -*- makefile -*- -> for BAJOS
# HWR/Fachbereich Berufsakademie (BA) - Java Operating System for Microcontroller
# atmega1280 (arduinoMega), atmega128 (CharonII), linux, avr32UCA (EVK1100ES), avr32AP7000(NGW100,STK1000)

# goals xxx
# charon ch arduinoMega am linux avr32-linux evk1100 ngw100 stk1000 clean java (for tests)
# make clean goal
# make compile goal
# make debug compile verbose goal
# make all goal
# make all withmon goal
# ...........
# make clobber goal

# xxx cpuinfo
# xxx cpuinfo
# xxx halt
# xxx reset
# xxx run
# xxx readregs
# xxx chiperase
# xxx program

# verbose
# clean

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# FUNCTIONS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
LastWord  = $(if $(1),$(word $(words $(1)) ,$(1)))
FirstWord = $(if $(1),$(word 1,$(1)))

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# CHECK  ECLIPSE BUILD
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **

ifneq ($(TARGETHW),)#variable is not set by environment

ifneq  "" "$(filter CH ch charon,$(TARGETHW))"
TARGETHW=charon
endif
ifneq  "" "$(filter AM am arduinoMega,$(TARGETHW))"
TARGETHW=arduinomega
endif

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
#  DEBUG
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
DEBUGGEN	= $(if $(filter Debug ,$(BUILDCONFIGURATION)), -DDEBUG,)

MAKECMDGOALS += $(if $(filter yes ,$(TINYBAJOS)), TINYBAJOS,)

else

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# CHECK COMMAND LINE
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# test command line - make it better
$(if   $(filter CH charon ch arduinoMega am AM linux avr32-linux evk1100 evk1104 stk1000 ngw100 xplain,$(MAKECMDGOALS))\
, ,$(error wrong or incomplete command line -> not valid target platform specified))

# WHATS TARGETHARDWARE
TARGETHW = noTarget
ifneq "1"  "$(words $(filter charon ch CH arduinoMega am  AM evk1100 evk1104 ngw100 stk1000 linux avr32-linux xplain,$(MAKECMDGOALS)))"
$(error only one target hardware accepted)
else
TARGETHW = $(filter charon ch arduionoMega AM evk1100 evk1104 ngw100 stk1000 linux avr32-linux java xplain,$(MAKECMDGOALS))
endif
ifneq  "" "$(filter CH ch charon,$(MAKECMDGOALS))"
TARGETHW=charon
endif
ifneq  "" "$(filter AM am arduinoMega,$(MAKECMDGOALS))"
TARGETHW=arduinomega
endif

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
#  DEBUG
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
DEBUGGEN	= $(if $(filter debug ,$(MAKECMDGOALS)), -DDEBUG,)

endif

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# JAVA-SOURCES AND TARGETS
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# java paths
JVMBASE		= JVM/
CLASSPATH	= $(APPPATH)
APPCLASSPATH	= $(CLASSPATH)JAVATESTS
BOOTCLASSPATH	= ${CLASSPATH}JAVABOOTCLASSES/classes/
LANG		= ${BOOTCLASSPATH}java/lang
IO		= ${BOOTCLASSPATH}java/io
UTIL		= ${BOOTCLASSPATH}java/util
JPLATFORM	= ${BOOTCLASSPATH}platform
GRAPHICS	= ${BOOTCLASSPATH}java/graphics
CONCURRENT	= ${BOOTCLASSPATH}java/util/concurrent

#java binaries and flags
JAVACOMP		= javac
JAVACOMPFLAGS		= -g:none 

#-source 1.4 -verbose
JAVACOMPBOOTCLASSES	= -bootclasspath ${BOOTCLASSPATH} -classpath JAVABOOTCLASSES/classes -extdirs .

# COMMON SOURCES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# BAJOS C-SOURCES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
BAJOSCSOURCES	= $(JVMBASE)bajvm.c $(JVMBASE)classfile.c $(JVMBASE)interpreter.c $(JVMBASE)heap.c $(JVMBASE)stack.c $(JVMBASE)scheduler.c \
		$(JVMBASE)nativedispatch.c \
		$(APPPATH)JAVALANGNATIVE/float.c \
		$(APPPATH)JAVALANGNATIVE/object.c \
		$(APPPATH)JAVALANGNATIVE/string.c \


# java system sources -> subset of java system sources for the small controller, add more sources later
JAVABOOTSOURCES =	$(JPLATFORM)/PlatForm.java $(LANG)/Throwable.java\
			$(LANG)/String.java $(LANG)/StringBuffer.java $(LANG)/StringBuilder.java \
			$(LANG)/Integer.java $(LANG)/Float.java \
			$(LANG)/Object.java $(LANG)/System.java \
			$(IO)/OutStream.java $(IO)/InStream.java \
			$(LANG)/Math.java
#			$(LANG)/ArrayIndexOutOfBoundsException.java

#############
# TINYBAJOS #
#############
#Smaller version of bajos with reduced feature set
ifeq ($(findstring TINYBAJOS,$(MAKECMDGOALS)),TINYBAJOS)
	#Shrink bajos, but looses features 
	DEFS			+= -DTINYBAJOS #general flag
	DEFS			+= -DTINYBAJOS_ERROREXIT # errorexit does only show error number no text
	DEFS			+= -DTINYBAJOS_PRINTF # remove unnessary printfs
	DEFS			+= -DTINYBAJOS_MULTITASKING # remove multitasking feature
	DEFS			+= -DTINYBAJOS_OTHER # remove different maybe unnessary functions
	DEFS			+= -DTINYBAJOS_EXCEPTION # remove exception handling
else
	JAVABOOTSOURCES 	+= 	$(LANG)/Thread.java $(LANG)/Exception.java

#JAVABOOTSOURCES		+= 	$(CONCURRENT)/Lock.java $(CONCURRENT)/InterruptThread.java
	BAJOSCSOURCES		+= 	$(APPPATH)JAVALANGNATIVE/lock.c \
					$(APPPATH)JAVALANGNATIVE/thread.c \
					$(APPPATH)JAVALANGNATIVE/interruptthread.c
endif

JAVABOOTCLASSES	= $(JAVABOOTSOURCES:.java=.class)			
JAVAAPPCLASSES 	= $(JAVAAPPSOURCES:.java=.class)


OBJFILES	= $(BAJOSCSOURCES:.c=.o) $(PLATFORMCSOURCES:.c=.o)


# APP PATH
APPPATH 	= ./

TARGETFILE	= $(basename $(call FirstWord,$(BAJOSCSOURCES)))

#$(PART)-$(TARGET)
LSS		= $(TARGETFILE:$(TGTTYPE)=.lss)
SYM		= $(TARGETFILE:$(TGTTYPE)=.sym)
HEX		= $(TARGETFILE:$(TGTTYPE)=.hex)
BIN		= $(TARGETFILE:$(TGTTYPE)=.bin)

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# BINUTILS-BINARIES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
RM		= rm -Rf
SLEEP		= sleep
SLEEPUSB	= 3
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **

# Display main executed commands.
ifneq ($(findstring verbose,$(MAKECMDGOALS)),verbose)
# Prefix displaying the following command if and only if verbose is a goal.
VERBOSE_CMD	= @
# New line displayed if and only if verbose is a goal.
VERBOSE_NL	=
else
VERBOSE_CMD	= 
VERBOSE_NL	= @echo
verbose:
	@:
endif

# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# PLATFORM SPECIFIC  MAKE RULES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
TARGETHWFOLDER =$(APPPATH)PLATFORMS/$(shell echo $(TARGETHW) | tr '[:lower:]' '[:upper:]')

ifeq  ($(TARGETHW), avr32-linux)
include PLATFORMS/LINUX/linux.inc
else
include $(TARGETHWFOLDER)/$(TARGETHW).inc
endif
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# COMMON MAKE RULES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
compile: $(TARGETFILE)


# Clean up the project.
.PHONY: clobber

clobber: clean
	-$(VERBOSE_CMD)$(RM) *bootpack amapp bajos.bin
	-$(VERBOSE_CMD)$(RM) `find . -name *.class -print`

.PHONY: clean
clean: copyandappend 
	@echo $(MSG_CLEANING)
	-$(VERBOSE_CMD)$(RM) $(BIN)
	-$(VERBOSE_CMD)$(RM) $(HEX)
	-$(VERBOSE_CMD)$(RM) $(SYM)
	-$(VERBOSE_CMD)$(RM) $(LSS)
	-$(VERBOSE_CMD)$(RM) $(TARGETFILE)
	-$(VERBOSE_CMD)$(RM) $(OBJFILES)
	-$(VERBOSE_CMD)$(RM)  *.o
	-$(VERBOSE_CMD)$(RM)  *.d
	-$(VERBOSE_CMD)$(RM)  */*.o
	-$(VERBOSE_CMD)$(RM)  */*.d
	-$(VERBOSE_CMD)$(RM)  */*.lst
	-$(VERBOSE_CMD)$(RM)  */*.c~
	-$(VERBOSE_CMD)$(RM)  */*.S~
	-$(VERBOSE_CMD)$(RM)  */*.h~
	-$(VERBOSE_CMD)$(RM)  */*.hex
	$(VERBOSE_NL)
	
# Rebuild the project.
.PHONY: rebuild
rebuild: clean $(TARGETFILE)

# Display CC version information.
.PHONY: ccversion
ccversion:
	@echo
	@echo
	@$(CC) --version

# Create extended listing from target output file.
.PHONY: lss
lss: $(LSS)

# Display target size information.
.PHONY: sizes
sizes: $(TARGETFILE)
	@echo
	@echo
ifeq ($(TGTTYPE),.a)
	@$(SIZE) -Bxt $<
else
ifeq ($(TGTTYPE),)
	-$(VERBOSE_CMD) $(SIZE) -Ax $<
	-$(VERBOSE_CMD) $(SIZE) -Bx $<
endif
endif

# Get CPU information.
.PHONY: cpuinfo
cpuinfo:
	@echo
	@echo $(MSG_GETTING_CPU_INFO)
	$(VERBOSE_CMD)$(PROGRAM) cpuinfo
ifneq ($(call LastWord,$(filter cpuinfo chiperase erase program secureflash reset debug run readregs,$(MAKECMDGOALS))),cpuinfo)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif

# Perform a JTAG Chip Erase command.
.PHONY: chiperase
chiperase:
	@echo
	@echo $(MSG_ERASING_CHIP)
	$(VERBOSE_CMD) $(PROGRAM) chiperase
ifneq ($(call LastWord,$(filter cpuinfo chiperase program secureflash reset debug run readregs,$(MAKECMDGOALS))),chiperase)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif

# Perform a flash chip erase.
.PHONY: erase
erase:
ifeq (  $(filter chiperase program,$(MAKECMDGOALS)),  )
	@echo
	@echo $(MSG_ERASING)
	$(VERBOSE_CMD)$(PROGRAM) erase $(FLASH:%=-f%)
ifneq ($(call LastWord,$(filter cpuinfo erase secureflash reset debug run readregs,$(MAKECMDGOALS))),erase)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif
else
	@:
endif

# Reset MCU.
.PHONY: reset
reset:
ifeq ($(filter program run,$(MAKECMDGOALS)),)
	@echo
	@echo $(MSG_RESETTING)
	$(VERBOSE_CMD)$(PROGRAM) reset
ifneq ($(call LastWord,$(filter cpuinfo chiperase erase secureflash reset debug readregs,$(MAKECMDGOALS))),reset)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif
else
	@:
endif

# Start CPU execution.
.PHONY: run
run:
ifeq ($(findstring program,$(MAKECMDGOALS)),)
	@echo
	@echo $(MSG_RUNNING)
	$(VERBOSE_CMD)$(PROGRAM) run $(if $(findstring reset,$(MAKECMDGOALS)),-R)
ifneq ($(call LastWord,$(filter cpuinfo chiperase erase secureflash debug run readregs,$(MAKECMDGOALS))),run)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif
else
	@:
endif

# Read CPU registers.
.PHONY: readregs
readregs:
	@echo
	@echo $(MSG_READING_CPU_REGS)
	$(VERBOSE_CMD)$(PROGRAM) readregs
ifneq ($(call LastWord,$(filter cpuinfo chiperase erase program secureflash reset debug run readregs,$(MAKECMDGOALS))),readregs)
	@$(SLEEP) $(SLEEPUSB)
else
	@echo
endif

# Create extended listing from target output file.
$(LSS): $(TARGETFILE)
	@echo $(MSG_EXTENDED_LISTING)
	$(VERBOSE_CMD)$(OBJDUMP) -h -S $< > $@
	$(VERBOSE_NL)

ifeq ($(TGTTYPE),.elf)
# Create binary image from ELF output file.
$(BIN): $(TARGETFILE)
	@echo $(MSG_BINARY_IMAGE)
	$(VERBOSE_CMD)$(OBJCOPY) -O binary $< $(TARGETFILE)
	$(VERBOSE_NL)
endif

debug:
	@:
TINYBAJOS:
	@:

bootclasses:
ifeq ($(findstring clean,$(MAKECMDGOALS)),clean)
	make -f ./JAVABOOTCLASSES/makefile clean
else
	make -f ./JAVABOOTCLASSES/makefile $(TARGETHW)
	make -f ./JAVABOOTCLASSES/makefile boot
endif
	
copyandappend:
ifeq ($(findstring clean,$(MAKECMDGOALS)),clean)
	make -f ./TOOLS/CopyAndAppend/makefile clean
else
	make -f ./TOOLS/CopyAndAppend/makefile
endif


# Copyright (c) 2007, Atmel Corporation All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/
# or other materials provided with the distribution.
#
# 3. The name of ATMEL may not be used to endorse or promote products derived
# from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
# SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
# MESSAGES
# ** ** ** *** ** ** ** ** ** ** ** ** ** ** **
ERR_TARGET_TYPE       = Target type not supported: `$(TGTTYPE)'
MSG_CLEANING          = Cleaning project.
MSG_PREPROCESSING     = Preprocessing \`$<\' to \`$@\'.
MSG_COMPILING         = Compiling \`$<\' to \`$@\'.
MSG_ASSEMBLING        = Assembling \`$<\' to \`$@\'.
MSG_ARCHIVING         = Archiving to \`$@\'.
MSG_LINKING           = Linking to \`$(TARGETFILE)\'.
MSG_EXTENDED_LISTING  = Creating extended listing to \`$@\'.
MSG_SYMBOL_TABLE      = Creating symbol table to \`$@\'.
MSG_IHEX_IMAGE        = Creating Intel HEX image to \`$@\'.
MSG_BINARY_IMAGE      = Creating binary image to \`$(TARGETFILE).bin\'.
MSG_GETTING_CPU_INFO  = Getting CPU information.
MSG_HALTING           = Stopping CPU execution.
MSG_ERASING_CHIP      = Performing a JTAG Chip Erase command.
MSG_ERASING           = Performing a flash chip erase.
MSG_PROGRAMMING       = Programming MCU memory from \`$(TARGETFILE)\'.
MSG_SECURING_FLASH    = Protecting chip by setting security bit.
MSG_RESETTING         = Resetting MCU.
MSG_DEBUGGING         = Opening debug connection with MCU.
MSG_RUNNING           = Starting CPU execution.
MSG_READING_CPU_REGS  = Reading CPU registers.
MSG_CLEANING_DOC      = Cleaning documentation.
MSG_GENERATING_DOC    = Generating documentation to \`$(DOC_PATH)\'.
