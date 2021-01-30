/*
 * Tufts University, Winter Break 2020-2021
 * GraphicDisplay.h
 * By: Sawyer Bailey Paccione
 * Completed:   January 28th, 2021
 *
 * Description: A series of logical statement for displaying pixels on a
 *              WIO Terminal made by SEEED.
 * Notes:       Adapted from http://www.technoblogy.com/show?39DR (Liscenced 
 *              Under http://creativecommons.org/licenses/by/4.0/)
 */

#ifndef GraphicDisplay_h
#define GraphicDisplay_h

#include <TFT_eSPI.h> // Hardware-specific library for Wio Terminal
#include "Arduino.h"

class GraphicDisplay : public TFT_eSPI
{
public:
  GraphicDisplay();
  uint16_t getPixel(uint16_t x, uint16_t y);
  void xorPixel(uint16_t x, uint16_t y, uint32_t color);
  void xorSprite(uint16_t x, uint16_t y, uint64_t sprite, uint32_t color);
  bool hitSprite(uint16_t x, uint16_t y, uint64_t sprite, uint32_t color);
  void moveSprite(uint16_t x, uint16_t y, uint64_t sprite, int dx, int dy,
                  uint32_t color);
};

#endif
