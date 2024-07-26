cmake_minimum_required(VERSION 3.9)

project(fatfs-aft VERSION 0.0.1 LANGUAGES C ASM)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_ASM_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER riscv64-unknown-elf-g++)

set(KERNEL_ROOT external/aft-femtokernel)
set(LINKER_SCRIPT ${KERNEL_ROOT}/arch/aftx07/link.ld)
set(C_FLAGS "-march=rv32imac_zicsr_zifencei -mabi=ilp32 -static -nostartfiles -ffreestanding -ffunction-sections -fdata-sections -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -O2 -g -mcmodel=medany")

add_compile_options(${C_FLAGS})
add_link_options(${C_FLAGS})

if(NOT DEFINED ENV{AFTX07_ROOT})
    message(FATAL_ERROR "$AFTX07_ROOT is not set")
else()
    set(TEST_RUNNER $ENV{AFTX07_ROOT}/aft_out/sim-verilator/Vaftx07)
endif()

add_library(fatfs-aft STATIC
    source/diskio.c
    source/diskio_mmc.c
    source/diskio_ram.c
    source/diskio_usb.c
    source/ff.c
    source/ffsystem.c
    source/ffunicode.c
    # TODO: create cmake file for femtokernel
    ${KERNEL_ROOT}/kernel/format.c
)

target_include_directories(fatfs-aft PUBLIC include)
target_include_directories(fatfs-aft PUBLIC ${KERNEL_ROOT}/include)

add_executable(TestExec
    examples/example.c
    # TODO: create cmake file for femtokernel
    ${KERNEL_ROOT}/arch/aftx07/boot.S
    ${KERNEL_ROOT}/kernel/kernel.c
    ${KERNEL_ROOT}/kernel/interrupt.c
    ${KERNEL_ROOT}/kernel/alloc.c
    ${KERNEL_ROOT}/kernel/riscv.c
)
target_link_libraries(TestExec fatfs-aft)
target_link_options(TestExec PRIVATE -T ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT})
add_test(NAME TestExec
    COMMAND ${TEST_RUNNER})

add_custom_command(
    TARGET TestExec
    POST_BUILD
    COMMAND riscv64-unknown-elf-objcopy
    ARGS -O binary $<TARGET_FILE:TestExec> meminit.bin
)
