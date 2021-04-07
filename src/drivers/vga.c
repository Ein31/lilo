//
// Created by cam on 3/27/21.
//

#include <stdint-gcc.h>
#include "vga.h"
#include "../util/io.h"

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

void vga3_scroll ()
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
  VGA_Cell blank_cell;
  blank_cell.displayChar = ' ';
  blank_cell.flags = 0;
  for (int i = 0; i < VGA_WIDTH; i++)
    {
      VGA_CELLS[VGA_HEIGHT - 1][i] = blank_cell;
    }
  row--;
  col = 0;
  vga3_setCursor (row, col);
}

void vga3_print_c (char c, VGA_Colors foreground, VGA_Colors background)
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
      VGA_CELLS[row][col] = vga_cell;
      col++;
    }
  if (col >= VGA_WIDTH)
    {
      col = 0;
      row++;
    }
  if (row >= VGA_HEIGHT)
    {
      vga3_scroll ();
    }
  vga3_setCursor (row, col);
}

void vga3_print (char *text, VGA_Colors foreground, VGA_Colors background)
{
  for (int i = 0; text[i] != 0; i++)
    {
      vga3_print_c (text[i], foreground, background);
    }
}

int vga3_setCursor (uint8_t yPos, uint8_t xPos)
{
  uint8_t compositePos = yPos * VGA_WIDTH + xPos;
  outb (0x3D4, 0x0F);
  outb (0x3D5, (compositePos & 0xFF));
  outb (0x3D4, 0x0E);
  outb (0x3D5, ((compositePos >> 8) & 0xFF));
  return 0;
}
