//
//  IRLeftBar.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#include "IRLeftBar.hpp"


IRLeftBar::IRLeftBar()
{
    setFps(60);
    
    this->objectMenuComponent.reset(new LeftBarObjectMenu(this->buttonSize,
                                                          this->topMarge,
                                                          this->leftMarge,
                                                          this->yMarge,
                                                          this->menuSpace,
                                                          this->buttomSpace,
                                                          this));
    
    this->objectMenuComponent->addMouseListener(this, true);
    this->objectMenuComponent->addChangeListener(this);
    addAndMakeVisible(this->objectMenuComponent.get());
    
    addButtons();
    
    this->ordinaryWidth = this->leftMarge * 2 + this->buttonSize;
    this->maxWidth = this->leftMarge * 2 + this->buttonSize + this->menuSpace;
    
}

IRLeftBar::~IRLeftBar()
{
    this->objectMenuComponent.reset();
}
//==================================================

void IRLeftBar::resized()
{
    int s = this->buttonSize;
    int y = this->topMarge;
    this->toNavigatorButton.setBounds(this->leftMarge, y, s, s);
    y += s + (this->yMarge * 2);
    this->objectMenuComponent->setBounds(0, y, this->ordinaryWidth, getHeight() - y);
}

void IRLeftBar::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.contents);
    g.setColour(SYSTEMCOLOUR.fundamental);
    g.fillRect(0, 0, this->ordinaryWidth, getHeight());
}

//==================================================


void IRLeftBar::mouseDrag(const MouseEvent& e)
{
    this->currentPos = e.getScreenPosition();
    Point<int>delta = this->currentPos - this->prevPos;
    
    // We need to subtract 1 to fix the gap between view position and window position!!
    this->pos.setX(getScreenX() + delta.getX());
    this->pos.setY(getScreenY() + delta.getY() - getY());
    
    this->status = MoveWindow;
    sendChangeMessage();
    
    this->prevPos = e.getScreenPosition();
}
void IRLeftBar::mouseUp(const MouseEvent& e)
{
    
}
void IRLeftBar::mouseDown(const MouseEvent& e)
{
    auto pos = e.getScreenPosition();
    this->prevPos = pos;
    checkResizableFromMouseDownPosition(pos);
}
void IRLeftBar::mouseMove(const MouseEvent& e)
{
    
}

//==================================================
void IRLeftBar::checkResizableFromMouseDownPosition(Point<int> pos)
{
    
}

// ==================================================

void IRLeftBar::createButton(IRImageButton* button, IRIconBank::IRIconImage img)
{
    if(SYSTEMCOLOUR.isWhiteBased)
        button->setImage(img.black);
    else
        button->setImage(img.white);
    
    //button->setDrawCircle(false);
    addAndMakeVisible(button);
}

void IRLeftBar::addButtons()
{
    
    createButton(&this->toNavigatorButton, ImageBank.icon_toNavigator);
    this->toNavigatorButton.onClick = [this]{ toNavigatorAction(); };
}
// ==================================================

void IRLeftBar::toNavigatorAction()
{
    
}

void IRLeftBar::updateAnimationFrame()
{
    // open menu space
    if(this->openMenuSpace)
    {
        if(getWidth() < this->maxWidth)
        {
            setBounds(getX(), getY(), getWidth() + this->openSpeed, getHeight());
        }else{
            // stop animation when current width reaches the maxWidth
            setBounds(getX(), getY(), this->maxWidth, getHeight());
            stopAnimation();
        }
    }else{
        // close menu space
        if(getWidth() > this->ordinaryWidth)
        {
            setBounds(getX(), getY(), getWidth() - this->openSpeed, getHeight());
        }else{
            
            setBounds(getX(), getY(), this->ordinaryWidth, getHeight());
            stopAnimation();
        }
    }

}

void IRLeftBar::changeListenerCallback (ChangeBroadcaster* source)
{
     if(source == this->objectMenuComponent.get())
     {
         auto type = this->objectMenuComponent->getSelectedButtonType();
         std::cout << "textMENU callback : type " << type << std::endl;
         
         if(this->currentMenuType == type)
         {
             // if already opened, then close
             this->openMenuSpace = false;
             startAnimation();
             this->objectMenuComponent->resetSelection();
             this->currentMenuType = objectCategory::NONE;
             
         }else{
             
             this->currentMenuType = type;
             
             // if menuSpace is not opened, then open it.
             if(!this->openMenuSpace)
             {
                 this->openMenuSpace = true;
                 startAnimation();
             }
             
         }


     }
}
