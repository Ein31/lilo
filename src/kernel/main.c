

#include <stdint-gcc.h>
#include <stddef.h>
#include "stivale2.h"
#include "../drivers/vga.h"
#include "gdt.h"

// We need to tell the stivale bootloader where we want our stack to be.
// We are going to allocate our stack as an uninitialised array in .bss.
static uint8_t stack[4096];

__attribute__((section(".stivale2hdr"), used))
struct stivale2_header stivale_hdr = {
    // We're using the linked entry point so we don't need to do anything.
    .entry_point = 0,
    // Let's tell the bootloader where our stack is.
    // We need to add the sizeof(stack) since in x86(_64) the stack grows
    // downwards.
    .stack = (uintptr_t) stack + sizeof (stack),
    // No flags are currently defined as per spec and should be left to 0.
    .flags = 0,
    // This header structure is the root of the linked list of header tags and
    // points to the first one (and in our case, only).
    .tags = NULL
};

//Kernel Entry Point
void _start (struct stivale2_struct *stivale2_struct)
{

  vga3_print ("Lilo VGA Running in Mode 3\n", Yellow, Blue);
  gdt_init ();
  vga3_print ("GDT Initialized\n", Yellow, Blue);

  asm ("hlt");
}