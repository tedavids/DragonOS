# handler page fault

.section .text
.type int14_handler,@function;
.global int14_handler;

int14_handler:

    cli                 # disable interrupts
 
    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 4 values: ERROR EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    call pagefault      # my actual routine

    add $4, %esp        # clean up the stack

    popa                # restore registers
    
    sti                 # reenable interrupts

    iret                # exit handler

