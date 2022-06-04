# $Id: Makefile,v 1.1 2014/05/02 22:33:16 david Exp $
#---------------------------------------------------------------
#             CONFIGURE THESE VARIABLES IF NEEDED
#---------------------------------------------------------------

ROOT = ${HOME}/compiladores/root
RTS_LIB_DIR = $(ROOT)/usr/lib

COMPILER=../simple
ASM=yasm

#---------------------------------------------------------------
#    PROBABLY, NO NEED TO CHANGE ANYTHING BEYOND THIS POINT
#---------------------------------------------------------------

COMPILER_FLAGS = 
LDFLAGS  = -m elf_i386 -L$(RTS_LIB_DIR) -lrts

SOURCES     = $(wildcard *.spl)
OBJECTS     = $(SOURCES:%.spl=%.o)
EXECUTABLES = $(SOURCES:%.spl=%)

#---------------------------------------------------------------
#                DO NOT CHANGE AFTER THIS LINE
#---------------------------------------------------------------

all: $(EXECUTABLES)

%.c: %.spl
	$(COMPILER) $< -o $@

%.asm: %.spl
	$(COMPILER) $< -o $@

%.o: %.asm
	$(ASM) -felf32 $< -o $@

%:%.o
	$(LD) $< -o $@ $(LDFLAGS)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLES) *.c *.asm

#---------------------------------------------------------------
#                           THE END
#---------------------------------------------------------------
