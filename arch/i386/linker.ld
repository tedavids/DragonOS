# linker for virtualization

ENTRY(_start)

SECTIONS 
{


    . = 0x00100000;

    /* the kernel will live at 3GB + 1M in the virtual address space, */
    /* which will be mapped to 1MB in the physical space. */
    /* Note: we page align the sections */

    _boot_kernel_start = .;
    .multiboot.data : 
    {
        *(.multiboot.data)
    } 


    /* this is what does the magic */
    .multiboot.text : 
    {
        *(.multiboot.text)
    }

    . += 0xC0000000;
    _kernel_start = .;
    _text_start = .;
    .text ALIGN(4K) : AT (ADDR (.text) - 0xC0000000)
    {
        *(.text)
    }
    _text_end = .;

    _rodata_start = .;
    .rodata ALIGN(4K) : AT (ADDR(.rodata) - 0xC0000000)
    {
        *(.rodata)
    }
    _rodata_end = .;

    _data_start = .;
    .data ALIGN(4K) : AT (ADDR(.data) - 0xC0000000)
    {
        *(.data)
    }
    _data_end = .;

    _bss_start = .;
    .bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
    {
        *(.COMMON)
        *(.bss)       
    }
    _bss_end = .;
    _kernel_end = .;
    
}