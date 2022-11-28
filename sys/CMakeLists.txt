add_library(
    riscv_d1_sys
        start.S

        kernel.c
        printf.c
        mbox.c

        lradc.c
        uart.c
)
target_compile_definitions(
    riscv_d1_sys
    PRIVATE PRINTF_INCLUDE_CONFIG_H
)
target_include_directories(
    riscv_d1_sys
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
