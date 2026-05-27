# DragonOS
This is a 'play' project to create an OS from scratch

## 🚀 Features

* Uses GRUB2 to load, using Multiboot
* loads the page direcory table, and page entry tables with the kernel addresses
* switches to virtual mode
* should call kernel_main

## TODOs

* load the GDT
* load a print function (kprintf - kernel formatted print)
* load the IDT
* set up the APIC
* add a keyboard handler

## 🛠️ Prerequisite

* you will need an i686 cross compiler
* you can go to osdev.org for how to build this (https://wiki.osdev.org/GCC_Cross-Compiler)

## 📦 Building and Running

* clone the repository
* run the makefile in the root directory

## Credits

* so far just OSDev

## License 
Disributed under GPL3 