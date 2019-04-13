# need to add dependency support and alternative OS builds

#.POSIX:

IDA=$(HOME)/.idapro/
IDASDK=../idasdk/

CC=clang++
CXXFLAGS=-g -D_DEBUG -fvisibility=hidden -fvisibility-inlines-hidden -Wall -Wextra -DNO_OBSOLETE_FUNCS -D__IDP__
LDFLAGS=-D__IDP__
LDLIBS=-lida64 -lpthread -liconv

ifeq ($(OS),Windows_NT)
	CXXFLAGS += -D__NT__

else
	UNAME_S := $(shell uname -s)
	
	ifeq ($(UNAME_S),Darwin)
		CXXFLAGS += -D__MAC__ -D__X64__ -D__EA64__ -isystem $(IDASDK)include 
		LDFLAGS += -L$(IDASDK)lib/x64_mac_gcc_64 -Wl,-install_name,avr32.dylib -dynamiclib -current_version 1.0 -compatibility_version 1.0 -single_module
	endif

	ifeq ($(UNAME_S),Linux)
		CXXFLAGS += -D__LINUX__
	endif
endif

all: avr32.dylib

install: avr32.dylib
	cp -f avr32.dylib $(IDA)procs/avr3264.dylib
	cp -f avr32.cfg $(IDA)cfg/

avr32.dylib: avr32.o ana.o emu.o out.o reg.o ins.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

