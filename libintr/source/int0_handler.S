# handler interrupt divide by zero interrupt

.section .text
.type int0_handler,@function;
.global int0_handler;

int0_handler:

    cli                 # disable interrupts
 
    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 3 values:  EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    call dividebyzero   # my actual routine

    add $4, %esp        # clean up the stack
    
    popa                # restore registers

    sti                 # reenable interrupts

    iret                # exit handler

