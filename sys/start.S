    .global _start
_start:
    csrrci x0, mstatus, 0x8
    csrw mie, zero

.option push
.option norelax
    la t0, __global_pointer$
    ld gp, (t0)
.option pop

    la sp, __stack_end

_loop:
    jal main
    j .

    .global table_val_set
table_val_set:
    la t0, sys_irq_handler
    csrw mtvec, t0
    jr ra

    .global sys_irq_enable_all
sys_irq_enable_all:
    csrs mstatus, 0x8
    ret

    .global sys_irq_disable_all
sys_irq_disable_all:
    csrrci zero, mstatus, 0x8
    ret
