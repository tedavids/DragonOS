# DragonOS
This is a 'play' project to create an OS from scratch

## 🚀 Features

* Uses GRUB2 to load, using Multiboot
* loads the page direcory table, and page entry tables with the kernel addresses
* switches to virtual mode
* calls kernel_main
* kernel main does the following:
*   Inits the display, loads the IDT, sets up the APIC Timer, sets up a memory map,
*   initializes pageing, and the keyboard
* Finaly drops info the kernel shell

## TODOs

* Set up paging
*   create functions mapKernelPage, mapUserPage, unmapPage, 
*       setPageReadOnly, setPageReadWrite
* set up heap processing
* change my invalidatePage function in paging.c to do a TLB shootdown, prep for multiprocessor



## 🛠️ Prerequisite

* you will need an i686 cross compiler
* you can go to osdev.org for how to build this (https://wiki.osdev.org/GCC_Cross-Compiler)

## 📦 Building and Running

* clone the repository
* run the makefile in the root directory

## Credits

* so far just OSDev

* OSDev contributors
 Octocontrabass, sebihepp, and nullplan for their help getting to the Higher Half

## License 
Disributed under GPL3 