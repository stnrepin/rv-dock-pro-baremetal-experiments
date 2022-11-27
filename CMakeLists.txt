cmake_minimum_required(VERSION 3.16)

project(riscv_d1 LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(PROJECTS "hello" "whoami")
set(RISCV_D1_LDSCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/sys/link.ld")

add_compile_options(
    -Wall
    -Wextra
    -Werror
)

function(add_linker_script ELF_TARGET)
    target_link_options(
        ${ELF_TARGET}
        PRIVATE -T ${RISCV_D1_LDSCRIPT}
    )
endfunction()

function(make_bin_file NAME ELF_TARGET)
    add_custom_target(
        ${NAME}_bin
        ALL
        DEPENDS ${ELF_TARGET}
    )

    add_custom_command(
        TARGET ${NAME}_bin
        COMMAND ${CMAKE_OBJCOPY}
        ARGS
            --binary-architecture=riscv64 --strip-all -O binary
                $<TARGET_FILE:${ELF_TARGET}>
                $<TARGET_FILE_DIR:${ELF_TARGET}>/${NAME}.bin
    )
endfunction()

add_subdirectory(sys)

foreach(project_name ${PROJECTS})
    message(STATUS "Load project \"${project_name}\"")

    add_subdirectory(${project_name})
    message(STATUS ${SRC})

    add_executable(${project_name}_elf ${SRC})
    target_link_libraries(${project_name}_elf riscv_d1_sys)

    add_linker_script(${project_name}_elf)
    make_bin_file(${project_name} ${project_name}_elf)
endforeach()
