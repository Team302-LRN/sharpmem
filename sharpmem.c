/*
  Andrue Peters
  3/25/19

*/

#include "sharpmem.h"
#include <string.h> // memset
#include <xc.h>
#include <stdint.h>



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


 /*
  | Display Pin  | Schematic Name    |   PIC Pin  |
  | ------------ | ----------------- | ---------- |
  | CS           | SS1               | RB5        |
  | D1           | SDI               | RB3        |
  | SCK          | SCK               | RB2        |
*/





void TOGGLE_VCOM(struct Adafruit_SharpMem *adsm) 
{
    adsm->sharpmem_vcom = adsm->sharpmem_vcom ? 0x00 : SHARPMEM_BIT_VCOM;
}

static const unsigned char BitReverseTable256[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

// 1<<n is a costly operation on AVR -- table usu. smaller & faster
static const uint8_t 
  set[] = {  1,  2,  4,  8,  16,  32,  64,  128 },
  clr[] = { (uint8_t)~1 , (uint8_t)~2 , (uint8_t)~4 , (uint8_t)~8,
            (uint8_t)~16, (uint8_t)~32, (uint8_t)~64, (uint8_t)~128 };

bool ADSM_begin(struct Adafruit_SharpMem *adsm)
{
  adsm->sharpmem_vcom = SHARPMEM_BIT_VCOM;
  return true;
}

void ADSM_sendbyte(struct Adafruit_SharpMem *adsm, uint8_t data)
{
    SPI1_Exchange8bit(data);
}

void ADSM_sendbyteLSB(struct Adafruit_SharpMem *adsm, uint8_t data)
{
  uint8_t data_lsb = BitReverseTable256[data];
  ADSM_sendbyte(adsm, data_lsb);
}



void ADSM_drawPixel(struct Adafruit_SharpMem *adsm, int16_t x, int16_t y, uint16_t color)
{
  //if((x < 0) || (x >= WIDTH) || (y < 0) || (y >= WIDTH)) return;

  switch(adsm->rotation) {
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
    adsm->sharpmem_buffer[(y * WIDTH + x) / 8] &= clr[x & 7];
  }
}


uint8_t ADSM_getPixel(struct Adafruit_SharpMem *adsm, uint16_t x, uint16_t y)
{
  if((x >= WIDTH) || (y >= HEIGHT)) return 0; // <0 test not needed, unsigned

  switch(adsm->rotation) {
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

void ADSM_clearDisplay(struct Adafruit_SharpMem *adsm)
{
  memset(adsm->sharpmem_buffer, 0xff, (WIDTH * HEIGHT) / 8);
  // Send the clear screen command rather than doing a HW refresh (quicker)
  set_ss(_HIGH_);
  ADSM_sendbyte(adsm, adsm->sharpmem_vcom | SHARPMEM_BIT_CLEAR);
  ADSM_sendbyteLSB(adsm, 0x00);
  TOGGLE_VCOM(adsm);
  set_ss(_LOW_);
}

void ADSM_refresh(struct Adafruit_SharpMem *adsm)
{
    int i = 0;
    for (i = 0; i < HEIGHT; ++i) {
        ADSM_updateLine(adsm, i);
    }
}

void ADSM_updateLine(struct Adafruit_SharpMem *adsm, uint16_t line)
{
    int i, numlines, curr_line;
    uint8_t *data; 
    numlines = ((i+1)/(WIDTH/8)) + 1;
    data = &adsm->sharpmem_buffer[((WIDTH * line) / 8)];
    
    
    /* Send write command */
    set_ss(_HIGH_);
    ADSM_sendbyte(adsm, SHARPMEM_BIT_WRITECMD | adsm->sharpmem_vcom);
    TOGGLE_VCOM(adsm);
    
    /* Send address for line */
    ADSM_sendbyteLSB(adsm, line + 1);
    
    /* Send data for the selected line */
    for (i = 0; i < WIDTH/8; ++i) {
        ADSM_sendbyteLSB(adsm, *data);
        ++data;
    }
    
    ADSM_sendbyte(adsm, 0x00);
    ADSM_sendbyte(adsm, 0x00);
    set_ss(_LOW_);
}