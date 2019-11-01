//
//  IRTitleBar.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#include "IRTitleBar.hpp"

//==============================================================================
IRTitleBar::IRTitleBar(IRStr* str, String title) : IRStrComponent(str),
comp(str, title)
{
    this->comp.addMouseListener(this, true);
    addAndMakeVisible(&this->comp);
    
    this->openGLContext.setRenderer(this);
    this->openGLContext.setContinuousRepainting(false);
    this->openGLContext.attachTo(*this);
}

IRTitleBar::~IRTitleBar()
{
    this->openGLContext.detach();
}

//==============================================================================
void IRTitleBar::paint (Graphics& g)
{
    /*
    g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    g.setColour(getStr()->SYSTEMCOLOUR.contents);
    g.drawLine(0, getHeight() , getWidth(), getHeight(), 2);
    g.setColour(getStr()->SYSTEMCOLOUR.text);
    Font f("Avenir Next",34, Font::plain);
    g.setFont(f);
    g.drawText("TiAALS", 30, 5, 200, getHeight(), dontSendNotification);
     */
    //this->openGLContext.triggerRepaint();

}

void IRTitleBar::resized()
{
    this->comp.setBounds(getLocalBounds());
}

void IRTitleBar::mouseDrag(const MouseEvent& e)
{
    this->currentPos = e.getScreenPosition();
    Point<int>delta = this->currentPos - this->prevPos;
    
    this->pos.setX(getScreenX() + delta.getX());
    this->pos.setY(getScreenY() + delta.getY());
    
    this->status = MoveWindow;
    sendChangeMessage();

    this->prevPos = e.getScreenPosition();
}
//==============================================================================

void IRTitleBar::mouseUp(const MouseEvent& e)
{
    
}
void IRTitleBar::mouseDown(const MouseEvent& e)
{
    auto pos = e.getScreenPosition();
    this->prevPos = pos;
    checkResizableFromMouseDownPosition(pos);
}

void IRTitleBar::mouseDoubleClick(const MouseEvent& e)
{
    if(this->titleDoubleClickedCallback != nullptr)
        this->titleDoubleClickedCallback();
}

void IRTitleBar::checkResizableFromMouseDownPosition(Point<int> pos)
{
    
}


void IRTitleBar::mouseMove(const MouseEvent& e)
{
    //std::cout << "mouse move : " << e.getPosition().getX() << " : " << e.getPosition().getY() << std::endl;
}
//==============================================================================

void IRTitleBar::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == &this->comp)
    {
        switch(this->comp.getSelectedButtonType())
        {
            case IRTitleBarComponent::SelectedButtonType::rightBar:
                break;
            case IRTitleBarComponent::SelectedButtonType::leftBar:
                break;
            default:
                break;
        }
    }
}
//==============================================================================

//==============================================================================
