#include "board.h"
#include <xc.h>

/*
  Sets pin equal to val.
  val should be 0 or 1
*/
void set_cs(uint8_t val)
{
    PORTBbits.RB5 = val;
}

void set_d1(uint8_t val)
{
    PORTBbits.RB3 = val;
}

void set_sck(uint8_t val)
{
    PORTBbits.RB2 = val;
}

/*
  Configure CS pin as output.
  CS is RB5, which has no analog select,
  so no need to disable.
*/
void cs_config_out(void)
{
  TRISBbits.TRISB5 = 0;
}

/*
  Configure D1 pin as output.
  D1 is on RB3. Disable analog and select digital out.
*/
void d1_config_out(void)
{
  ANSBbits.ANSB3 = 0;
  TRISBbits.TRISB3 = 0;
}

/*
  Configure SCK pin as output.
  SCK is on RB2. Disable analog and select digital out.
*/
void sck_config_out(void)
{
  ANSBbits.ANSB2 = 0;
  TRISBbits.TRISB3 = 0;
}
