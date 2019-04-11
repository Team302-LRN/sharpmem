#ifndef _TEAM_302_BOARD_H_
#define _TEAM_302_BOARD_H_

#include <stdint.h>
/*
  Just define these functions to use this driver.
*/

/*
  Sets pin equal to val.
  val should be 0 or 1
*/
void set_ss(uint8_t val);
void set_mosi(uint8_t val);
void set_clk(uint8_t val);

/*
  Configure pins as output.
*/
void ss_config_out(void);
void mosi_config_out(void);
void clk_config_out(void);
#endif
