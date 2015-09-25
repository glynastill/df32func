# Project: df32func
#
#	df32func dll extension for Console Mode DataFlex 3.2
#
# Copyright (c) 2007-2015, glyn@8kb.co.uk
# Author: Glyn Astill <glyn@8kb.co.uk>

CC   = gcc.exe
WINDRES = windres.exe
RES  = df32func.res
OBJ  = df32func.o memman.o gnuregex.o $(RES)
LINKOBJ  = df32func.o memman.o gnuregex.o $(RES)
LIBS =  --no-export-all-symbols --add-stdcall-alias -lwsock32 -lgnurx
BIN  = df32func.dll
CFLAGS = -O2
DLLWRAP=dllwrap.exe
DEFFILE=libdf32func.def
STATICLIB=libdf32func.a
RM = del -f

.PHONY: all all-before all-after clean clean-custom

all: all-before df32func.dll all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN) $(RES) $(DEFFILE) $(STATICLIB)

$(BIN): $(LINKOBJ)
	$(DLLWRAP) --output-def $(DEFFILE) --implib $(STATICLIB) $(LINKOBJ) $(LIBS) -o $(BIN)

df32func.o: df32func.c
	$(CC) -c df32func.c -o df32func.o $(CFLAGS)

memman.o: memman.c
	$(CC) -c memman.c -o memman.o $(CFLAGS)

gnuregex.o: gnuregex.c
	$(CC) -c gnuregex.c -o gnuregex.o $(CFLAGS)

df32func.res: df32func.rc 
	$(WINDRES) -i df32func.rc --input-format=rc -o $(RES) -O coff 
