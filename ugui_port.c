#include "ugui_port.h"
#include "sharpmem.h"

struct Adafruit_SharpMem adsm;
void drawPixelPort(UG_S16 x, UG_S16  y, UG_COLOR c)
{
    ADSM_drawPixel(&adsm, x, y, c);
}

void dispInit()
{
    UG_Init(&_gui, drawPixelPort, HEIGHT, WIDTH);
    ADSM_begin(&adsm);
}

void refreshDisp()
{
    ADSM_refresh(&adsm);
}