//
// Created by cam on 4/6/21.
//


#include "idt.h"

typedef struct {
    uint16_t limit;
    uint64_t offset;
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

void idt_edit_entry (int index, uint64_t interruptAddress, uint16_t segment, uint8_t type)
{
  IDT_Entry idt_entry;
  idt_entry.zero = 0;
  idt_entry.offset_1 = (interruptAddress & 0x00000000FFFF);
  idt_entry.offset_2 = (interruptAddress & 0x0000FFFF0000) >> 16;
  idt_entry.offset_2 = (interruptAddress & 0xFFFFFFFF00000000) >> 32;
  idt_entry.selector = segment;
  idt_entry.type_attr = type;
  idt_entry.ist = 0;
  IDT_ENTRIES[index] = idt_entry;
}

void isr ()
{
  int i = 0;
  i++;
}

void idt_init ()
{
  IDT_DESCRIPTOR.limit = (sizeof (IDT_Entry) * IDT_SIZE) - 1;
  IDT_DESCRIPTOR.offset = (uint64_t) &IDT_ENTRIES;

  for (int i = 0; i < IDT_SIZE; i++)
    {
      idt_edit_entry (i, (uint64_t) &isr, 0x08, 0x8e);
    }

  __asm__("lidt %0"
        :
        :"m"(IDT_DESCRIPTOR));

}
