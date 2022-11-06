#ifndef _MENUPAGE_H
#define _MENUPAGE_H

#include "..\Page.h"
#include "XPT2046_Touchscreen.h"

class MenuPage : public Page{
public:
    MenuPage(Adafruit_HX8357* tft, XPT2046_Touchscreen* ts) : Page(tft, ts){

    }
    bool handleTouch(long x, long y);
    void draw();
    void runEachTurn();
};


#endif