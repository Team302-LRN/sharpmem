/*
  Andrue Peters
  3/25/19

*/

#include "sharpmem.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif
#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b) { uint16_t t = a; a = b; b = t; }
#endif

/**************************************************************************
    Sharp Memory Display Connector
    -----------------------------------------------------------------------
    Pin   Function        Notes
    ===   ==============  ===============================
      1   VIN             3.3-5.0V (into LDO supply)
      2   3V3             3.3V out
      3   GND
      4   SCLK            Serial Clock
      5   MOSI            Serial Data Input
      6   CS              Serial Chip Select
      9   EXTMODE         COM Inversion Select (Low = SW clock/serial)
      7   EXTCOMIN        External COM Inversion Signal
      8   DISP            Display On(High)/Off(Low)

 **************************************************************************/

#define SHARPMEM_BIT_WRITECMD   (0x80)
#define SHARPMEM_BIT_VCOM       (0x40)
#define SHARPMEM_BIT_CLEAR      (0x20)
#define TOGGLE_VCOM             do { _sharpmem_vcom = _sharpmem_vcom ? 0x00 : SHARPMEM_BIT_VCOM; } while(0);


bool ADSM_begin(Adafruit_SharpMem *adsm)
{
  // SS = HIGH
  // CLK = LOW
  // MOSI = HIGH

  // ss = OUTPUT
  // clk = OUTPUT
  // MOSI = OUTPUT


  adsm->sharpmem_vcom = SHARPMEM_BIT_VCOM;
  return true;
}

void ADSM_sendbyte(Adafruit_SharpMem *adsm, uint8_t data)
{
  uint8_t i;
  for (i = 0; i < 8; ++i) {
    // CLK = LOW
    if (data & 0x80) {
      // MOSI = HIGH
    } else {
      // MOSI = LOW
    }

    // CLK = HIGH
    data <<= 1;
  }
  // Make sure clock ends low
  // CLK = LOW
}

void ADSM_sendbyteLSB(Adafruit_SharpMem *adsm, uint8_t data)
{
  uint8_t i;
  for (i = 0; i < 8; ++i) {
    // CLK = LOW
    if (data & 0x01) {
      // MOSI = HIGH
    } else {
      MOSI = LOW;
    }
    // clock is active high
    // CLK = HIGH
    data >>= 1;
  }
  // Make sure clock ends low
  // CLK = LOW
}

// 1<<n is a costly operation on AVR -- table usu. smaller & faster
static const uint8_t PROGMEM
  set[] = {  1,  2,  4,  8,  16,  32,  64,  128 },
  clr[] = { (uint8_t)~1 , (uint8_t)~2 , (uint8_t)~4 , (uint8_t)~8,
            (uint8_t)~16, (uint8_t)~32, (uint8_t)~64, (uint8_t)~128 };

void ADSM_drawPixel(Adafruit_SharpMem *adsm, int16_t x, int16_t y, uint16_t color)
{
  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

  switch(rotation) {
   case 1:
    _swap_int16_t(x, y);
    x = WIDTH  - 1 - x;
    break;
   case 2:
    x = WIDTH  - 1 - x;
    y = HEIGHT - 1 - y;
    break;
   case 3:
    _swap_int16_t(x, y);
    y = HEIGHT - 1 - y;
    break;
  }

  if(color) {
    adsm->sharpmem_buffer[(y * WIDTH + x) / 8] |= set[x & 7];
  } else {
    adsom->sharpmem_buffer[(y * WIDTH + x) / 8] &= clr[x & 7];
  }
}


uint8_t ADSM_getPixel(Adafruit_SharpMem *adsm, uint16_t x, uint16_t y)
{
  if((x >= _width) || (y >= _height)) return 0; // <0 test not needed, unsigned

  switch(rotation) {
   case 1:
    _swap_uint16_t(x, y);
    x = WIDTH  - 1 - x;
    break;
   case 2:
    x = WIDTH  - 1 - x;
    y = HEIGHT - 1 - y;
    break;
   case 3:
    _swap_uint16_t(x, y);
    y = HEIGHT - 1 - y;
    break;
  }

  return adsm->sharpmem_buffer[(y * WIDTH + x) / 8] & set[x & 7] ? 1 : 0;
}

void ADSM_clearDisplay(Adafruit_SharpMem *adsm)
{
  memset(adsm->sharpmem_buffer, 0xff, (WIDTH * HEIGHT) / 8);
  // Send the clear screen command rather than doing a HW refresh (quicker)
  // SS = HIGH
  sendbyte(adsm->sharpmem_vcom | SHARPMEM_BIT_CLEAR);
  sendbyteLSB(0x00);
  TOGGLE_VCOM;
  // SS = LOW
}

void ADSM_refresh(Adafruit_SharpMem *adsm)
{
  uint16_t i, totalbytes, currentline, oldline;
  totalbytes = (WIDTH * HEIGHT) / 8;

  // Send the write command
  // SS = HIGH
  sendbyte(SHARPMEM_BIT_WRITECMD | adsm->sharpmem_vcom);
  TOGGLE_VCOM;

  // Send the address for line 1
  oldline = currentline = 1;
  sendbyteLSB(currentline);

  // Send image buffer
  for (i=0; i<totalbytes; i++)
  {
    sendbyteLSB(adsm->sharpmem_buffer[i]);
    currentline = ((i+1)/(WIDTH/8)) + 1;
    if(currentline != oldline)
    {
      // Send end of line and address bytes
      sendbyteLSB(0x00);
      if (currentline <= HEIGHT)
      {
        sendbyteLSB(currentline);
      }
      oldline = currentline;
    }
  }

  // Send another trailing 8 bits for the last line
  sendbyteLSB(0x00);
  // SS = LOW
}
