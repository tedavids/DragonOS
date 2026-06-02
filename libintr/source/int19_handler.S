# handler  for a SIMD Floating point exception

.section .text
.type int19_handler,@function;
.global int19_handler;

int19_handler:

    cli                 # disable interrupts
 
    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 4 values: ERROR EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    call floatptexc     # my actual routine

    add $4, %esp        # clean up the stack

    popa                # restore registers

    sti                 # reenable interrupts

    iret                # exit handler

