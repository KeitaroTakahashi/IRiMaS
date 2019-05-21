//
//  IRImageButton.cpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 02/05/2019.
//

#include "IRImageButton.hpp"

IRImageButton::IRImageButton()
{
    
}

IRImageButton::~IRImageButton()
{
    
}

// ==================================================

void IRImageButton::paint(Graphics& g)
{
    g.setColour(Colours::transparentWhite);
    g.fillEllipse(0, 0, getWidth(), getHeight());
    g.setColour(Colours::white);
    g.drawEllipse(1, 1, getWidth()-2, getHeight()-2, 2);
    g.drawImage(this->buttonImage, getLocalBounds().toFloat());
}

void IRImageButton::resized()
{
    
}

void IRImageButton::setImage(Image img)
{
    this->buttonImage = img;
    repaint();
}

void IRImageButton::mouseDown(const MouseEvent& e)
{
    if(this->onClick != nullptr) onClick();
}


