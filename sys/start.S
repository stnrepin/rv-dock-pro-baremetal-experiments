.macro IRQ_DISABLE_ALL
    csrrci x0, mstatus, 0x8
.endm

    .global _start
_start:
    IRQ_DISABLE_ALL
    csrw mie, zero

.option push
.option norelax
    la t0, __global_pointer$
    ld gp, (t0)
.option pop

    la sp, __stack_end

    jal main
_loop:
    j _loop

    .global sys_irq_set_handler
sys_irq_set_handler:
    csrw mtvec, a0
    ret

    .global sys_irq_enable_all
sys_irq_enable_all:
    csrs mstatus, 0x8
    ret

    .global sys_irq_disable_all
sys_irq_disable_all:
    IRQ_DISABLE_ALL
    ret
