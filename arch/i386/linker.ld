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


    .text ALIGN(4K) : AT (ADDR (.text) - 0xC0000000)
    {
    _text_start = .;
        *(.text)
    }
    _text_end = .;


    .rodata ALIGN(4K) : AT (ADDR(.rodata) - 0xC0000000)
    {
    _rodata_start = .;
        *(.rodata)
    }
    _rodata_end = .;


    .data ALIGN(4K) : AT (ADDR(.data) - 0xC0000000)
    {
    _data_start = .;
        *(.data)
    }
    _data_end = .;


    .bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
    {
    _bss_start = .;
        *(.COMMON)
        *(.bss)       
    }
    _bss_end = .;
    _kernel_end = .;
    
}