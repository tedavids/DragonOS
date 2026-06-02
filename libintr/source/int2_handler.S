# handler interrupt non maskable interrupt

.section .text
.type int2_handler,@function;
.global int2_handler;

int2_handler:

    cli                 # disable interrupts
 
    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 3 values:  EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    call NMI_handler    # my actual routine

    add $4, %esp        # clean up the stack

    popa                # restore registers

    sti                 # reenable interrupts

    iret                # exit handler

