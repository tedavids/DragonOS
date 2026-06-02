# handler interrupt general protection fault

.section .text
.type int8_handler,@function;
.global int8_handler;

int8_handler:

    cli                 # disable interrupts
    pushfl              # push error flags

    pusha               # push general purpose registers

    push %esp           # pass a pointer to the stack

    call doublefault    # my actual routine

# not needed this is always fatal   

    add $4, %esp        # clean up the stack

    popa                # remove registers
 
    sti                 # reenable interrupts

    iret                # exit handler

ctxswt: .long