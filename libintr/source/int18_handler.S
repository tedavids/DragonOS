# handler  for a machine check

.section .text
.type int18_handler,@function;
.global int18_handler;

int18_handler:

    cli                 # disable interrupts
 
    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 4 values: ERROR EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    call machinecheck   # my actual routine

    add $4, %esp        # clean up the stack

    popa                # restore registers

    sti                 # reenable interrupts

    iret                # exit handler

