//
//  PropertyComponent.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 13/03/2019.
//

#include "IRPropertyComponent.hpp"

IRPropertyComponent::IRPropertyComponent(const String &propertyName, Point<int>Marge, int preferredHeight) :
PropertyComponent(propertyName, preferredHeight)
{
    this->title.setText(propertyName, dontSendNotification);
    addAndMakeVisible(this->title);
    
    setMarge(Marge.getX(), Marge.getY());
}

IRPropertyComponent::~IRPropertyComponent()
{
    
}

void IRPropertyComponent::resized()
{
    this->title.setBounds(this->xMarge, this->yMarge, getWidth(), getHeight());
}
void IRPropertyComponent::paint(Graphics& g)
{
    
}

void IRPropertyComponent::refresh()
{
    
}

void IRPropertyComponent::setMarge(int x, int y)
{
    this->xMarge = x;
    this->yMarge = y;
}
