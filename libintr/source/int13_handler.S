# handler interrupt general protection fault

.section .text
.type int13_handler,@function;
.global int13_handler;

int13_handler:

    cli                 # disable interrupts
 
    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 4 values: ERROR EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    call gpfault        # my actual routine

    add $4, %esp        # clean up the stack

    popa                # restore registers

    sti                 # reenable interrupts

    iret                # exit handler

