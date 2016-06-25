
/**
 * This display is a take on the work game slither.io
 * 
 * The work is not allowed to swirm off of the display.  The worm
 * looks for a locaiton on the board if it is against a wall.
 * 
 * 
 *  Required Libraries:
 *  
 *    Use Adafruit NeoMatrix version 1.1.0
 * 
 */

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "Worm.h"

#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 15

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

const int width = 8;
const int height = 4;

// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(width, height, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

Worm worm = Worm();

const uint16_t colors[] = 
{
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) 
};

//const int maxSegments = 10;

/**
 * The elements at segmentLocations[0][0] and  segmentLocations[0][1] are the (x,y) coordinates of the head of the worm.
 */
//int segmentLocations [maxSegments][2] = {0};

int validMovesCount;

/**
 * This two dimentional array holds the x,y coordinates for a total of 4 possible move locations; up, down, left, and right.
 */
volatile int validMoves [4][2];

void drawWorm()
{
    // go over each worm segment
    for(int i=0; i<worm.length; i++)
    {        
        int x = worm.segmentLocations[i][0];
        int y = worm.segmentLocations[i][1];

        matrix.drawPixel(x,y, colors[2]);
    }
}

void loop() 
{
  matrix.fillScreen(0);

  moveWorm();
  
  drawWorm();
  
  matrix.show();
  
  delay(500);
}

void moveWorm()
{
    for(int i=worm.length-1; i>0; i--)
    {
        worm.segmentLocations[i][0] = worm.segmentLocations[i-1][0];
        worm.segmentLocations[i][1] = worm.segmentLocations[i-1][1];
    }

    updateValidMoves();

    int m = random(0, validMovesCount);

    int x = validMoves[m][0];
    int y = validMoves[m][1];

    worm.segmentLocations[0][0] = x;
    worm.segmentLocations[0][1] = y;
}

void setup() 
{
  matrix.begin();

  matrix.setBrightness(5);
}

void updateValidMoves()
{
    int headX = worm.segmentLocations[0][0];
    int headY = worm.segmentLocations[0][1];

    int i = 0;

    // up
    if(headY > 0)
    {
        validMoves[i][1] = headY - 1;
        validMoves[i][0] = headX;
        
        i++;
    }

    // down
    if(headY < height-1)
    {
        validMoves[i][1] = headY + 1;
        validMoves[i][0] = headX;

        i++;
    }
    
    // left
    if(headX > 0)
    {
        validMoves[i][0] = headX - 1;
        validMoves[i][1] = headY;

        i++;
    }

    // right
    if(headX < width-1)
    {
        validMoves[i][0] = headX + 1;
        validMoves[i][1] = headY;

        i++;
    }

    validMovesCount = i;
}
