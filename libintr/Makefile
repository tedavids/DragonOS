# this makefile creates the libc.a object
DEFAULT_HOST!=../default_host.sh
HOST?=$(DEFAULT_HOST)
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

TARGET=libintr.a
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
CPPFLAGS ?=
LDFLAGS ?= 
LIBS ?=

#add my flags
CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra -pedantic -Wconversion -Wcast-align -Wcast-qual \
		 -Wredundant-decls -Wsign-conversion -Wswitch-default -Wundef -Wfloat-equal -Werror  -fPIC
CPPFLAGS := $(CPPFLAGS) -D__is_libtty -isystem ../include 
LDFLAGS := $(LDFLAGS) -nostdlib  -Wl,-Map,output.map


#add architecture specific flags
CFLAGS := $(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS := $(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LIBTTY_CFLAGS := $(LIBTTY_CFLAGS) $(ARCH_CFLAGS)
LIBTTY_CPPFLAGS := $(LIBTTY_CPPFLAGS) $(ARCH_CPPFLAGS)
ASFLAGS:= -m32 -Wa,-alh -MD -masm=att 

FREEOBJS= $(OBJDIR)/idt.o \
$(OBJDIR)/int0_handler.o \
$(OBJDIR)/dividebyzero.o \
$(OBJDIR)/int2_handler.o \
$(OBJDIR)/NMI_handler.o \
$(OBJDIR)/int3_handler.o \
$(OBJDIR)/int5_handler.o \
$(OBJDIR)/indexoutofrange.o \
$(OBJDIR)/int6_handler.o \
$(OBJDIR)/invalidopcode.o \
$(OBJDIR)/int7_handler.o \
$(OBJDIR)/devicenotavail.o \
$(OBJDIR)/int8_handler.o \
$(OBJDIR)/doublefault.o \
$(OBJDIR)/int9_handler.o \
$(OBJDIR)/coprocoverrun.o \
$(OBJDIR)/int10_handler.o \
$(OBJDIR)/invalidTSS.o \
$(OBJDIR)/int11_handler.o \
$(OBJDIR)/segnotpresent.o \
$(OBJDIR)/int12_handler.o \
$(OBJDIR)/stacksegflt.o \
$(OBJDIR)/int13_handler.o \
$(OBJDIR)/gpfault.o \
$(OBJDIR)/int14_handler.o \
$(OBJDIR)/pagefault.o \
$(OBJDIR)/int16_handler.o \
$(OBJDIR)/fpuerror.o \
$(OBJDIR)/int17_handler.o \
$(OBJDIR)/alignmentcheck.o \
$(OBJDIR)/int18_handler.o \
$(OBJDIR)/machinecheck.o \
$(OBJDIR)/int19_handler.o \
$(OBJDIR)/floatptexc.o \
$(OBJDIR)/int20_handler.o \
$(OBJDIR)/virtexcp.o \
$(OBJDIR)/int21_handler.o \
$(OBJDIR)/ctlprotexc.o \
$(OBJDIR)/faulthandler.o \
$(OBJDIR)/kernelabort.o \
$(OBJDIR)/apic.o \
$(OBJDIR)/int32_handler.o \
$(OBJDIR)/apictimer.o \
$(OBJDIR)/intprint.o \

HOSTEDOBJS = \

OBJS=$(FREEOBJS) \
$(HOSTEDOBJS) \

.PHONY: all clean



$(LIBDIR)/$(TARGET) : $(DESTDIR)/$(TARGET)
	cp $(DESTDIR)/$(TARGET) $(LIBDIR)/$(TARGET)

$(DESTDIR)/$(TARGET) : $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(OBJDIR)/idt.o : $(SRCDIR)/idt.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int0_handler.o : $(SRCDIR)/int0_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/dividebyzero.o : $(SRCDIR)/dividebyzero.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int2_handler.o : $(SRCDIR)/int2_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/NMI_handler.o : $(SRCDIR)/NMI_handler.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int3_handler.o : $(SRCDIR)/int3_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/int5_handler.o : $(SRCDIR)/int5_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/indexoutofrange.o : $(SRCDIR)/indexoutofrange.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int6_handler.o : $(SRCDIR)/int6_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/invalidopcode.o : $(SRCDIR)/invalidopcode.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int7_handler.o : $(SRCDIR)/int7_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/devicenotavail.o : $(SRCDIR)/devicenotavail.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int8_handler.o : $(SRCDIR)/int8_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/doublefault.o : $(SRCDIR)/doublefault.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int9_handler.o : $(SRCDIR)/int9_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/coprocoverrun.o : $(SRCDIR)/coprocoverrun.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int10_handler.o : $(SRCDIR)/int10_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/invalidTSS.o : $(SRCDIR)/invalidTSS.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int11_handler.o : $(SRCDIR)/int11_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/segnotpresent.o : $(SRCDIR)/segnotpresent.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int12_handler.o : $(SRCDIR)/int12_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/stacksegflt.o : $(SRCDIR)/stacksegflt.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int13_handler.o : $(SRCDIR)/int13_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/gpfault.o : $(SRCDIR)/gpfault.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int14_handler.o : $(SRCDIR)/int14_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/pagefault.o : $(SRCDIR)/pagefault.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int16_handler.o : $(SRCDIR)/int16_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/fpuerror.o : $(SRCDIR)/fpuerror.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int17_handler.o : $(SRCDIR)/int17_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/alignmentcheck.o : $(SRCDIR)/alignmentcheck.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int18_handler.o : $(SRCDIR)/int18_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/machinecheck.o : $(SRCDIR)/machinecheck.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int19_handler.o : $(SRCDIR)/int19_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/floatptexc.o : $(SRCDIR)/floatptexc.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int20_handler.o : $(SRCDIR)/int20_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/virtexcp.o : $(SRCDIR)/virtexcp.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int21_handler.o : $(SRCDIR)/int21_handler.S 
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

$(OBJDIR)/ctlprotexc.o : $(SRCDIR)/ctlprotexc.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/faulthandler.o : $(SRCDIR)/faulthandler.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/kernelabort.o : $(SRCDIR)/kernelabort.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/apic.o : $(SRCDIR)/apic.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/apictimer.o : $(SRCDIR)/apictimer.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/int32_handler.o : $(SRCDIR)/int32_handler.S 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/intprint.o : $(SRCDIR)/intprint.c 
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
clean:
	rm -f $(DESTDIR)/*.a
	rm -f $(OBJDIR)/*.d 
	rm -f $(OBJDIR)/*.o
