# DragonOS
This is a 'play' project to create an OS from scratch

## 🚀 Features

* Uses GRUB2 to load, using Multiboot
* loads the page direcory table, and page entry tables with the kernel addresses
* switches to virtual mode
* calls kernel_main
* kernel main does the following:
*   Inits the display, loads the IDT, sets up the APIC Timer, sets up a memory map,
*   initializes pageing, initializes the heap, and the keyboard
* Finaly drops info the kernel shell

## TODOs

* create kcalloc function
* finish strntok function
* finish mapPage to get a page from the kernel (or user) heap
* create power down command (shutdown)
* create swap file system
* determine which file system to support (FAT32, ext4?)
* multi-threading
* fix screen scrolling to do page up
* Heap Additions
  - Add Garbage collection on out of memory
  - Add preallocated pages
  - expose all stats
  - prefix pagealloctree functions and globals with pt
* General cleanup
  - add a global constant PAGESIZE and replace all 4096 and 0x1000
  - add a global constant PGTBLADRSIZE and replace all referencx to 0x400000, the size of memory covered by one page table
  - update header files with parameter info
  - update doc with constants, funtions, parameters, and types
  - move 'Halting message to the kernelabort() function and remove from the interrupt functions


## 🛠️ Prerequisite

* you will need an i686 cross compiler
* you can go to osdev.org for how to build this (https://wiki.osdev.org/GCC_Cross-Compiler)

## 📦 Building and Running

* clone the repository
* run the makefile in each lib* directory
* run the makefile in the root directory

## Credits

* OSDev contributors
 Octocontrabass, sebihepp, and nullplan for their help getting to the Higher Half

* https://www.tutorialspoint.com/data_structures_algorithms/avl_tree_algorithm.htm for a self balancing binary tree

## License 
Disributed under GPL3 