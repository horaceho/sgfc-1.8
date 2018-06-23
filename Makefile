# Makefile for SGFC
# Copyright (C) 1996-2018 by Arno Hollosi
# (see 'main.c' for more copyright information)

# System configuration
# SHELL = /bin/bash

# System environment
CC = gcc

#OPTIONS = -pedantic -W -Wimplicit -Wreturn-type -Wswitch -Wformat\
#	  -Wuninitialized -Wparentheses -Wpointer-arith -Wbad-function-cast\
#	  -Wcast-qual -Wcast-align -Waggregate-return -Wstrict-prototypes\
#	  -Wnested-externs -Wshadow -Wchar-subscripts -ansi

OPTIONS = 
OPTIMIZATION = -O1
CFLAGS = $(OPTIMIZATION) $(OPTIONS)

LIB = 
OBJ = execute.o gameinfo.o load.o main.o parse.o parse2.o\
	properties.o save.o strict.o util.o

sgfc: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIB)

all: clean sgfc

clean:
	rm -f $(OBJ) sgfc

%.o: %.c all.h protos.h 
	$(CC) $(CFLAGS) -c $<
