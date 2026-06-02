# DragonOS
This is a 'play' project to create an OS from scratch

## 🚀 Features

* Uses GRUB2 to load, using Multiboot
* loads the page direcory table, and page entry tables with the kernel addresses
* switches to virtual mode
* should call kernel_main

## TODOs

* Map 0x0-0xFFFF memory
* Set up paging
*   load multiboot to get memory size
*   Set permissions correctly on existing memory (.text, .rodata, & Bios)
*   create functions mapKernelPage, mapKernelROPage, mapUserPage,
*       mapUserROPage, unmapPage
*   Get clock working
* Set up keyboard
* Get commadn prompt

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