//
// Created by cam on 3/27/21.
//

#ifndef LILO_GDT_H
#define LILO_GDT_H

#define GDT_ATTR_NULL 0X00
#define GDT_ATTR_PRESENT (1 << 7)
#define GDT_ATTR_CODE (1 << 4)
#define GDT_ATTR_EXEC (1 << 3)
#define GDT_ATTR_GROW_DOWN (1 << 2)
#define GDT_ATTR_RW (1 << 1)
#define GDT_ATTR_TSS 1
#define GDT_ATTR_RING3 (1 << 5 | 1 << 6)

#define GDT_GRAN1B 0X00
#define GDT_FLAG_NULL 0X00
#define GDT_FLAG_GRAN4K (1 << 3)
#define GDT_FLAG_MODE32BIT (1 << 2)
#define GDT_FLAG_MODE64BIT (1 << 1)

#endif //LILO_GDT_H
