# handler FPU error

.section .text
.type int16_handler,@function;
.global int16_handler;

int16_handler:

    cli                 # disable interrupts
 
    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 4 values: ERROR EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    call fpuerror       # my actual routine

    add $4, %esp        # clean up the stack

    popa                # restore registers

    sti                 # reenable interrupts

    iret                # exit handler

