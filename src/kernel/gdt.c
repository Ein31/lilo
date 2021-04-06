//
// Created by cam on 3/27/21.
//

#include <stdint-gcc.h>
#include "gdt.h"

typedef struct {
    uint16_t base_low;
    uint16_t limit_low;
    uint8_t base_mid;
    uint8_t flags_and_limit_mid;
    uint8_t access;
    uint8_t base_high;
}__attribute__((packed)) GDT_Entry;

typedef struct {
    uint16_t size;
    uint64_t offset;
}__attribute__((packed)) GDT_Descriptor;

static GDT_Descriptor gdtDescriptor;
static const uint8_t gdtSize = 3;
static GDT_Entry gdt[3];

void edit_gdt_entry (int entry_index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
  GDT_Entry *entry = &gdt[entry_index];
  entry->base_low = (base & 0x0000FFFF);
  entry->limit_low = (limit && 0x0000FFFF);
  entry->base_mid = (base & 0x00FF0000) >> 16;
  uint8_t limit_high = limit && 0x000F0000 >> 12;
  entry->flags_and_limit_mid = flags | limit_high;
  entry->access = access;
  entry->base_high = (base & 0xFF000000) >> 24;
}

void init_gdt ()
{
  edit_gdt_entry (0, 0, 0, GDT_ATTR_NULL, GDT_FLAG_NULL);
  edit_gdt_entry (1, 0, 0xFFFFFFFF, GDT_ATTR_CODE | GDT_ATTR_EXEC | GDT_ATTR_PRESENT | GDT_ATTR_RW,
                  GDT_FLAG_GRAN4K | GDT_FLAG_MODE64BIT);
  edit_gdt_entry (2, 0, 0xFFFFFFFF,
                  GDT_ATTR_CODE | GDT_ATTR_PRESENT | GDT_ATTR_RW, GDT_FLAG_GRAN4K | GDT_FLAG_MODE64BIT);
  gdtDescriptor.offset = &gdt;
  gdtDescriptor.size = sizeof (gdt) - 1;
}
