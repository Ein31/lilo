//
// Created by cam on 4/6/21.
//

#ifndef _IO_H_
#define _IO_H_

#include <stdint-gcc.h>

void outb (uint16_t port, uint8_t val);
uint8_t inb (uint16_t port);
#endif //_IO_H_
