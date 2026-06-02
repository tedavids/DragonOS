# handler Alignment Check

.section .text
.type int17_handler,@function;
.global int17_handler;

int17_handler:
    cli                 # disable interrupts
 
    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 4 values: ERROR EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    call alignmentcheck # my actual routine

    add $4, %esp        # clean up the stack

    popa                # restore registers
    
    sti                 # reenable interrupts

    iret                # exit handler

