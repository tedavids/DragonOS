# handler fault index out of range

.section .text
.type int6_handler,@function;
.global int6_handler;

int6_handler:

    cli                 # disable interrupts

     pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 3 values:  EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack
 
    call invalidopcode  # my actual routine

    add $4, %esp        # clean up the stack
    
    popa                # restore registers

 
    sti                 # reenable interrupts

    iret                # exit handler