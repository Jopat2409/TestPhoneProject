#include "BankApp.h"

BankApp::~BankApp(){
    // pray no memory leakage
    delete this->currentPage;
}


void BankApp::setCurrentPage(page_t newPage){
    // Dont bother swapping if we are for some reason switching to the same page
    if(newPage == this->currentPageID) {
        return;
    }
    // pray no memory leakage
    delete this->currentPage;
    
    switch(newPage){
        case p_balance:
            D("MAde it this far to the balance page!");
            this->currentPage = new BalancePage(this->m_tft, this->m_ts); break;
        case p_paymerchant:
            //TODO add merchant page
            this->currentPage = new MenuPage(this->m_tft, this->m_ts); break;
        case p_payuser:
            //TODO add pay user page
            this->currentPage = new MenuPage(this->m_tft, this->m_ts); break;
        case p_menu:
            D("About to go back to the menu!");
            this->currentPage = new MenuPage(this->m_tft, this->m_ts); break;
        default:
            D("Defaulted... probably not good");
            this->currentPage = new MenuPage(this->m_tft, this->m_ts); break;
    }
    this->currentPageID = newPage;
    this->draw();
}

bool BankApp::handleTouch(long x, long y){
    // 10 million better ways to handle this but it is 3am
    return this->currentPage->handleTouch(x, y);
}

void BankApp::draw(){
    this->m_tft->fillScreen(HX8357_BLACK);
    // 10 million better ways to handle this but it is 3am
    this->currentPage->draw();
}

void BankApp::runEachTurn(){
    this->currentPage->updateButtons();
    // 10 million better ways to handle this but it is 3am
    this->currentPage->runEachTurn();
    // Check to see if we need to switch pages
    if(this->currentPage->getShouldSwap()){
        D("Detected should swap page..");
        this->setCurrentPage(this->currentPage->getSwapTo());
    }
}