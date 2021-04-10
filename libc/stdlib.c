//
// Created by cam on 4/10/21.
//

#include "stdlib.h"
#include "stdbool.h"

void itoa (char *buffer, int val, uint8_t buffSize)
{
  bool neg = false;
  if (val < 0)
    {
      neg = true;
      val *= -1;
    }

  int i = buffSize - 1;
  while (i >= neg ? 1 : 0)
    {
      buffer[i] = "0123456789"[val % 10];
      val /= 10;
      i--;
    }
  if (neg)
    {
      buffer[i] = '-';
    }
}

void uitoa (char *buffer, uint32_t val, uint8_t buffSize)
{
  int i = buffSize - 1;
  while (i >= 0)
    {
      buffer[i] = "0123456789"[val % 10];
      val /= 10;
      i--;
    }
}

void uitoa_hex (char *buffer, uint32_t val, uint8_t buffSize)
{
  int i = buffSize - 1;
  while (i >= 0)
    {
      buffer[i] = "0123456789ABCDEF"[val % 16];
      val /= 16;
      i--;
    }
}

void uitoa_binary (char *buffer, uint32_t val, uint8_t buffSize)
{
  int i = buffSize - 1;
  while (i >= 0)
    {
      buffer[i] = "01"[val % 2];
      val /= 2;
      i--;
    }
}