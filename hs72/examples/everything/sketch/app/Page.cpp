#include "Page.h"
#include "unphone.h"

/**
    * Draws text to the current page
    * @param textValue String of text to be printed to the screen
    * @param xPos X position of top-left corner of text
    * @param yPos Y position of top-left corner of text
    * @param textSize Relative size of text. 1 is equal to a char size of 6x8 and each incrememnt doubles the dimensions.
    * @param color Color of the text to be drawn
    * @return None
    */
void Page::drawText(const char* textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize, uint16_t color){
    
    // Dimensions of text to be rendered
    int16_t xL, yL = 0;
    uint16_t width, height = 0;
    // Set text size of whole canvas (for calculation reasons) and get size information
    this->m_tft->setTextSize(textSize);
    this->m_tft->getTextBounds(textValue, xPos, yPos, 
                                &xL, &yL, 
                                    &width, &height);
    // Create Canvas the size of the text and change the canvas-specific text size.
    GFXcanvas1 textCanvas(width, height);
    textCanvas.setTextSize(textSize);
    // Draw text to canvas and draw canvas to main surface
    textCanvas.println(textValue);
    this->m_tft->drawBitmap(xPos, yPos, 
                            textCanvas.getBuffer(), width, height, color);
}


// Various function overloads for drawing text of different values.
void Page::drawText(int textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize, uint16_t color){
    this->drawText(String(textValue).c_str(), xPos, yPos, textSize, color);
}
void Page::drawTextCenter(int textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize, uint16_t color){
    this->drawTextCenter(String(textValue).c_str(), xPos, yPos, textSize, color);
}
void Page::drawText(double textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize, uint16_t color){
    this->drawText(String(textValue).c_str(), xPos, yPos, textSize, color);
}
void Page::drawTextCenter(double textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize, uint16_t color){
    this->drawTextCenter(String(textValue).c_str(), xPos, yPos, textSize, color);
}

/**
    * Draws text to the current page with the center being at the coordinates desired. Surely no repeated code here...
    * @param textValue String of text to be printed to the screen
    * @param xPos X position of center of the text
    * @param yPos Y position of center of the text
    * @param textSize Relative size of text. 1 is equal to a char size of 6x8 and each incrememnt doubles the dimensions.
    * @param color Color of the text to be drawn
    * @return None
    */
void Page::drawTextCenter(const char* textValue, uint16_t xPos, uint16_t yPos, uint8_t textSize, uint16_t color){
    // Dimensions of text to be rendered
    int16_t xL, yL = 0;
    uint16_t width, height = 0;
    // Set text size of whole canvas (for calculation reasons)
    this->m_tft->setTextSize(textSize);
    // Get the bounds of the text to be drawn and store them in dimensions
    this->m_tft->getTextBounds(textValue, xPos, yPos, 
                                &xL, &yL, 
                                    &width, &height);
    // Create Canvas the size of the text and change the canvas-specific text size.
    GFXcanvas1 textCanvas(width, height);
    textCanvas.setTextSize(textSize);
    // Draw text to canvas and draw canvas to main surface
    textCanvas.println(textValue);
    this->m_tft->drawBitmap(xPos-width/2, yPos-height/2, 
                            textCanvas.getBuffer(), width, height, color);
}

/**
 * @brief Draws a rectangle with various different rendering options
 * 
 * @param xPos X position of the top left corner
 * @param yPos Y position of the top left corner
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Colour of the rectangle
 * @param fill Should the rectangle be filled (will use color for the fill)
 * @param borderWidth Width of the border (only works when fill is set to true)
 * @param borderColor Colour of the border
 */
void Page::drawRect(uint16_t xPos, uint16_t yPos, uint8_t width, uint8_t height, uint16_t color, bool fill, uint16_t borderWidth, uint16_t borderColor){
    if (!fill)
        this->m_tft->drawRect(xPos, yPos, width, height, color);
    else{
        if(borderWidth)
            this->m_tft->fillRect(xPos-borderWidth, yPos-borderWidth, width+borderWidth*2, height+borderWidth*2, borderColor);
        this->m_tft->fillRect(xPos, yPos, width, height, color);
    }
}


/**
 * @brief Draws a rectangle with various different rendering options
 * 
 * @param xPos X position of the center of the rectangle
 * @param yPos Y position of the center of the rectangle
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Colour of the rectangle
 * @param fill Should the rectangle be filled (will use color for the fill)
 * @param borderWidth Width of the border (only works when fill is set to true)
 * @param borderColor Colour of the border
 */
void Page::drawRectCenter(uint16_t xPos, uint16_t yPos, uint8_t width, uint8_t height, uint16_t color, bool fill, uint16_t borderWidth, uint16_t borderColor){
    xPos = xPos - width/2;
    yPos = yPos - height/2;
    if (!fill)
        this->m_tft->drawRect(xPos, yPos, width, height, color);
    else{
        if(borderWidth)
            this->m_tft->fillRect(xPos-borderWidth, yPos-borderWidth, width+borderWidth*2, height+borderWidth*2, borderColor);
        this->m_tft->fillRect(xPos, yPos, width, height, color);
    }
}

/**
 * @brief Changes the current page being viewed
 * 
 * @param newPage Page id of the new page
 */
void Page::setNewPage(page_t newPage){
    this->shouldSwap = true;
    this->swapTo = newPage;
}

bool Page::checkButton(button_t button){
    static unPhone &u = unPhone::me();
    switch(button){
    case b_one:
        return u.button1(); break;
    case b_two:
        return u.button2(); break;
    case b_three:
        return u.button3(); break;
    default:
        return false;
    }
}

/**
 * @brief Updates the status of all three of the buttons to allow for release and press checks
 * 
 */
void Page::updateButtons(){
    // This code sucks, dont care it probably works i wrote it at like 5am
    int cButton = 0;
    // loop through button status arrays and update if A: was pressed in the last tick or B: was released in the last tick
    for(bool* button: this->buttonStatus){
        bool isPressed = this->checkButton((button_t)cButton);
        // If button is still pressed then set pressed to false
        if(isPressed){
            if(!button[s_held] && !button[s_pressed]){
                D("button Pressed!");
                button[s_held] = true;
                button[s_pressed] = true;
            }
            else if(button[s_held] && button[s_pressed]){
                button[s_pressed] = false;
            }
        }else{
            if(button[s_held]){
                D("Button released!");
                button[s_held] = false;
                button[s_released] = true;
            }
            else
                button[s_released] = false;
        }
        cButton += 1;
    }
}

/**
 * @brief Gets status of the button requested
 * 
 * @param button B_ONE, B_TWO, or B_THREE (probably left to right)
 * 
 * @return true if the button is pressed
 */
bool Page::getButtonHeld(button_t button){
    return this->buttonStatus[button][s_held];
}

bool Page::getButtonPressed(button_t button){
    return this->buttonStatus[button][s_pressed];
}

bool Page::getButtonReleased(button_t button){
    return this->buttonStatus[button][s_released];
}
