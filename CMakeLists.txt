cmake_minimum_required(VERSION 3.9)

project(fatfs-aft VERSION 0.0.1 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER riscv64-unknown-elf-g++)

add_compile_options(-march=rv32imac_zicsr -mabi=ilp32 -mcmodel=medany -static -nostartfiles -nostdlib -ffreestanding -ffunction-sections -fdata-sections -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -O2 -g)

if(NOT DEFINED ENV{AFTX07_ROOT})
    message(FATAL_ERROR "$AFTX07_ROOT is not set")
endif()

add_library(fatfs-aft SHARED
    source/diskio.c
    source/ff.c
    source/ffsystem.c
    source/ffunicode.c
)

target_include_directories(fatfs-aft PUBLIC include)

add_executable(TestExec examples/example.c)
target_link_libraries(TestExec fatfs-aft)
add_test(NAME TestExec
         COMMAND TestInstantiator)

add_custom_command(
    TARGET TestExec
    POST_BUILD
    COMMAND riscv64-unknown-elf-objcopy
    ARGS -O binary $<TARGET_FILE:TestExec> meminit.bin
)
