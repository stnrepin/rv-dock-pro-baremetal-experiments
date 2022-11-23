add_library(
    riscv_d1_sys
        start.S
        uart.c
)
target_include_directories(
    riscv_d1_sys
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
