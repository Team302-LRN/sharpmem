/* 
 * File:   disp_obj.h
 * Author: andruepeters
 *
 * Created on April 16, 2019, 11:02 AM
 * 
 * This file is a wrapper for the display.
 * The main file only needs call dispInit() and refreshDisp().
 * 
 * drawPixelPort is a wrapper for the pixel set function for ugui.h
 */

#ifndef DISP_OBJ_H
#define	DISP_OBJ_H
#include "sharpmem.h"
#include "ugui.h"

/* Global context for sharp memory display */
extern struct Adafruit_SharpMem adsm;
extern UG_GUI _gui;

void drawPixelPort(UG_S16 x, UG_S16  y, UG_COLOR c);
void refreshDisp();
void dispInit();


#endif	/* DISP_OBJ_H */

