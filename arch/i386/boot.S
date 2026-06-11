
#   Copyright (C) 2026 Tom Davidson

#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.


# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC1,    0x1BADB002       # 'magic number' lets bootloader find the header (multiboot1)
.set MAGIC2,    0xE85250D6	 	# 'magic number' for multiboot 2
.set CHECKSUM1, -(MAGIC1 + FLAGS) # checksum of above, to prove we are multiboot

# Declare a multiboot header that marks the program as a kernel.
#.section .multiboot.data, "aw"
#.align 4
#.long MAGIC1
#.long FLAGS
#.long CHECKSUM1

# Multiboot 2 header
.section .multiboot.data, "aw"
.align 8

multiboot2:
#general multiboot 2 header
.long MAGIC2											# Multiboot 2 magic number
.long 0x0												# Architecture (0 = i386)
.long (multiboot2_end - multiboot2)						# header length
.long -(0xE85250D6 + 0 + (multiboot2_end - multiboot2))  # checksum
.long 0x0												# type
.long 0x0												# Flags
.long 0x8												# size

.align 8
# tag 1 general info
multiboot2_info_request_tag:
.long 0x1												# General info
.long 0													# required or optional (0 = optional)
.long multiboot2_info_request_end - multiboot2_info_request_tag # size of section
.long 1													# command line 
.long 2													# bootloader Name
.long 4													# memory sizes
.long 5													# bios boot device
.long 6													# memory map 
multiboot2_info_request_end:

.align 8
# the end of the tag list
multiboot2_end_tag:
.long 0													# type
.long 0													# Flags
.long multiboot2_end_end - multiboot2_end_tag			# size
multiboot2_end_end:

multiboot2_end:

# The kernel entry point.
# This will do the initial switch to virtual address space
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
	# make sure boot is ok
	cmp $MAGIC2, %eax
	jne boot
badboot:
	cli
	hlt
	jmp badboot

boot:

	# inhibit interrupts until the APIC is set up
	cli

	# mask the APIC until we get it set up
	xor %ax, %ax
	mov	0xFF, %al
	out	%al, $0x21		# Mask all master PIC IRQs
	out %al, $0xA1		# Mask all slave PIC IRQs

	# save multiboot info
    mov %ebx, (MULTIBOOT_INFO_ADDRESS - 0xC0000000)

	# Physical address of page_table_C00.
	movl $(page_table_C00 - 0xC0000000), %edi

	# First address to map is address 0.
	movl $0, %esi

	# Map 1024 pages.  if the kernel gets longer add a page table after _C00
	# and make this 2048
	movl $1024, %ecx

bootprocaddress:
	# Only map the kernel.
	cmpl $(_boot_kernel_start), %esi
	jl loadaddress

	# do we have the entire kernel
	cmpl $(_kernel_end - 0xC0000000), %esi
	jg krnlmapdone

	# Map physical address as "present, writable". Note that this maps
	# .text and .rodata as writable. TODO: write a function to set permissions
	# correctly
	movl %esi, %edx		# this is our address
	orl $0x003, %edx	# set present and writable bits
	movl %edx, (%edi)	# put it in the table

loadaddress:
	# Size of page is 4096 bytes.
	addl $4096, %esi
	# Size of entries in page_table_C00 is 4 bytes.
	addl $4, %edi
	# Loop to the next entry if we haven't finished.
	loop bootprocaddress

	# we have mapped the kernel, now set up the video buffer
.global krnlmapdone
krnlmapdone:
	# Map VGA video memory to 0xC03FF000 as "present, writable".
	#movl $(0x000B8000 | 0x003), page_table_C00 - 0xC0000000 + 1023 * 4

	# The page table is used at both page directory entry 0 (virtually from 0x0
	# to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
	# 768 (virtually from 0xC0000000 to 0xC03FFFFF) (thus mapping it in the
	# higher half). The kernel is identity mapped because enabling paging does
	# not change the next instruction, which continues to be physical. The CPU
	# would instead page fault if there was no identity mapping.

	# Map the page table to both virtual addresses 0x00000000 and 0xC0000000.
	movl $(page_table_C00 - 0xC0000000 + 0x003), page_directory - 0xC0000000 + 0
	movl $(page_table_C00 - 0xC0000000 + 0x003), page_directory - 0xC0000000 + 768 * 4
	
	# to make things easier we will map 12M for the kernel in advance
	movl $(page_table_C04 - 0xC0000000 + 0x003), page_directory - 0xC0000000 + 769 * 4
	movl $(page_table_C04 - 0xC0000000 + 0x003), page_directory - 0xC0000000 + 770 * 4


	# Set cr3 to the address of the page_directory.
	movl $(page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3

	call MapPage0

	# Enable paging and the write-protect bit.
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump. 
	lea _start2, %ecx
	jmp *%ecx

	# This function maps the pages for the base 1M
MapPage0:
		# Physical address of page_table_C00.
	movl $(page_table_000 - 0xc0000000) , %edi

	# First address to map is address 0.
	xor %esi, %esi

	# Map the first 4 megs because we don't 
	# know where the multiboot info will be
	movl $1024, %ecx

proczeroaddress:

	# Map physical address as "present, writable". Note that this maps
	# the bios and as writable

	movl %esi, %edx		# this is our address
	orl $0x003, %edx	# set present and writable bits
	movl %edx, (%edi)	# put it in the table

# Size of page is 4096 bytes.
	addl $4096, %esi	# Size of entries in page_table_000 is 4 bytes.
	addl $4, %edi		# Loop to the next entry if we haven't finished.
	loop proczeroaddress

zeromapdone:

	ret




#   TTTTTTTT  EEEEEEEE  XX   XX  TTTTTTTT
#      TT     EE         XX XX      TT
#      TT     EEEEE       XXX       TT
#      TT     EE         XX XX      TT
#      TT     EEEEEEEE  XX   XX     TT

.section .text
.global _start2
_start2:
	# At this point, paging is fully set up and enabled.

	# Unmap the identity mapping as it is now unnecessary. 
	# replace with the identity mapping for the first meg
	movl $((page_table_000 - 0xC0000000) + 0x003), page_directory + 0

	# Reload crc3 to force a TLB flush so the changes to take effect.
	movl %cr3, %ecx
	movl %ecx, %cr3

	# Set up the stack.
	mov $stack_top, %esp

	# load the GDT

    # load Global Discriptor Table

    lgdt GDTP

     mov $0x10, %ax
     mov %ax, %ds
     mov %ax, %es
     mov %ax, %fs
     mov %ax, %gs
     mov %ax, %ss
    ljmp $0x08, $flush_CS    #KERNEL_CODE_SELECTOR
flush_CS:

    mov $0x10, %eax
    mov %ax, %ds


    # GDT loaded
    movl BOOTSTATUS, %eax
    addl GDTLOADED, %eax
    movl %eax, BOOTSTATUS




	# Enter the high-level kernel.  Interrupts are stil
	# supressed, you must call EnableInterrupts once the 
	# IDT and APIC are set up

	call kernel_main

	# Infinite loop if the system has nothing more to do.

bootloop:
	cli
	hlt
	jmp bootloop



.global setIDT
.type setIDT, @function
#load the IDT 
setIDT:
    lidt [IDTP]
    
    sidt IDTR               # save for debugging

    movl BOOTSTATUS, %eax
    addl IDTLOADED, %eax
    movl %eax, BOOTSTATUS

    ret 

	# Reload crc3 to force a TLB flush so the changes to take effect.
.global FlushPageTable
.type FlushPageTable, @function
FlushPageTable:
	push %ecx
	movl %cr3, %ecx
	movl %ecx, %cr3
	pop %ecx
	ret

#	RRRRRR    OOOOOOOO  DDDDDD       A      TTTTTTTT      A
#   RR    RR  OO    OO  DD   DD     AAA        TT        AAA
#	RRRRRR    OO    OO  DD    DD   AA AA       TT       AA AA
#   RR   RR   OO    OO  DD   DD   AAAAAAA      TT      AAAAAAA
#   RR    RR  OOOOOOOO  DDDDDD   AA     AA     TT     AA     AA

.section .rodata
.align 4096

.global OSNAME
.type OSNAME, @object
OSNAME:	.asciz	"DragonOS"

.global OSVERSION
.type OSVERSION, @object
OSVERSION:	.asciz	"0.0.0"

.global OSDATE
.type OSDATE, @object
OSDATE:		.asciz	"2026-05-30"


# BOOTSTATUS constants
.global GDTLOADED
.type GDTLOADED, @object
GDTLOADED:    .long 0x00000001
.global IDTLOADED
.type IDTLOADED, @object
IDTLOADED:    .long 0x00000002
.global TSSLOADED
.type TSSLOADED, @object
TSSLOADED:    .long 0x00000004
.global MEMMAPLOADED
.type MEMMAPLOADED, @object
MEMMAPLOADED: .long 0x00000008
.global PAGINGIMPLEMENTED
.type PAGINGIMPLEMENTED, @object
PAGINGIMPLEMENTED: .long 0x00000010

.global KERNEL_CODE_SELECTOR, KERNEL_DATA_SELECTOR
KERNEL_CODE_SELECTOR:    .word   0X08
KERNEL_DATA_SELECTOR:    .word   0x10
.global USER_CODE_SELECTOR, USER_DATA_SELECTOR
USER_CODE_SELECTOR:      .word   0x18
USER_DATA_SELECTOR:      .word   0x20
.global TSS_SELECTOR
TSS_SELECTOR:            .word   0x28


#	DDDDDD        A      TTTTTTTT      A
#   DD   DD      AAA        TT        AAA
#   DD    DD    AA AA       TT       AA AA
#   DD   DD    AAAAAAA      TT      AAAAAAA
#   DDDDDD    AA     AA     TT     AA     AA

# Allocate the initial stack.
.section .data
.align 4096
.global stack_bottom
.type stack_bottom, @object
stack_bottom:
.skip 16384 # 16 KiB
.global stack_top
.type stack_top, @object
stack_top:

# boot status
.global BOOTSTATUS
.type BOOTSTATUS, @object
BOOTSTATUS: .long       0x00000000

# GDT

GDT:
gdt_start:
    # Null descriptor (segment selector 0x00)
    .quad       0x0000000000000000

    # kernel code segment (segment selector 0x08)
    .word       0xFFFF          # limit (size) about 200M
    .word       0x0000          # memory start address, low 16 bits
    .byte       0x00            # memory start address, middle 8 bytes
    .byte       0x9A            # access byte
    .byte       0xCF            # Flags 
    .byte       0x00            # memory start address, high 8 bytes

   # kernel data segment (segment selector 0x10)
    .word       0xFFFF          # limit (size) about 200M
    .word       0x0000          # memory start address, low 16 bits
    .byte       0x00            # memory start address, middle 8 bytes
    .byte       0x92            # access byte
    .byte       0xC0            # Flags 
    .byte       0x00            # memory start address, high 8 bytes

   # user code segment (segment selector 0x18)
    .word       0xFFFF          # limit (size) about 200M
    .word       0x16C1          # memory start address, low 16 bits
    .byte       0x00            # memory start address, middle 8 bytes
    .byte       0xFA            # access byte
    .byte       0xCF            # Flags 
    .byte       0x00            # memory start address, high 8 bytes

   # user data segment (segment selector 0x20)
    .word       0xFFFF          # limit (size) about 200M
    .word       0x16C1          # memory start address, low 16 bits
    .byte       0x00            # memory start address, middle 8 bytes
    .byte       0xF2            # access byte
    .byte       0xCF            # Flags 
    .byte       0x00            # memory start address, high 8 bytes

    # Transaction State Segment (TSS) (segment selector 0x28)
    .word       0x0068          # limit (size) about 200M
    .word       0x12C1          # memory start address, low 16 bits
    .byte       0x00            # memory start address, middle 8 bytes
    .byte       0xF2            # access byte
    .byte       0xCF            # Flags 
    .byte       0x00            # memory start address, high 8 bytes
gdt_end:

    # pointer to GDT
.global GDTP
.type GDTP,@object
GDTP:
    .word gdt_end - gdt_start - 1   # size of GDT
    .long gdt_start

#interrupt descriptor table
.align 8
.type IDT, @object
.GLOBAL IDT
IDT: 
.skip 2048
idt_end:

# pointer to IDT
.align 8
.GLOBAL IDTP
.type IDTP, @object
IDTP:
    .word (idt_end - IDT - 1)
    .long IDT

#used for debugging
.global IDTR
.type IDTR,@object
IDTR:	.word	0x0000
		.long	0x00000000

.global MULTIBOOT_INFO_ADDRESS
.type MULTIBOOT_INFO_ADDRESS, @object
MULTIBOOT_INFO_ADDRESS: .long  0x00000000

#	BBBBBB    SSSSSSSS  SSSSSSSS      
#   BB    BB  SS        SS
#	BBBBBB    SSSSSSSS  SSSSSSSS
#	BB    BB        SS        SS
#	BBBBBB    SSSSSSSS  SSSSSSSS

.section .bss
	.align 4096

# Preallocate pages used for paging. Don't hard-code addresses and assume they
# are available, as the bootloader might have loaded its multiboot structures or
# modules there. This lets the bootloader know it must avoid the addresses.

.global page_directory
.type page_directory, @object
page_directory:
	.skip 4096
.global page_table_000
.type page_table_000, @object
page_table_000:
.skip 4096

.global page_table_C00
.type page_table_C00, @object
page_table_C00:
	.skip 4096
	.global page_table_C00
.type page_table_C00, @object
page_table_C04:
	.skip 4096
page_table_C08:
	.skip 4096
# Further page tables may be required if the kernel grows beyond 3 MiB.
# you will also have to update paging.c functions initPDTVirtPhys() &
# dltPDTEntry()
