# handler interrupt for the keyboard

.section .text
.type kbd_handler,@function;
.global kbd_handler;

kbd_handler:

    cli                 # disable interrupts
 
    pusha               # push general purpose registers

    # stack is now [esp]-> %edi, %esi, %esp, %ebx, %edx, %ecx, %eax
    #  then our 3 values:  EIP, CS, EFLAGS

    push %esp           # pass a pointer to the stack

    call ps2kbd_handler # my actual routine

    add $4, %esp        # clean up the stack

    popa                # restore registers
        
    sti                 # reenable interrupts

    iret                # exit handler

