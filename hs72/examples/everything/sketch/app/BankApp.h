/*
    Main application class for the paypal shit
*/
#ifndef _BANKAPP_H
#define _BANKAPP_H

#include "pages\MenuPage.h"
#include "pages\BalancePage.h"
#include "..\UIController.h"

class BankApp : public UIElement{
public:
    BankApp(Adafruit_HX8357* tft, XPT2046_Touchscreen* ts, SdFat* sd)
        : UIElement(tft, ts, sd) {
            this->currentPageID = p_menu;
            this->currentPage = new MenuPage(this->m_tft, this->m_ts);
    }
    void setCurrentPage(page_t newPage);
    ~BankApp();
    bool handleTouch(long x, long y);
    void draw();
    void runEachTurn();
private:
    // uh oh manual memory
    Page* currentPage;
    page_t currentPageID;
};



#endif