//
// Created by cam on 3/27/21.
//

#ifndef LILO_VGA_H
#define LILO_VGA_H

#include <stdbool.h>

typedef enum {
    Color_Graphics_Four,
    Color_Graphics_Sixteen,
    Color_Graphics_TwoHundredFiftySix,
    Text
} VGA_Modes;

typedef enum {
    Black,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Brown,
    Light_Gray,
    Dark_Gray,
    Light_Blue,
    Light_Green,
    Light_Cyan,
    Light_Red,
    Pink,
    Yellow,
    White
} VGA_Colors;

void print_c (char c, VGA_Colors foreground, VGA_Colors background, bool blink);
void print_s (char *text, VGA_Colors foreground, VGA_Colors background, bool blink);
void scroll ();

#endif //LILO_VGA_H
