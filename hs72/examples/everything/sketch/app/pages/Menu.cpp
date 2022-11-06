#include "MenuPage.h"


bool MenuPage::handleTouch(long x, long y){
    
    return false;
}

void MenuPage::draw(){
    printf("Drawing menu");
    this->drawTextCenter("Test Menu", 100, 100);
    this->m_tft->drawCircle(100, 100, 10, HX8357_WHITE);
    this->drawRectCenter(100, 100, 50, 50, HX8357_WHITE, true, 5, HX8357_BLUE);
    this->drawText(100, 150, 100, 2);
}

void MenuPage::runEachTurn(){
    if(this->getButtonReleased(b_one)){
        printf("Swithcing to balance!");
        this->setNewPage(p_balance);
    }
        
}


