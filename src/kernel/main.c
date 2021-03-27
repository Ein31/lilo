#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "stivale.h"
#include "stivale2.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__amd64__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void terminal_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}



// We need to tell the stivale bootloader where we want our stack to be.
// We are going to allocate our stack as an uninitialised array in .bss.
static uint8_t stack[4096];

// stivale2 uses a linked list of tags for both communicating TO the
// bootloader, or receiving info FROM it. More information about these tags
// is found in the stivale2 specification.

// As an example header tag, we're gonna define a framebuffer header tag.
// This tag tells the bootloader that we want a graphical framebuffer instead
// of a CGA-compatible text mode. Omitting this tag will make the bootloader
// default to text mode.
struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
        // All tags need to begin with an identifier and a pointer to the next tag.
        .tag = {
                // Identification constant defined in stivale2.h and the specification.
                .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
                // If next is 0, then this marks the end of the linked list of tags.
                .next = 0
        },
        // We set all the framebuffer specifics to 0 as we want the bootloader
        // to pick the best it can.
        .framebuffer_width  = 0,
        .framebuffer_height = 0,
        .framebuffer_bpp    = 0
};

// The stivale2 specification says we need to define a "header structure".
// This structure needs to reside in the .stivale2hdr ELF section in order
// for the bootloader to find it. We use this __attribute__ directive to
// tell the compiler to put the following structure in said section.
__attribute__((section(".stivale2hdr"), used))
struct stivale2_header stivale_hdr = {
        // The entry_point member is used to specify an alternative entry
        // point that the bootloader should jump to instead of the executable's
        // ELF entry point. We do not care about that so we leave it zeroed.
        .entry_point = 0,
        // Let's tell the bootloader where our stack is.
        // We need to add the sizeof(stack) since in x86(_64) the stack grows
        // downwards.
        .stack = (uintptr_t)stack + sizeof(stack),
        // No flags are currently defined as per spec and should be left to 0.
        .flags = 0,
        // This header structure is the root of the linked list of header tags and
        // points to the first one (and in our case, only).
        .tags = (uintptr_t)&framebuffer_hdr_tag
};

// We will now write a helper function which will allow us to scan for tags
// that we want FROM the bootloader (structure tags).
void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {
        // If the tag pointer is NULL (end of linked list), we did not find
        // the tag. Return NULL to signal this.
        if (current_tag == NULL) {
            return NULL;
        }

        // Check whether the identifier matches. If it does, return a pointer
        // to the matching tag.
        if (current_tag->identifier == id) {
            return current_tag;
        }

        // Get a pointer to the next tag in the linked list and repeat.
        current_tag = (void *)current_tag->next;
    }
}


void _start(struct stivale2_struct *stivale2_struct)
{

    // Let's get the framebuffer tag.
    struct stivale2_struct_tag_framebuffer *fb_hdr_tag;
    fb_hdr_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

    // Check if the tag was actually found.
    if (fb_hdr_tag == NULL) {
        // It wasn't found, just hang...
        for (;;) {
            asm ("hlt");
        }
    }

    // Let's get the address of the framebuffer.
    uint8_t *fb_addr = (uint8_t *)fb_hdr_tag->framebuffer_addr;

    // Let's try to paint a few pixels white in the top left, so we know
    // that we booted correctly.
    for (size_t i = 0; i < 128; i++) {
        fb_addr[i] = 0xff;
    }

//    /* Initialize terminal interface */
//    terminal_initialize();
//
//    /* Newline support is left as an exercise. */
//    terminal_writestring("Hello, kernel World!\n");

    __asm__("hlt");
}