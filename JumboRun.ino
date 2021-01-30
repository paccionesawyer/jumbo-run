/*
 * Tufts University, Winter Break 2020-2021
 * JumboRun.ino
 * By: Sawyer Bailey Paccione
 * Completed:   January 28th, 2021
 *
 * Description: Simple Maze Runner Game built for the WIO Terminal
 * Licensed under a Creative Commons Attribution 4.0 International license: 
   http://creativecommons.org/licenses/by/4.0/
 */

/******************************************************************************
*                                   Includes                                  *
******************************************************************************/

#include <Adafruit_GFX.h> // Core graphics library
#include <TFT_eSPI.h>     // Hardware-specific library for Wio Terminal
#include <SPI.h>
#include <LIS3DHTR.h>
#include "GraphicDisplay.h"
#include "Wall.h"

/******************************************************************************
*                           WIO Terminal Definitions                          *
******************************************************************************/

#define TFT_CS        69 // Chip select
#define TFT_RST       71 // Display reset
#define TFT_DC        66 // Display data/command select
#define TFT_BACKLIGHT 72 // Display backlight pin
#define TFT_MOSI      62 // Data out
#define TFT_SCLK      63 // Clock out

const int Button1 = 30;   // Leftmost button above display
const int Speaker = 12;
const int Sens    = 1024; // Accelerometer sensitivity

const uint32_t ballCol  = TFT_WHITE;
const uint32_t boardCol = TFT_BLACK;
const uint32_t wallCol  = TFT_PINK;

const uint64_t Sprite = 0b\
00111100\
01111110\
11111111\
11111111\
11111111\
11111111\
01111110\
00111100;

const int tftWidth = 320, tftHeight = 240;
String curr_score = "0";

GraphicDisplay tft = GraphicDisplay();
Wall wallOne = Wall(wallCol);
Wall wallTwo = Wall(wallCol);
LIS3DHTR<TwoWire> lis;

/******************************************************************************
*                                   Timing                                    *
******************************************************************************/
unsigned long currTime = 0; // will store last time LED was updated
unsigned int movementSpeed = 10;

bool restartGame = false;

/******************************************************************************
*                               Main  Functions                               *
******************************************************************************/

/*
 * setup
 * Purpose:     The setup() function is called when a sketch starts. Use it to 
 *              initialize variables, pin modes, start using libraries, etc. 
 *              The setup() function will only run once, after each powerup or 
 *              reset of the Arduino board.
 * Arguments:   Nothing
 * Returns:     Nothing
 * Effects:     Initializes all values
 * Notes:       
 */
void setup()
{
  tft.init();
  tft.setRotation(3);

  tft.setTextColor(TFT_BLUE);
  tft.setTextSize(2);

  wallOne.init();
  wallOne.setRotation(3);

  wallTwo.init();
  wallTwo.setRotation(3);

  lis.begin(Wire1);

  if (!lis)
  {
    exit(1);
  }

  lis.setOutputDataRate(LIS3DHTR_DATARATE_10HZ); // Data output rate (1Hz-5kHZ)
  lis.setFullScaleRange(LIS3DHTR_RANGE_4G);      // Scale Range (2g up to 16g)

  pinMode(Button1, INPUT_PULLUP);
}

/*
 * loop
 * Purpose:     Does precisely what its name suggests, and loops consecutively,
 *              allowing your program to change and respond. Use it to actively
 *              control the Arduino board.
 * Arguments:   Nothing
 * Returns:     Nothing (void)
 * Effects:     
 * Notes:       
 */
void loop()
{
  curr_score = "0";
  wallOne.resetWall();
  wallTwo.resetWall();
  tft.fillScreen(TFT_BLACK);

  tft.drawRect(0, 0, tftWidth, tftHeight, wallCol);
  wallOne.drawWall();

  int x = 160, y = 210, dx, dy;
  tft.xorSprite(x, y, Sprite, ballCol);

  unsigned long startMillis = millis();

  unsigned long currentMillis = 0;

  restartGame = false;

  while (!restartGame)
  {
    currentMillis = millis();
    int vx, vy;
    vy = 7 * lis.getAccelerationX();
    vx = -7 * lis.getAccelerationY();

    if (currentMillis - startMillis >= (2000) 
        and currentMillis - startMillis < 2020)
    {
      wallTwo.drawWall();
    }

    // Set dx and dy to -1, 0, or +1
    int dx, dy;
    dx = (vx > 0) - (vx < 0);
    dy = (vy > 0) - (vy < 0);

    tft.setTextColor(TFT_BLACK);
    tft.drawString("Score: " + curr_score, 2, 2);
    curr_score = String((currentMillis - startMillis) / 35);
    tft.setTextColor(TFT_BLUE);
    tft.drawString("Score: " + curr_score, 2, 2);

    if (tft.hitSprite(x + dx, y, Sprite, wallCol))
    {
      dx = 0;
    }
    if (tft.hitSprite(x, y + dy, Sprite, wallCol))
    {
      dy = 0;
    }
    if (tft.hitSprite(x, y - 2, Sprite, wallCol) 
        or tft.hitSprite(x, y - 1, Sprite, wallCol))
    {
      dy = 1;
      if (tft.hitSprite(x, y + 1, Sprite, wallCol))
      {
        gameOver("SCORE: " + curr_score);
      }
    }

    tft.moveSprite(x, y, Sprite, dx, dy, ballCol);
    wallOne.moveWall();
    if (currentMillis - startMillis >= 3000)
    {
      wallTwo.moveWall();
    }
    x = x + dx;
    y = y + dy;
    delay(movementSpeed);
  }
}

/******************************************************************************
*                              Helper  Functions                              *
******************************************************************************/

/*
 * gameOver
 * Purpose:     Draws gameover to the screen and the screen
 * Arguments:   score [String] The users final score
 * Returns:     Nothing (void)
 * Effects:     The WIO Terminal Screen
 * Notes:       
 */
void gameOver(String score)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  String msg = "GAMEOVER";
  typeString(msg, 70, 100);

  tft.setTextSize(2);
  typeString(score, 70, 160);
  typeString("RESET", 1, 1);
  delay(100);

  unsigned long currTime = millis();

  while (digitalRead(Button1))
  {
    if (currTime % 1500 < 750)
    {
      if (tft.readPixel(0, 0) == TFT_BLACK)
      {
        tft.fillRect(0, 0, 61, 16, TFT_BLUE);
        tft.setTextColor(TFT_BLACK);
        tft.drawString("RESET", 1, 1);
      }
    }
    else
    {
      if (tft.readPixel(0, 0) == TFT_BLUE)
      {
        tft.fillRect(0, 0, 61, 16, TFT_BLACK);
        tft.setTextColor(TFT_BLUE);
        tft.drawString("RESET", 1, 1);
      }
    }
    currTime = millis();
  }
  restartGame = true;
}

/*
 * typeString
 * Purpose:     Type a given string out with each letter delayed
 * Arguments:   to_type [String]  The String to be printed
 *              x_start [int]     The x postion of the string
 *              y_start [int]     The y position of the string
 * Returns:     Nothing (void)
 * Effects:     The WIO Terminal Screen
 * Notes:       
 */
void typeString(String to_type, int x_start, int y_start)
{
  String to_print = "";
  for (int i = 0; i < to_type.length(); ++i)
  {
    to_print += to_type[i];
    tft.drawString(to_print, x_start, y_start);
    delay(150);
  }
}
