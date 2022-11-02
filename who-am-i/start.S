    .equ REGSZ, 8

    .global _start
_start:
    j reset

/*
 * The actual reset code
 */
reset:
    /* Mask all interrupts */
    csrw mideleg, zero
    csrw medeleg, zero
    csrw mie, zero
    csrw mip, zero

    /* Initialize global pointer */
.option push
.option norelax
    la t0, _global_pointer$
    ld gp, (t0)
.option pop

    /* Initialize stacks */
    la t1, _stack_start
    ld t1, (t1)
    la t2, _stack_end
    ld t2, (t2)
    sub t0, t2, t1
    csrr t3, mhartid
    li t4, 1
    div t0, t0, t4
    mul t0, t0, t3
    sub sp, t2, t0

    j main

/*
 * The location of section
 */
    .align 3
_image_start:
    .dword __image_start
_image_end:
    .dword __image_end
_global_pointer$:
    .dword __global_pointer$
_data_start:
    .dword __data_start
_data_end:
    .dword __data_end
_bss_start:
    .dword __bss_start
_bss_end:
    .dword __bss_end
_stack_start:
    .dword __stack_start
_stack_end:
    .dword __stack_end
