/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system intialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.95-b-SNAPSHOT
        Device            :  PIC24FJ256GA702
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36
        MPLAB 	          :  MPLAB X v5.10
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/clock.h"
#include "mcc_generated_files/pin_manager.h"
#include "sharpmem.h"
#include <libpic30.h>
#include <xc.h>
#include <stdint.h>
#include <string.h>

#include "ugui.h"
#include "disp_obj.h"



UG_GUI _gui;
extern struct Adafruit_SharpMem adsm;
/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    /* Set up Pins*/
    PIN_MANAGER_Initialize();

    
    set_ss(_LOW_);
    SPI1_Initialize();

    
    ADSM_begin(&adsm);
    ADSM_clearDisplay(&adsm);
    
    __delay_ms(500);
    adsm.rotation = 0;
    int i;
    
     memset(adsm.sharpmem_buffer, 0x00, (WIDTH * HEIGHT) / 8);
     ADSM_refresh(&adsm);
     __delay_ms(500);
     ADSM_clearDisplay(&adsm);
    
    dispInit();
    UG_FontSelect(&FONT_6X8);
    UG_SetBackcolor(C_WHITE);
    UG_SetForecolor(C_BLACK);
    while (1)
    {
        UG_PutString(50,50, "Hello World!");
      //  UG_DrawLine(0,0, 50,50, C_BLACK);
        UG_Update();
        for (i = 0; i < HEIGHT; ++i) {
            ADSM_updateLine(&adsm, i);
        }
        __delay_ms(500);
        led_Toggle();
    }

    return 1;
}
/**
 End of File
*/

