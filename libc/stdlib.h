//
// Created by cam on 4/10/21.
//

#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stdint-gcc.h>

void itoa (char *buffer, int val, uint8_t buffSize);
void uitoa (char *buffer, uint32_t val, uint8_t buffSize);
void uitoa_hex (char *buffer, uint32_t val, uint8_t buffSize);
void uitoa_binary (char *buffer, uint32_t val, uint8_t buffSize);

#endif //_STDLIB_H_
