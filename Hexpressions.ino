#include <FastLED.h>
#include "Hexpressions.h"
#define LED_PIN      2 // The pin which DIN is connected to
#define NUM_LEDS    64 // The total number of LEDs in your matrix


CRGB leds[NUM_LEDS];

/* Colour definitions
 *
 * As with matrices in Hexpressions.h, it's 
 *  recommended you comment out unused colours to 
 *  save on-board memory.
 */
CRGB HexCol   = CRGB( 128,   8, 118 );
CRGB HexGreen = CRGB(  43, 181, 129 );


/* Do not touch unless you know what you're doing and 
 * have used different hardware to the guide.
 */
void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

/* Linearises 2D matrix into 1D array compatible with 
 *  the LED matrix. Currently only accepts 1 input colour,
 *  but can easily add more using varargs or a switch statement.
 * 
 * You shouldn't have to call this function directly; use 
 *  show_pattern instead unless you have good reason not to.
 * 
 * This function will break if dimensions other than 8x8 are used.
 *  Fix this by setting ROW_SIZE and COL_SIZE. Although it still might
 *  not work. 
 */
void set_by_matrix(int matrix[ROW_SIZE][COL_SIZE], CRGB active_color) {
  CRGB Black  = CRGB(0, 0, 0);
  int row     = ROW_SIZE - 1;
  int col     = 0;
  int i       = 0;

  for (row = ROW_SIZE - 1; row >= 0; row--) {
    int invert = !(row%2);
    col = invert ? COL_SIZE - 1 : 0; 
    for ( ; col >= 0 && col < COL_SIZE; ) 
    {
      leds[i] = matrix[row][col] ? active_color : Black;
      i++;
      col += invert ? -1 : 1;
    }
  }
}

void scroll_matrix(void* pMtx, CRGB active_color, int colSize, long dly, bool loop) {
  int (*matrix)[colSize] = (int (*)[colSize]) pMtx;
  int colOffset = 0;
  int currWindow[ROW_SIZE][COL_SIZE];
  int currRow;
  int currLogicalCol, currPhysicalCol;

  
  if (loop) {
    while (loop) {
      for (colOffset = 0; colOffset < colSize; colOffset++) { 
        for (currRow = 0; currRow < ROW_SIZE; currRow++) {
          for (currLogicalCol = 0; currLogicalCol < COL_SIZE; currLogicalCol++) {
            currPhysicalCol = currLogicalCol + colOffset;
            if (currPhysicalCol > colSize) {
              currPhysicalCol-= colSize; // Wraps window around to start of scrolling range
            }
            currWindow[currRow][currLogicalCol] = matrix[currRow][currPhysicalCol];
          }
        }
        show_pattern(currWindow, active_color);
        delay(dly);
      }
    }
  } else {
    // Scan starting at column 0. Stops once last frame is fully visible.
    for (colOffset = 0; colOffset < (colSize - ROW_SIZE); colOffset++) { 
      for (currRow = 0; currRow < ROW_SIZE; currRow++) {
        for (currLogicalCol = 0; currLogicalCol < COL_SIZE; currLogicalCol++) {
          currPhysicalCol = currLogicalCol + colOffset;
          currWindow[currRow][currLogicalCol] = matrix[currRow][currPhysicalCol];
        }
      }
      show_pattern(currWindow, active_color);
      delay(dly);
    }
  }
}

/* Takes a 2D matrix pattern and a single colour, and 
 *  displays it on the connected LED display. 
 * 
 * For any given matrix node greater than 0, the corresponding 
 *  LED's colour will be set to color. For all nodes less than 
 *  or equal to zero, the LED will be turned off.
 */
void show_pattern(int matrix[8][8], CRGB color) {
  set_by_matrix(matrix, color);
  FastLED.show();
}

/* Demo function - displays Hex Drone #8483's ID number.
 * 
 * Feel free to play around with this and make it show your own
 *  ID if you have one <3
 * 
 * N.B.: The delay() function takes a number of milliseconds and 
 *  waits that amount of time before proceeding.
 */
void show_ID() {
  show_pattern(mtx_hex, HexCol);
  delay(1000);
  show_pattern(mtx_8, HexCol);
  delay(750);
  show_pattern(mtx_4, HexCol);
  delay(750);
  show_pattern(mtx_8, HexCol);
  delay(750);
  show_pattern(mtx_3, HexCol);
  delay(750);
}

/* Demo function - displays an animated spiral.
 */
void show_spiral() {
  show_pattern(mtx_spiral1, HexCol);
  delay(250);
  show_pattern(mtx_spiral2, HexCol);
  delay(250);
  show_pattern(mtx_spiral3, HexCol);
  delay(250);
  show_pattern(mtx_spiral4, HexCol);
  delay(250);
  show_pattern(mtx_spiral5, HexCol);
  delay(250);
  show_pattern(mtx_spiral6, HexCol);
  delay(250);
}

/* This is where you call functions to be executed while the 
 *  visor is powered on. 
 */
void loop() {
  scroll_matrix(&mtx_scrollable, HexCol, 48, 100, true);
}
