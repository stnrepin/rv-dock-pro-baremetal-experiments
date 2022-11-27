    .global _start
_start:
.option push
.option norelax
    la t0, __global_pointer$
    ld gp, (t0)
.option pop

    la sp, __stack_end
    jal main
    j .