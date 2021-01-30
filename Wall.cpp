/*
 * Tufts University, Winter Break 2020-2021
 * Wall.cpp
 * By: Sawyer Bailey Paccione
 * Completed:   January 28th, 2021
 * 
 * Description: A wall will start at the top of the screen and travel 
 *              vertically to the bottom of the screen. It prevents the 
 *              player's sprite from passing it.
 */

#include "Wall.h"

/******************************************************************************
*                               Public  Members                               *
******************************************************************************/

/*
 * Basic Constructor
 * Purpose:     Build a wall and set it up with basic information
 * Arguments:   N/A
 * Returns:     Nothing
 * Effects:     Allocates space for a Wall
 * Notes:       The parameterized Constructor below should be used when
 *              possible.
 */
Wall::Wall() : TFT_eSPI()
{
   this->y = 1;

   for (int i = 0; i < this->MAX_HOLES; i++)
   {
      this->holes[i] = -36;
   }

   for (int i = 0; i < (this->TFTWIDTH - 2); ++i)
   {
      lineColors[i] = 0;
   }

   this->wallCol = TFT_PINK;
}

/*
 * Parameterized Constructor
 * Purpose:    Build a wall with 3 random holes of the desired color 
 * Arguments:  color [int] The desired TFT COLOR 
 * Returns:    Nothing
 * Effects:    Allocates space for a wall.
 * Notes:      
 */
Wall::Wall(uint32_t color) : TFT_eSPI()
{
   this->y = 1;
   this->randomHoles();

   for (int i = 0; i < (this->TFTWIDTH - 2); ++i)
   {
      lineColors[i] = 0;
   }

   this->wallCol = color;
}

/*
 * drawWall
 * Purpose:     Draw a wall at its starting point 
 * Arguments:   N/A
 * Returns:     Nothing
 * Effects:     WIO Terminal Screen
 * Notes:       Only used at first, after that moving the wall also 
 *              draws the wall.
 */
void Wall::drawWall()
{
   this->fillRect(1, this->y, TFTWIDTH - 1, this->THICKNESS, wallCol);

   for (int i = 0; i < MAX_HOLES; i++)
   {
      if (this->holes[i] > -35 and this->holes[i] < this->TFTWIDTH)
      {
         this->fillRect(holes[i], this->y, this->HOLESIZE, this->THICKNESS, 
                        TFT_BLACK);
      }
   }
   saveLine();
}

/*
 * moveWall
 * Purpose:     Move the wall one pixel down by erasing the top line and 
 *              drawing an additional one at the bottom  
 * Arguments:   Nothing
 * Returns:     Nothing
 * Effects:     WIO Terminal Screen and this->y
 * Notes:       To get this to work correctly I had to only delete the line
 *              where there was a wall. (I couldn't just draw a horizontal
 *              black line because it would delete the sprite as well when
 *              passing) the same is true of drawing the line at the bottom.
 */
void Wall::moveWall()
{
   if (y > 0)
   {

      if (y < TFTHEIGHT - 1)
      {
         this->eraseWallLine(y);
      }
      else
      {
         this->resetWall();
      }
      y++;
      if (y + THICKNESS < TFTHEIGHT)
      {
         this->drawWallLine(y + THICKNESS - 1);
      }
   }
}

/*
 * resetWall
 * Purpose:    Returns the wall to the top of the screen
 * Arguments:  Nothing 
 * Returns:    Nothing (void)
 * Effects:    WIO Terminal Screen
 * Notes:       
 */
void Wall::resetWall()
{
   this->y = 2;
   this->randomHoles();

   for (int i = 0; i < (this->TFTWIDTH - 2); ++i)
   {
      lineColors[i] = 0;
   }

   drawWall();
   this->drawFastHLine(1,this->y,318,TFT_BLACK);
}

/*
 * setHeight
 * Purpose:    Sets the new height of the wall
 * Arguments:  desHeight [int] The new desired y location of the wall
 * Returns:    Nothing (void)
 * Effects:    WIO Terminal
 * Notes:      
 */
void Wall::setHeight(int desHeight)
{
   y = desHeight;
}

/******************************************************************************
*                               Private Members                               *
******************************************************************************/

/*
 * saveLine
 * Purpose:     Save what a single horizontal line looks like in the wall.
 * Arguments:   Nothing
 * Returns:     Nothing (void)
 * Effects:     linesColors
 * Notes:       This occurs after the holes have been assigned and is called
 *              in drawWall(). Will save a 1 if there is a wall at that pixel.
 */
void Wall::saveLine()
{
   for (int i = 0; i < this->TFTWIDTH - 2; ++i)
   {
      if (readPixel(i + 1, y) == TFT_PINK)
      {
         lineColors[i] = 1;
      }
   }
}

/*
 * randomHoles
 * Purpose:     Randomly assign the x-pos of the start of holes
 * Arguments:   Nothing
 * Returns:     Nothing (void)
 * Effects:     The location of the holes in a wall
 * Notes:         
 */
void Wall::randomHoles()
{
   randomSeed(analogRead(0));
   for (int i = 0; i < this->MAX_HOLES; i++)
   {
      this->holes[i] = random(1, this->TFTWIDTH - HOLESIZE - 1);
   }
}


/*
 * drawWallLine
 * Purpose:     Draw a single wall line at the desired y level
 * Arguments:   new_bot [int]    The y location of the new wall line
 * Returns:     Nothing (void)
 * Effects:     WIO Terminal Screen
 * Notes:       
 */
void Wall::drawWallLine(int new_bot)
{
   for (int i = 0; i < this->TFTWIDTH - 2; ++i)
   {
      if (lineColors[i] == 1)
      {
         this->drawPixel(i + 1, new_bot, wallCol);
      }
   }
}

/*
 * eraseWallLine
 * Purpose:     Erases the wall only at the pixel where a wall is.
 * Arguments:   old_top [int] the y location of the top of the wall before move
 * Returns:     Nothing (void)
 * Effects:     WIO Terminal Screen
 * Notes:       
 */
void Wall::eraseWallLine(int old_top)
{
   for (int i = 0; i < this->TFTWIDTH - 2; ++i)
   {
      if (lineColors[i] == 1)
      {
         this->drawPixel(i + 1, old_top, TFT_BLACK);
      }
   }
}
