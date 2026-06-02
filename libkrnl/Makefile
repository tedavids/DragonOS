# this makefile creates the libc.a object
DEFAULT_HOST!=../default_host.sh
HOST?=$(DEFAULT_HOST)
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

TARGET=libkrnl.a
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
CFLAGS ?= -O2 -ggdb
#CFLAGS ?= -g
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
ASFLAGS:= -m32 -Wa,-alh -MD -masm=att 

FREEOBJS= $(OBJDIR)/gdt.o\
$(OBJDIR)/getselectortext.o \
$(OBJDIR)/ports.o \
$(OBJDIR)/multiboot.o \
$(OBJDIR)/paging.o \
$(OBJDIR)/time.o \
$(OBJDIR)/heap.o \

HOSTEDOBJS = \

OBJS=$(FREEOBJS) \
$(HOSTEDOBJS) \

.PHONY: all clean

$(LIBDIR)/$(TARGET) : $(DESTDIR)/$(TARGET)
	cp $(DESTDIR)/$(TARGET) $(LIBDIR)/$(TARGET)

$(DESTDIR)/$(TARGET) : $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(OBJDIR)/gdt.o : $(SRCDIR)/gdt.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/getselectortext.o : $(SRCDIR)/getselectortext.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/ports.o : $(SRCDIR)/ports.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/multiboot.o : $(SRCDIR)/multiboot.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/paging.o : $(SRCDIR)/paging.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/time.o : $(SRCDIR)/time.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/heap.o : $(SRCDIR)/heap.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(DESTDIR)/*.a
	rm -f $(OBJDIR)/*.d 
	rm -f $(OBJDIR)/*.o
