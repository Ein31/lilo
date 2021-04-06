//
// Created by cam on 3/27/21.
//

#include <stdint-gcc.h>
#include "vga.h"

#define VGA_BASE_ADDR 0XB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

int row = 0;
int col = 0;

typedef struct {
    char displayChar;
    uint8_t flags;
} VGA_Cell;

volatile VGA_Cell (*VGA_CELLS)[VGA_WIDTH] = (VGA_Cell (*)[VGA_WIDTH]) VGA_BASE_ADDR;

void scroll ()
{
  for (int i = 0; i < VGA_HEIGHT - 1; i++)
    {
      for (int j = 0; j < VGA_WIDTH; j++)
        {
          //GCC Tries to use memmove if you do this in one line. Since I haven't implemented it yet need to split it out and turn the optimizer off. Should revisit this eventually
          VGA_Cell src = VGA_CELLS[i + 1][j];
          VGA_CELLS[i][j] = src;
        }
    }
  row--;
  col = 0;
}

void print_c (char c, VGA_Colors foreground, VGA_Colors background, bool blink)
{
  if (c == 0)
    {
      return;
    }
  if (c == '\n')
    {
      row++;
      col = 0;
    }
  else
    {
      VGA_Cell vga_cell;
      vga_cell.displayChar = c;
      vga_cell.flags = 0;
      vga_cell.flags = foreground;
      vga_cell.flags |= background << 4;
      if (blink)
        {
          vga_cell.flags |= 1 << 8;
        }
      VGA_CELLS[row][col] = vga_cell;
      col++;
    }
  if (col >= VGA_WIDTH)
    {
      col--;
      row++;
    }
  if (row >= VGA_HEIGHT)
    {
      scroll ();
    }
}

void print_s (char *text, VGA_Colors foreground, VGA_Colors background, bool blink)
{
  for (int i = 0; text[i] != 0; i++)
    {
      print_c (text[i], foreground, background, blink);
    }
}
