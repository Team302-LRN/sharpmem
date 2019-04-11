/*
 * File:   main.c
 * Author: andruepeters
 *
 * Controller for SharpMemory Display
 * 
 * Pinout found on https://github.com/Team302-LRN/sharpmem
 */


#define _XTAL_FREQ 20000000
#define SYS_FREQ _XTAL_FREQ
#define FCY (_XTAL_FREQ/2)

#include <xc.h>
#include <stdint.h>
#include <libpic30.h>
#include "sharpmem.h"





void init_pins(void);
void set_cs(uint8_t val);
void set_d1(uint8_t val);
void set_sck(uint8_t val);

int main(void) {
    struct Adafruit_SharpMem adsm;
    int i = HEIGHT;
    int j = 0;
    adsm.rotation = 1;
    

    
    // don't exit
    while (1) {
        for (i = HEIGHT - 24;  i > 0; --i) {
            /* Prints one row of triangle */
            for (j = i; j > 0; --j) {
                ADSM_drawPixel(&adsm, i,j,1);
            }
        }
        
        __delay_ms(500);
    }
    
    return 0;
}


