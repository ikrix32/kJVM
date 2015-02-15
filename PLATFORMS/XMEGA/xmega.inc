#
# HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
# See the file "license.terms" for information on usage and redistribution of this file.
#

# java for 8 Bit uC atxmega128 at xplain board
#=================================#
# working without monitor (standalone application)
# make all am -> generates bajos.bin (jvm, boot classes and appl classes) and burns at arduinoMega
#
# working with monitor bamo128  (cs.ba-berlin.de) -> first load bamo128 in boot section
# make all am withmon -> generates bajvm.bin, avr8bootpack and xpapp
# load bajvm.bin avr8bootpack and xpapp in flash, then start bajos with the "go" command of  monitor
# fuses setting
# make writeFusesA	-> start after reset at address 0000
# make writeFusesM	-> start after reset vom boot section 0xf000 (in words)

# TOOLS
# from avr32studio www.atmel.no/beta_ware
BINDIR	=
DUDEDIR = 
CC		= $(BINDIR)avr-gcc
OBJCOPY		= $(BINDIR)avr-objcopy
PROGRAM		= $(DUDEDIR)avrdude
# flasher means the avrdude device which you are using for flashing
#FLASHER 	= stk200 
FLASHER 	= avrispmkII -P usb 


# DEVICE
#ARCH			= AVR7
PART			= atxmega128a1
DEFS 			+= -DAVR8 -DXPLAIN -DF_CPU=16000000 -IXMEGA -I$(TARGETHWFOLDER)/atmel/include -include $(TARGETHWFOLDER)/atmel/autoconf.h
DEFS+=  -Os

#OPTIMIZATION		=   
#-gdwarf-2 -Werror-implicit-function-declaration  -mrelax -Wmissing-prototypes   -Wstrict-prototypes  -Wpointer-arith -fshort-enums  -ffunction-sections -fdata-sections

# for standalone application
CLASSFILEBASE		=  0xC480
# fix address for class files (in words), make it better

# JAVA SOURCES
JAVAAPPSOURCES		= JAVATESTS/XM.java




PLATFORMCSOURCES	= $(TARGETHWFOLDER)/native.c $(TARGETHWFOLDER)/atmel/generic_memcpy.c \
			  $(TARGETHWFOLDER)/atmel/generic_strlen.c  $(TARGETHWFOLDER)/atmel/port.c \
			  $(TARGETHWFOLDER)/atmel/generic_memset.c  $(TARGETHWFOLDER)/atmel/init.c \
			  $(TARGETHWFOLDER)/atmel/uart_xmega_ctrl.c $(TARGETHWFOLDER)/atmel/delay.c \
			  $(TARGETHWFOLDER)/atmel/generic_strcmp.c  $(TARGETHWFOLDER)/platform.c $(TARGETHWFOLDER)/atmel/sysclk.c \
			  $(TARGETHWFOLDER)/atmel/xmega_pr.c

PLATFORMASMSOURCES	=  $(TARGETHWFOLDER)/routines.S  $(TARGETHWFOLDER)/routines_stack.S

#$(TARGETHWFOLDER)/ccp.S  X$(TARGETHWFOLDER)MEGA/entry.S
OBJFILES		+=	$(PLATFORMASMSOURCES:.S=.o)

# GOALS
ifeq ($(findstring withmon,$(MAKECMDGOALS)),withmon)
DEFS+= -DWITHMON
all:	clean  bootclasses compXM bootpack app compile
	@echo created $(BIN) arv8bootpack xpapp

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)${CC} $(TARGETHWFOLDER)/atmel/link.lds $(filter %.o,$+) -mmcu=$(PART)    -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE) 
	@echo $(MSG_BINARY_IMAGE)
	$(VERBOSE_CMD) $(OBJCOPY) -O binary $(TARGETFILE) $(BIN)
	@${CC} --version
#	$(VERBOSE_CMD)${CC} $(filter %.o,$+) -mmcu=$(PART) -architecture=$(ARCH)   -Wl,--section-start,.data=0x800200,--defsym=__heap_start=0x800c00,--defsym=__heap_end=0x802080,--defsym=__stack=0x802100    -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm -o$(TARGETFILE)
##	@rm ${EXE}
#-Wl,--defsym=__stack=0x802100 
else
all:	clean  bootclasses compXM bootpack app compile bajospack burnbajos

$(TARGETFILE):	${OBJFILES}
	@echo $(MSG_LINKING)
	$(VERBOSE_CMD)${CC}  $(filter %.o,$+) -mmcu=$(PART) -lm -o$(TARGETFILE) 
	@echo $(MSG_BINARY_IMAGE)
	$(VERBOSE_CMD) $(OBJCOPY) -O binary $(TARGETFILE) $(BIN)
#   -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm 
#XMEGA/link.lds 

#"/usr/lib/gcc/avr/4.3.3/avrxmega7/libgcc.a"
# -Wl,--section-start,.data=0x802000,--defsym=__heap_start=0x802800,--defsym=__heap_end=0x803f00,--defsym=__stack=0x803fff  
bajospack:
	$(VERBOSE_CMD) cat $(BIN) > bajos.bin
	$(VERBOSE_CMD)(i=`ls -l $(BIN) | cut -f5 -d' '`; limit=`expr 2 \* $$(($(CLASSFILEBASE)))`;while [ "$$i" -lt "$$limit" ];  do  i=$$(($$i+1));echo -n 'x' >> bajos.bin;  done)
	cat avr8bootpack >> bajos.bin
	cat xpapp >> bajos.bin
	@echo Creating binary image to \`bajos.bin\'	.
	@${CC} --version

endif	#withmon

withmon:
	@:
xmega:
	@:

burnbajos:
	$(PROGRAM) -p x128a1 -cjtagmkII -D -e -P usb  -Uflash:w:bajos.bin:a


.PHONY:	writeFusesM		# arduinoMega with monitor starts at 0xf000
writeFusesM:	# 4 k word boot section, start from boot section after reset
	$(PROGRAM) -p x128a1 -cjtagmkII  -P usb  -Ulfuse:w:0xff:m
	$(PROGRAM) -p x128a1 -cjtagmkII  -P usb  -Uhfuse:w:0xd8:m
	$(PROGRAM) -p x128a1 -cjtagmkII -P usb  -Uefuse:w:0xf5:m

.PHONY:	writeFusesA		# arduinoMega without monitpts starts at 0x0000
writeFusesA:	# 4 k word boot section, start from address 0000 after reset
	$(PROGRAM) -p x128a1 -cjtagmkII  -P usb  -Ulfuse:w:0xff:m
	$(PROGRAM) -p x128a1 -cjtagmkII  -P usb  -Uhfuse:w:0xd9:m
	$(PROGRAM) -p x128a1 -cjtagmkII  -P usb  -Uefuse:w:0xf5:m

.PHONY:	bootpack
bootpack:
	@echo -n generate file: avr8bootpack\; numClasses: 
	@printf %4d `echo $(JAVABOOTCLASSES)| wc -w` |tee  avr8bootpack
	@for i in $(JAVABOOTCLASSES) ;do printf %4d `cat $$i| wc -c` >> avr8bootpack;	cat $$i >> avr8bootpack;	done
	@echo "  length:" `ls -l avr8bootpack | cut -f5 -d' '`


.PHONY:	app
app:
	@echo -n generate file: xpapp\; numClasses: 
	@printf %4d `echo $(JAVAAPPCLASSES)| wc -w` |tee  xpapp
	@for i in $(JAVAAPPCLASSES) ;do printf %4d `cat $$i| wc -c` >> xpapp;	cat $$i >> xpapp;	done
	@echo "  length:" `ls -l xpapp | cut -f5 -d' '`

%.o: %.c
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) ${CC} -c -Wall $(DEFS) ${DEBUGGEN} ${OPTIMIZATION}   -mmcu=$(PART) -o $@ $<   #-mtiny-stack

%o : %S
	@echo $(MSG_COMPILING)
	$(VERBOSE_CMD) $(CC) -x assembler-with-cpp -D__ASSEMBLY__ -Wall ${DEFS} ${DEBUGGEN}  -mmcu=$(PART)  -c $< -o $@

# java app
compXM:	
	$(JAVACOMP) $(JAVACOMPFLAGS) $(JAVACOMPBOOTCLASSES) $(JAVAAPPSOURCES)
