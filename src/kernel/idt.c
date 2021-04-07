//
// Created by cam on 4/6/21.
//

#include "idt.h"

typedef struct {
    uint16_t limit;
    uint32_t offset;
} IDT_Descriptor;

typedef struct {
    uint16_t offset_1; // offset bits 0..15
    uint16_t selector; // a code segment selector in GDT or LDT
    uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    uint8_t type_attr; // type and attributes
    uint16_t offset_2; // offset bits 16..31
    uint32_t offset_3; // offset bits 32..63
    uint32_t zero;     // reserved
} IDT_Entry;

static IDT_Descriptor IDT_DESCRIPTOR;
static IDT_Entry IDT_ENTRIES[IDT_SIZE];

void idt_init ()
{

}

void idt_edit_entry (int index, uint64_t interruptAddress, uint16_t segment, uint8_t ist, uint8_t type)
{
  IDT_Entry idt_entry;
  idt_entry.zero = 0;
  idt_entry.selector = 1; // Doing 1 since we have a flatmap in gdt for now
  idt_entry.offset_1 = (interruptAddress & 0x00000000FFFF);
  idt_entry.offset_2 = (interruptAddress & 0x0000FFFF0000) >> 16;
  idt_entry.offset_2 = (interruptAddress & 0xFFFFFFFF00000000) >> 32;
}