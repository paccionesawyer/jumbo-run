/*
 * Tufts University, Winter Break 2020-2021
 * GraphicDisplay.cpp
 * By: Sawyer Bailey Paccione
 * Completed:   January 28th, 2021
 *
 * Description: A library of functions that help display sprites that are 
 *              moving
 * Notes:       Adapted from http://www.technoblogy.com/show?39DR (Liscenced 
 *              Under http://creativecommons.org/licenses/by/4.0/)
 */

#include "Arduino.h"
#include "GraphicDisplay.h"

/******************************************************************************
*                               Public  Members                               *
******************************************************************************/

/*
 * Basic Constructor
 * Purpose:     Inherits the constructor from TFT_eSPI
 * Arguments:   Nothing
 * Returns:     Nothing
 * Effects:     Allocates space for Class TFT_eSPI
 */
GraphicDisplay::GraphicDisplay() : TFT_eSPI()
{
}

/*
 * getPixel
 * Purpose:     Return the colour of the pixel at x, y
 * Arguments:   x [uint16_t]  x-coordinate of the pixel
 *              y [uint16_t]  y-coordinate of the pixel
 * Returns:     [uint16_t]    the color value of the pixel
 * Effects:     Nothing
 * Notes:       
 */
uint16_t GraphicDisplay::getPixel(uint16_t x, uint16_t y)
{
  return readPixel(x, y);
}

/*
 * xorPixel
 * Purpose:     Plot a pixel at x, y by exclusive-ORing the colour with the 
 *              display
 * Arguments:   x     [uint_16t]  x-coordinate of the pixel
 *              y     [uint_16t]  y-coordinate of the pixel
 *              color [uint32_t]  a tft color code
 * Returns:     Nothing (void)
 * Effects:     Color of pixel on WIO Terminal
 * Notes:       
 */
void GraphicDisplay::xorPixel(uint16_t x, uint16_t y, uint32_t color)
{
  uint16_t lastcolor = readPixel(x, y);
  drawPixel(x, y, color ^ lastcolor);
}

/*
 * xorSprite
 * Purpose:     Plot an 8x8 sprite at x, y by exclusive-ORing the colour with 
 *              the display
 * Arguments:   x0      [uint_16t]  x-coordinate of the pixel
 *              y0      [uint_16t]  y-coordinate of the pixel
 *              sprite  [uint64_t]  the 8x8 sprite 
 *              color   [uint32_t]  a tft color code
 * Returns:     Nothing (void)
 * Effects:     Calls xorPixel
 * Notes:       
 */
void GraphicDisplay::xorSprite(uint16_t x0, uint16_t y0, uint64_t sprite, 
                               uint32_t color)
{
  bool bit;
  if ((x0 >= 0) && (x0 + 7 < width()) && (y0 >= 0) && (y0 + 7 < height()))
  {
    for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
      {
        bit = sprite >> (63 - x - y * 8) & 1;
        if (bit)
          xorPixel(x0 + x, y0 + y, color);
      }
    }
  }
}

/*
 * hitSprite
 * Purpose:     Collision detection between an 8x8 sprite at x, y with a 
 *              specified colour
 * Arguments:   x0      [uint_16t]  x-coordinate of the pixel
 *              y0      [uint_16t]  y-coordinate of the pixel
 *              sprite  [uint64_t]  the 8x8 sprite 
 *              color   [uint32_t]  a tft color code
 * Returns:     [bool]  Whether or not the sprite has been hit
 * Effects:     Movement
 * Notes:       
 */
bool GraphicDisplay::hitSprite(uint16_t x0, uint16_t y0, uint64_t sprite, 
                               uint32_t color)
{
  uint16_t row[8];
  uint32_t col16;
  bool bit;
  if ((x0 >= 0) && (x0 + 7 < width()) && (y0 >= 0) && (y0 + 7 < height()))
  {
    for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
      {
        col16 = readPixel(x0 + x, y0 + y);
        bit = sprite >> (63 - x - y * 8) & 1;
        if (bit && (col16 == color))
        {
          return true;
        }
      }
    }
  }
  return false;
}

/*
 * moveSprite
 * Purpose:     Move a sprite from x, y by one pixel in any direction by 
 *              exclusive-ORing only the changed pixels with the display.
 * Arguments:   x0      [uint_16t]  x-coordinate of the pixel
 *              y0      [uint_16t]  y-coordinate of the pixel
 *              sprite  [uint64_t]  The 8x8 sprite 
 *              dx      [int]       Sprite movement in the x-direction
 *              dy      [int]       Sprite movement in the y-direction
 *              color   [uint32_t]  A tft color code
 * Returns:     Nothing (void)
 * Effects:     calls xOrPixel
 * Notes:       
 */
void GraphicDisplay::moveSprite(uint16_t x0, uint16_t y0, uint64_t sprite, 
                                int dx, int dy, uint32_t color)
{
  int oldbit, newbit;
  if ((x0 >= 0) && (x0 + 7 < width()) && (y0 >= 0) && (y0 + 7 < height()))
  {
    dx = (dx > 0) - (dx < 0);
    dy = (dy > 0) - (dy < 0);
    for (int y = 0; y < 10; y++)
    {
      for (int x = 0; x < 10; x++)
      {
        // Sprite's previous position
        int xs = x - 1, ys = y - 1;
        if (xs >= 0 && xs <= 7 && ys >= 0 && ys <= 7)
        {
          oldbit = sprite >> (63 - ys * 8 - xs) & 1;
        }
        else
          oldbit = 0;
        // Sprite's new position
        int xn = x - 1 - dx, yn = y - 1 - dy;
        if (xn >= 0 && xn <= 7 && yn >= 0 && yn <= 7)
        {
          newbit = sprite >> (63 - yn * 8 - xn) & 1;
        }
        else
          newbit = 0;
        if (oldbit != newbit)
          xorPixel(x0 - 1 + x, y0 - 1 + y, color);
      }
    }
  }
}
