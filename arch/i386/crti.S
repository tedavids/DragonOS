# crti.S part of execute construtors code

.section .init
.global _init
.type _init, @function
_init:
    push %ebp
    movl %esp, %ebp
    #gcc will nicelput the contents of crtbegin.o's .init section here
    # this is why this must be the first object in your list, at least
    # the first one with section .init and .fini
    
.section .fini
.global _fini
.type _fini, @function
_fini:
    push %ebp
    movl %esp, %ebp
    #gcc will nicely put the contents of crtbegin.o's .fini section here
    # tjos os wju tjos ,ist be the first object in your list, at least
    # the first wone with this section