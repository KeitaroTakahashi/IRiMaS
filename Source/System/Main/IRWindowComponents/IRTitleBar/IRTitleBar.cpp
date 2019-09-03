//
//  IRTitleBar.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#include "IRTitleBar.hpp"

//==============================================================================
IRTitleBar::IRTitleBar(IRStr* str) : IRStrComponent(str)
{
    
}

IRTitleBar::~IRTitleBar()
{
    
}

//==============================================================================
void IRTitleBar::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::white);
    g.setColour(Colours::lightgrey);
    g.drawLine(0, getHeight() - 1 , getWidth(), getHeight() - 1, 2);
    
}

void IRTitleBar::resized()
{
    // This is called when the IRTitleBar is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void IRTitleBar::mouseDrag(const MouseEvent& e)
{
    this->currentPos = e.getScreenPosition();
    Point<int>delta = this->currentPos - this->prevPos;
    
    // We need to subtract 1 to fix the gap between view position and window position!!
    this->pos.setX(getScreenX() + delta.getX());
    this->pos.setY(getScreenY() + delta.getY());
    
    this->status = MoveWindow;
    sendChangeMessage();
    std::cout<<"mouseDrag\n";

    this->prevPos = e.getScreenPosition();
}
void IRTitleBar::mouseUp(const MouseEvent& e)
{
    
}
void IRTitleBar::mouseDown(const MouseEvent& e)
{
    auto pos = e.getScreenPosition();
    this->prevPos = pos;
    checkResizableFromMouseDownPosition(pos);
}
void IRTitleBar::checkResizableFromMouseDownPosition(Point<int> pos)
{
    
}


void IRTitleBar::mouseMove(const MouseEvent& e)
{
    //std::cout << "mouse move : " << e.getPosition().getX() << " : " << e.getPosition().getY() << std::endl;
}
