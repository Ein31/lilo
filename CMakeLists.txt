cmake_minimum_required(VERSION 3.15)
project(lilo)

set(LIMINE_DIR ${CMAKE_SOURCE_DIR}/limine)

## Setup cross-compilation
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR i686)

ENABLE_LANGUAGE(ASM)

set(TOOLS $ENV{HOME}/opt/cross/bin)
set(CMAKE_C_COMPILER ${TOOLS}/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER ${TOOLS}/x86_64-elf-g++)
set(CMAKE_ASM_COMPILER ${TOOLS}/x86_64-elf-as)

set(CMAKE_C_STANDARD 11)

set(CORE_LD_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -static	-nostdlib -no-pie")
set(CORE_C_FLAGS "-I.-ffreestanding	-fno-stack-protector -fno-pic -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mcmodel=kernel	-mno-red-zone")

set(CMAKE_C_FLAGS "${CORE_C_FLAGS} -Wall -Wextra -O2 -pipe")


## A simple lib to test C compilation and linking

## Building the kernel binary
add_executable(${CMAKE_PROJECT_NAME} src/kernel/main.c src/kernel/stivale2.h)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS ${CORE_LD_FLAGS})

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND rm -f lilo.hdd
        COMMAND dd if=/dev/zero bs=1M count=0 seek=64 of=lilo.hdd
        COMMAND parted -s lilo.hdd mklabel gpt
        COMMAND parted -s lilo.hdd mkpart primary 2048s 100%
        COMMAND echfs-utils -g -p0 lilo.hdd quick-format 512
        COMMAND echfs-utils -g -p0 lilo.hdd import ${CMAKE_SOURCE_DIR}/limine.cfg limine.cfg
        COMMAND echfs-utils -g -p0 lilo.hdd import lilo lilo
        COMMAND make -C ${CMAKE_SOURCE_DIR}/limine
        COMMAND ${CMAKE_SOURCE_DIR}/limine/limine-install lilo.hdd
        COMMAND echfs-utils -g -p0 lilo.hdd import ${CMAKE_SOURCE_DIR}/limine/limine.sys limine.sys
        COMMAND rm -f ${CMAKE_SOURCE_DIR}/limine/limine-hdd.o ${CMAKE_SOURCE_DIR}/limine/limine-install
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building ECHFS for Lilo and Limine")