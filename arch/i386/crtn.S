#crtn.s end of global constructior code

.section .init
    # gcc will nicelyy put the contents of crtend.o's .init section
    # as long as crtn.o is the last object in the list

    popl %ebp
    ret

.section .fini
   # gcc will nicelyy put the contents of crtend.o's .fini section
    # as long as crtn.o is the last object in the list

    popl %ebp
    ret