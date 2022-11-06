/*
Base page for our app. Should have basic functions to draw text etc.
Because the original Ui elements were filled with 
*/
#ifndef _PAGE_H
#define _PAGE_H

// For colors
#include "Adafruit_HX8357.h"
#include "XPT2046_Touchscreen.h"
#include "Types.h"


class Page{
public:
    Page (Adafruit_HX8357* tft, XPT2046_Touchscreen* ts) : m_tft(tft), m_ts(ts){
    };
    virtual bool handleTouch(long x, long y) = 0;
    virtual void draw() = 0;
    virtual void runEachTurn() = 0;

    bool getShouldSwap() {return this->shouldSwap; };
    page_t getSwapTo() { return this->swapTo; };

    void updateButtons();
private:
    bool checkButton(button_t buttonToCheck);

protected:
    void drawText(const char* textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize = 1, uint16_t color = HX8357_WHITE);
    void drawTextCenter(const char* textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize = 1, uint16_t color = HX8357_WHITE);
    void drawText(int textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize = 1, uint16_t color = HX8357_WHITE);
    void drawTextCenter(int textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize = 1, uint16_t color = HX8357_WHITE);
    void drawText(double textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize = 1, uint16_t color = HX8357_WHITE);
    void drawTextCenter(double textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize = 1, uint16_t color = HX8357_WHITE);

    void drawRect(uint16_t xPos, uint16_t yPos, uint8_t width, uint8_t height, uint16_t color, bool fill=false, uint16_t borderWidth=0, uint16_t borderColor=HX8357_BLACK);
    void drawRectCenter(uint16_t xPos, uint16_t yPos, uint8_t width, uint8_t height, uint16_t color, bool fill=false, uint16_t borderWidth=0, uint16_t borderColor=HX8357_BLACK);

    void setNewPage(page_t newPage);

    bool getButtonPressed(button_t buttonToCheck);
    bool getButtonReleased(button_t buttonToCheck);
    bool getButtonHeld(button_t buttonToCheck);

    Adafruit_HX8357* m_tft;
    XPT2046_Touchscreen* m_ts;
    // callbacks are nasty in cpp so settings variables will suffice :)
    bool shouldSwap{false};
    page_t swapTo;

    // pressed, held released respectively
    bool buttonStatus[3][3] {{false, false, false},
                            {false, false, false},
                            {false, false, false}};
};

#endif