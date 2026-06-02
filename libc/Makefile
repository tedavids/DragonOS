# this makefile creates the libc.a object
DEFAULT_HOST!=../default_host.sh
HOST?=$(DEFAULT_HOST)
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

TARGET=libc.a
SRCDIR=source
OBJDIR=object
DESTDIR=.
ARCHDIR=../arch/$(HOSTARCH)/
LIBDIR=../lib

#tool chain
CC=i686-elf-gcc
CPP=i686-elf-g++
AS=i686-elf-gcc
LD=i686-elf-gcc

#initail flags
#CFLAGS ?= -O2 -ggdb
CFLAGS ?= -g
CPPFLAGS ?=
LDFLAGS ?= 
LIBS ?=

#add my flags
CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra -pedantic -Wconversion -Wcast-align -Wcast-qual \
		 -Wredundant-decls -Wsign-conversion -Wswitch-default -Wundef -Wfloat-equal -Werror -fPIC
CPPFLAGS := $(CPPFLAGS) -D__is_libtty -isystem ../include 
LDFLAGS := $(LDFLAGS) -nostdlib  -Wl,-Map,output.map


#add architecture specific flags
CFLAGS := $(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS := $(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LIBTTY_CFLAGS := $(LIBTTY_CFLAGS) $(ARCH_CFLAGS)
LIBTTY_CPPFLAGS := $(LIBTTY_CPPFLAGS) $(ARCH_CPPFLAGS)

FREEOBJS=$(OBJDIR)/memcmp.o \
$(OBJDIR)/memcpy.o \
$(OBJDIR)/memmove.o \
$(OBJDIR)/memset.o \
$(OBJDIR)/strlen.o \
$(OBJDIR)/abort.o \
$(OBJDIR)/strncpy.o \
$(OBJDIR)/strntok.o \
$(OBJDIR)/strncmp.o \
$(OBJDIR)/toHex.o \
$(OBJDIR)/toBase10.o \
$(OBJDIR)/trim.o \
$(OBJDIR)/snprintf.o \

HOSTEDOBJS = \

OBJS=$(FREEOBJS) \
$(HOSTEDOBJS) \

.PHONY: all clean

all: $(LIBDIR)/$(TARGET)

$(LIBDIR)/$(TARGET) : $(DESTDIR)/$(TARGET)
	cp $(DESTDIR)/$(TARGET) $(LIBDIR)/$(TARGET)

$(DESTDIR)/$(TARGET) : $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(OBJDIR)/memcmp.o : $(SRCDIR)/memcmp.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/memcpy.o : $(SRCDIR)/memcpy.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/memmove.o : $(SRCDIR)/memmove.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/memset.o : $(SRCDIR)/memset.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/strlen.o : $(SRCDIR)/strlen.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
	
$(OBJDIR)/strncpy.o : $(SRCDIR)/strncpy.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
	
$(OBJDIR)/strncmp.o : $(SRCDIR)/strncmp.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/strntok.o : $(SRCDIR)/strntok.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/abort.o : $(SRCDIR)/abort.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/toHex.o : $(SRCDIR)/toHex.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/toBase10.o : $(SRCDIR)/toBase10.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/trim.o : $(SRCDIR)/trim.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/snprintf.o : $(SRCDIR)/snprintf.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(DESTDIR)/*.a
	rm -f $(OBJDIR)/*.d 
	rm -f $(OBJDIR)/*.o
