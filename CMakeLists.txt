cmake_minimum_required(VERSION 3.15)
project(lilo)

## Setup cross-compilation
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR i686)

ENABLE_LANGUAGE(ASM)

set(TOOLS $ENV{HOME}/opt/cross/bin)
set(CMAKE_C_COMPILER ${TOOLS}/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER ${TOOLS}/x86_64-elf-g++)
set(CMAKE_ASM_COMPILER ${TOOLS}/x86_64-elf-as)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti")
set(CMAKE_C_FLAGS "-ffreestanding -O2 -Wall -Wextra -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2")


## A simple lib to test C compilation and linking

## Building the kernel binary
add_executable(${CMAKE_PROJECT_NAME} src/kernel/main.c src/kernel/stivale.h src/kernel/stivale2.h)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -ffreestanding -O2 -no-pie -static -nostdlib -lgcc")

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND generate-iso.sh
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building ECHFS for Lilo and Limine")