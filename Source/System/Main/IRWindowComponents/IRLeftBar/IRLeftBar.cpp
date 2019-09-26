//
//  IRLeftBar.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#include "IRLeftBar.hpp"


IRLeftBar::IRLeftBar(IRStr* str) : IRStrComponent(str)
{
    setFps(60);
    setOpaque(true);
        
    this->objectMenuComponent.reset(new LeftBarObjectMenu(str,
                                                          this->buttonSize,
                                                          this->topMarge,
                                                          this->leftMarge,
                                                          this->yMarge,
                                                          this->menuSpace,
                                                          this->buttomSpace,
                                                          this));
    
    this->slideMenuComponent.reset(new LeftBarSlideMenu(str));
    
    this->objectMenuComponent->addMouseListener(this, true);
    this->objectMenuComponent->addChangeListener(this);
    this->slideMenuComponent->addMouseListener(this, true);
    this->slideMenuComponent->addChangeListener(this);

    addAndMakeVisible(this->objectMenuComponent.get());
    //addAndMakeVisible(this->slideMenuComponent.get());
    addButtons();
    
    this->ordinaryWidth = this->leftMarge + this->rightMarge + this->buttonSize;
    this->maxWidth = this->leftMarge + this->rightMarge + this->buttonSize + this->menuSpace;
    
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
    this->toObjectMenuButton.setBounds(this->leftMarge, y, s, s);

    y += s + (this->yMarge * 2);
    
    this->objectMenuComponent->setBounds(0, y,
                                         this->ordinaryWidth,
                                         getHeight() - y);
    
    this->slideMenuComponent->setBounds(0, y,
                                        this->ordinaryWidth,
                                        getHeight() - y);
    
    if(this->isOpened)
        setBounds(getX(), getY(), this->maxWidth, getHeight());
    
}

void IRLeftBar::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    g.setColour(getStr()->SYSTEMCOLOUR.fundamental);
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
    if(getStr()->SYSTEMCOLOUR.isWhiteBased)
        button->setImage(img.black);
    else
        button->setImage(img.white);
    
    //button->setDrawCircle(false);
    addAndMakeVisible(button);
}

void IRLeftBar::addButtons()
{
    createButton(&this->toNavigatorButton, getStr()->ICONBANK.icon_toNavigator);
    this->toNavigatorButton.onClick = [this]{ toNavigatorAction(); };
}

// ==================================================

void IRLeftBar::toNavigatorAction()
{
    removeChildComponent(&this->toNavigatorButton);
    createButton(&this->toObjectMenuButton, getStr()->ICONBANK.icon_toObjectMenu);
    this->toObjectMenuButton.onClick = [this] { toObjectMenuAction(); };
    this->objectMenuComponent->toNavigatorAction();
    
    removeChildComponent(this->objectMenuComponent.get());
    addAndMakeVisible(this->slideMenuComponent.get());
}

void IRLeftBar::toObjectMenuAction()
{
    removeChildComponent(&this->toObjectMenuButton);
    createButton(&this->toNavigatorButton, getStr()->ICONBANK.icon_toNavigator);
    this->toObjectMenuButton.onClick = [this] { toNavigatorAction(); };
    this->objectMenuComponent->toObjectMenuAction();
    
    removeChildComponent(this->slideMenuComponent.get());
    addAndMakeVisible(this->objectMenuComponent.get());

}

// ==================================================

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
            this->isOpened = true;
            stopAnimation();
        }
    }else{
        // close menu space
        if(getWidth() > this->ordinaryWidth)
        {
            setBounds(getX(), getY(), getWidth() - this->openSpeed, getHeight());
        }else{
            
            setBounds(getX(), getY(), this->ordinaryWidth, getHeight());
            this->isOpened = false;
            stopAnimation();
        }
    }

}

// ==================================================

void IRLeftBar::changeListenerCallback (ChangeBroadcaster* source)
{
     if(source == this->objectMenuComponent.get())
     {
         auto type = this->objectMenuComponent->getSelectedButtonType();
         std::cout << "textMENU callback : type " << type << std::endl;
         
         if(this->currentMenuType == type)
         {
             closeMenu();
         }else{
             openMenu(type);
         }
     }
}

// ==================================================

void IRLeftBar::openMenu(objectCategory type)
{
    this->currentMenuType = type;
    // if menuSpace is not opened, then open it.
    if(!this->openMenuSpace)
    {
        this->openMenuSpace = true;
        startAnimation();
    }
}

void IRLeftBar::closeMenu()
{
    // if already opened, then close
    this->openMenuSpace = false;
    this->isOpened = false;
    startAnimation();
    this->objectMenuComponent->resetSelection();
    this->currentMenuType = objectCategory::NONE;
}

// ==================================================
