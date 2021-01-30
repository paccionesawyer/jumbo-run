/*
 * Tufts University, Winter Break 2020-2021
 * Wall.h
 * By: Sawyer Bailey Paccione
 * Completed:   January 28th, 2021
 *
 * Description: A wall will start at the top of the screen and travel 
 *              vertically to the bottom of the screen. It prevents the 
 *              player's sprite from passing it.
 */

#ifndef Wall_h
#define Wall_h

#include <TFT_eSPI.h> // Hardware-specific library for Wio Terminal
#include "Arduino.h"

class Wall : public TFT_eSPI
{
public:
  Wall();
  Wall(uint32_t color);
  void drawWall();
  void moveWall();
  void setHeight(int desHeight);
  void resetWall();

private:
  const int THICKNESS = 15; //pixels
  static const int MAX_HOLES = 3;
  static const int TFTWIDTH = 320;
  static const int TFTHEIGHT = 240;
  static const int HOLESIZE = 20;

  void randomHoles();
  void drawWallLine(int new_bot);
  void eraseWallLine(int old_top);
  void saveLine();

  int y;

  uint32_t wallCol;

  // left-most x-comp of a 35 pixel long hole
  int holes[MAX_HOLES];
  int lineColors[TFTWIDTH - 2];
};

#endif
