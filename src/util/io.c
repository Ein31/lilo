//
// Created by cam on 4/6/21.
//

#include "io.h"

void outb (uint16_t port, uint8_t val)
{
  __asm__ volatile  ( "outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb (uint16_t port)
{
  uint8_t readByte;
  __asm__ volatile  ( "inb %1, %0 " : "=a"(readByte) : "Nd"(port));
  return readByte;
}