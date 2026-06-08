# gets i686-host (host it's built for?)
DEFAULT_HOST !=./default_host.sh
HOST ?= $(DEFAULT_HOST)				# set only if not already set
HOSTARCH!=./target-triplet-to-arch.sh $(HOST)

#tool chain
CC=i686-elf-gcc
CPP=i686-elf-g++
AS=i686-elf-gcc
LD=i686-elf-gcc

#initail flags
#CFLAGS ?= -O2 -g 
CFLAGS ?= -g 
CPPFLAGS ?=
LDFLAGS ?= 
LIBS ?=

DESTDIR ?=/data/projects/kernel/sysroot
PREFIX ?=/usr/local
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot
INCLUDE_DIR ?=$(PREFIX)/include

# directories
OBJDIR=object
SRCDIR=source
INCDIR=include
LIBDIR=lib
ISODIR=iso

ISO = DragonOS.iso
TARGET=DragonOS.kernel

#add my flags
CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra -pedantic -Wconversion -Wcast-align -Wcast-qual \
		 -Wredundant-decls -Wsign-conversion -Wswitch-default -Wundef -Wfloat-equal -Werror -fPIC
CPPFLAGS := $(CPPFLAGS) -D__is_kernel -Iinclude -g
LDFLAGS := $(LDFLAGS) -nostdlib -L$(LIBDIR) -Wl,-Map,output.map
LIBS := $(LIBS) -lkrnl -ltty -lc -lstdio -lintr -lgcc -lkbd -lcmd

LIB_LIST = \
$(LIBDIR)/libkrnl.a \
$(LIBDIR)/libtty.a \
$(LIBDIR)/libc.a \
$(LIBDIR)/libstdio.a \
$(LIBDIR)/libgcc.a \
$(LIBDIR)/libintr.a \
$(LIBDIR)/libkbd.a \
$(LIBDIR)/libcmd.a \


ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

#add architecture specific flags
CFLAGS := $(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS := $(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS := $(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS := $(LIBS) $(KERNEL_ARCH_LIBS)
ASFLAGS:= -m32 -Wa,-alh -MD

KERNEL_OBJS = \
$(KERNEL_ARCH_OBJS) \
$(OBJDIR)/kernel.o #\
#$(OBJDIR)/execcmd.o \


OBJS = \
$(OBJDIR)/crti.o \
$(OBJDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(OBJDIR)/crtend.o \
$(OBJDIR)/crtn.o \



LINK_LIST = \
$(LDFLAGS) \
$(OBJDIR)/crti.o \
$(OBJDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(OBJDIR)/crtend.o \
$(OBJDIR)/crtn.o \


.PHONY: all clean run debug

.SUFFIXES: .o .S .cpp .c

all: $(ISO) 


$(ISO) : $(TARGET) 
	cp $(TARGET) $(ISODIR)/boot
	grub2-mkrescue -o $(ISO) $(ISODIR) 

$(TARGET): $(OBJS) $(LIB_LIST) $(ARCHDIR)/linker.ld 
	$(LD) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) -Wl,--start-group $(LINK_LIST) -Wl,--end-group 
	grub2-file --is-x86-multiboot2 DragonOS.kernel 

# libraries (do nothing they were changed)
$(LIBDIR)/libkrnl.a : ;
$(LIBDIR)/libtty.a : ;
$(LIBDIR)/libc.a : ;
$(LIBDIR)/libstdio.a : ;
$(LIBDIR)/libgcc.a : ;
$(LIBDIR)/libcmd.a : ;

# arch directory objects

$(OBJDIR)/crtbegin.o: 
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@ 


$(OBJDIR)/crtend.o: 
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@ 


$(OBJDIR)/boot.o : $(ARCHDIR)/boot.S
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)


$(OBJDIR)/crti.o : $(ARCHDIR)/crti.S
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)


$(OBJDIR)/crtn.o : $(ARCHDIR)/crtn.S
	$(CC) $(ASFLAGS) -c $< -o $@  $(CDFLAGS) $(CPPFLAGS)

#kernel objects

$(OBJDIR)/kernel.o : $(SRCDIR)/kernel.c 
	$(CC) -MD -c $< -o $@ -std=gnu23 $(CFLAGS) $(CPPFLAGS) 

$(OBJDIR)/execcmd.o : $(SRCDIR)/execcmd.c 
	$(CC) -MD -c $< -o $@ -std=gnu23 $(CFLAGS) $(CPPFLAGS) 

clean: 
	rm -f DragonOS.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d *.*.d 


run:
	qemu-system-i386 -cdrom DragonOS.iso -no-reboot -no-shutdown

debug:
	qemu-system-i386 -cdrom DragonOS.iso -no-reboot -no-shutdown -s -S

-include $(OBJS:.o=.d)
