# handler interrupt for the APIC timer

.section .text
.type int32_handler,@function;
.global int32_handler;

int32_handler:

    cli                 # disable interrupts
 
    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 3 values:  EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    call apictimer      # my actual routine

    add $4, %esp        # clean up the stack

    popa                # restore registers
        
    sti                 # reenable interrupts

    iret                # exit handler

