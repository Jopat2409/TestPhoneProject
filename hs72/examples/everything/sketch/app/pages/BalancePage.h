#ifndef _BALANCEPAGE_H
#define _BALANCEPAGE_H

#include "..\Page.h"
#include "XPT2046_Touchscreen.h"

class BalancePage : public Page{
public:
    BalancePage(Adafruit_HX8357* tft, XPT2046_Touchscreen* ts) : Page(tft, ts){

    }
    bool handleTouch(long x, long y);
    void draw();
    void runEachTurn();
};


#endif