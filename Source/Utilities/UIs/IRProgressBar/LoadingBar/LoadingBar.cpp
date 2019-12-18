//
//  LoadingBar.cpp
//  IRiMaS
//
//  Created by Keitaro on 17/12/2019.
//

#include "LoadingBar.hpp"

LoadingBar::LoadingBar(IRStr* str) : IRStrComponent(str)
{
    setFps(33);
}

LoadingBar::~LoadingBar()
{
    stopAnimation();
}
// ==================================================
void LoadingBar::resized()
{
    
}
void LoadingBar::paint(Graphics& g)
{
    g.setColour(getStr()->SYSTEMCOLOUR.fundamental);
    float w = (float)getWidth() * getProgressRate();
    //g.drawRect(0, 0, w, getHeight());
}
// ==================================================

// ==================================================

void LoadingBar::setCompleteValue(float val)
{
    this->completeVal = val;
}

void LoadingBar::setCurrentVal(float currentVal)
{
    this->currentVal = currentVal;
}

void LoadingBar::initialise()
{
    this->completeVal = 0.0;
    this->currentVal = 0.0;
    
    stopAnimation();
}
// ==================================================

void LoadingBar::start()
{
    startAnimation();
}

void LoadingBar::pause()
{
    stopAnimation();
}

// ==================================================

float LoadingBar::getProgressRate() const
{
    return this->completeVal / this->currentVal;
}

// ==================================================
void LoadingBar::updateAnimationFrame()
{
    repaint();
}
// ==================================================
// ==================================================
