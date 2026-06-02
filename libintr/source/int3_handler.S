# handler debug request

.section .text
.type int3_handler,@function;
.global int3_handler;

int3_handler:

    cli

    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 3 values:  EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    #call <placeholder> # my actual routine

    add $4, %esp        # clean up the stack

    popa                # restore registers

    sti 
    
    iret                # exit handler

