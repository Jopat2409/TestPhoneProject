#include "BalancePage.h"


bool BalancePage::handleTouch(long x, long y){

    return false;
}

void BalancePage::draw(){
    printf("Drawing balance");
    this->drawTextCenter("Test Balance", 100, 100);
    this->m_tft->drawCircle(100, 100, 10, HX8357_WHITE);
}

void BalancePage::runEachTurn(){
    if(this->getButtonReleased(b_one)){
        this->setNewPage(p_menu);
    }
}


