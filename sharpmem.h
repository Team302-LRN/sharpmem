/*
  Andrue Peters
  3/25/19

  Driver code for Adafruit Sharp Memory Display: https://www.adafruit.com/product/3502
  Based on Adafruit_SharpMem code and adapted for PIC24.

*/

#ifndef _TEAM_302_SHARPMEM_H_
#define _TEAM_302_SHARPMEM_H_

#include "board.h"

// Width and Heigh of the display
#define WIDTH (168)
#define HEIGHT (144)

// Contains information for display, but can probably get rid of the pins variables
// beause of how PIC access works.
struct Adafruit_SharpMem {
  uint8_t ss;
  uint8_t clk;
  uint8_t mosi;
  uint32_t sharpmem_vcom;
  uint8_t sharpmem_buffer[WIDTH * HEIGHT / 8];
};



/* Adafruit_SharpMem functions */
void ADSM_init(Adafruit_SharpMem *adsm);
bool ADSM_begin(Adafruit_SharpMem *adsm);
void ADSM_drawPixel(Adafruit_SharpMem *adsm, int16_t x, int16_t y, uint16_t color);
uint8_t ADSM_getPixel(Adafruit_SharpMem *adsm, uint16_t x, uint16_t y);
void ADSM_clearDisplay(Adafruit_SharpMem *adsm);
void ADSM_refresh(Adafruit_SharpMem *adsm);
void ADSM_sendbyte(Adafruit_SharpMem *adsm, uint8_t data);
void ADSM_sendbyteLSB(Adafruit_SharpMem *adsm, uint8_t data);

#endif
