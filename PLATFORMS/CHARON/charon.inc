#
# HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
# See the file "license.terms" for information on usage and redistribution of this file.
#
# java for 8 Bit uC atmega128 at CharonII
#=================================#
# working with monitor bamo128  (cs.ba-berlin.de) -> first load bamo128 in boot section
# make all ch -> generates bajvm.bin, avr8bootpack and chapp
# load bajvm.bin avr8bootpack and chapp in flash, then start bajos with the "go" command of  monitor

# TOOLS
CC		= avr-gcc
OBJCOPY		= avr-objcopy
PROGRAM	= avrdude

# DEVICE
ARCH		= AVR5
PART		= atmega128
DEFS		+= -DCH -DAVR8
TEXTSEGMENT	= 0x100
STACKSEGMENT	= 0x4000

OPTIMIZATION		=
# -O3

# JAVA SOURCES
#JAVAAPPSOURCES		= JAVATESTS/Charon.java
JAVAAPPSOURCES		= JAVATESTS/SchedulerTest.java

PLATFORMCSOURCES	= $(TARGETHWFOLDER)/lcd.c $(TARGETHWFOLDER)/shift.c $(TARGETHWFOLDER)/ds182x.c \
			$(TARGETHWFOLDER)/platform.c $(TARGETHWFOLDER)/native.c
PLATFORMASMSOURCES 	= $(TARGETHWFOLDER)/routines.asm $(TARGETHWFOLDER)/routines_stack.asm

OBJFILES		+=	$(PLATFORMASMSOURCES:.asm=.o)


all:	clean compile bootclasses compCharon bootpack app

charon:
	@:
ch:
	@:
CH:	
	@:

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH)   -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE) -Wl,--defsym=__stack=0x807e00 -mtiny-stack 
#-mno-tablejump -mshort-calls 
#	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH)   -Wl,--section-start,.data=0x800a00,--defsym=__heap_start=0x8022b00,--defsym=__heap_end=0x807Dff,--defsym=__stack=0x807E00    -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE)
#	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH)   -Wl,--defsym=__heap_end=0x807eff  -Wl,--defsym=__stack=0x807f00   -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE)
#		 -Wl,--defsym=__heap_start=0x802000,--defsym=__heap_end=0x807fff    -o$@
# ... <- stack 0x1100 data -> bss ->  0x2200 heap->
	@echo $(MSG_BINARY_IMAGE)
	$(VERBOSE_CMD) avr-objcopy -O binary $(TARGETFILE)  $(BIN)
	@${CC} --version


.PHONY:	bootpack
bootpack:
	@echo -n generate file: avr8bootpack\; numClasses: 
	@printf %4d `echo $(JAVABOOTCLASSES)| wc -w` |tee  avr8bootpack
	@for i in $(JAVABOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> avr8bootpack;	cat $$i >> avr8bootpack;	done
	@echo "  length:" `ls -l avr8bootpack | cut -f5 -d' '`

.PHONY:	app
app:
	@echo -n generate file: chapp\; numClasses: 
	@printf %4d `echo $(JAVAAPPCLASSES)| wc -w` |tee  chapp
	@for i in $(JAVAAPPCLASSES) ;do printf %4d `cat $$i| wc -c` >> chapp;	cat $$i >> chapp;	done
	@echo "  length:" `ls -l chapp | cut -f5 -d' '`

%.o: %.c
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) ${CC} -c -Wall $(DEFS) ${DEBUGGEN} ${OPTIMIZATION} -mmcu=$(PART)  -o $@ $< # -mtiny-stack

%o : %asm
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) $(CC) -x assembler-with-cpp -Wall ${DEFS} ${DEBUGGEN}  -mmcu=$(PART) -gstabs -Wa,-ahlms=$(<:.asm=.lst)  -c $< -o $@

# java app
compCharon:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(JAVAAPPSOURCES)
