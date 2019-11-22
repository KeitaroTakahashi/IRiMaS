//
//  IRLeftBar.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#include "IRLeftBar.hpp"


IRLeftBar::IRLeftBar(IRStr* str) : IRStrComponent(str),
IRHeavyWeightComponent(this)
{
    setFps(60);
    setOpaque(true);
        
    this->objectMenuComponent.reset(new LeftBarObjectMenu(str,
                                                          this->buttonSize,
                                                          this->topMarge,
                                                          this->leftMarge,
                                                          this->yMarge,
                                                          this->menuSpace,
                                                          this->preferenceMaxWidth,
                                                          this->buttomSpace,
                                                          this));
    
    this->slideMenuComponent.reset(new LeftBarSlideMenu(str));
    this->slideMenuComponent->workspaceSelectedCallback = [this](IRWorkspace* space) { workspaceSelectedAction(space); };
    
    this->objectMenuComponent->addMouseListener(this, true);
    this->objectMenuComponent->addChangeListener(this);
    this->slideMenuComponent->addMouseListener(this, true);
    this->slideMenuComponent->addChangeListener(this);

    addAndMakeVisible(this->objectMenuComponent.get());
    addButtons();
    
    this->objectSlideSwitchButton.reset( new ObjectSlideSwitchSpace(str,
                                                                    this->buttonSize) );
    addAndMakeVisible(this->objectSlideSwitchButton.get());
    this->objectSlideSwitchButton->toNavigatorButtonClicked = [this] { toNavigatorAction(); };
    this->objectSlideSwitchButton->toObjectMenuButtonClicked = [this] { toObjectMenuAction(); };

    this->ordinaryWidth = this->leftMarge + this->rightMarge + this->buttonSize;
    this->maxWidth = this->leftMarge + this->rightMarge + this->buttonSize + this->menuSpace;
    
    
    //OpenGL
    this->openGLContext.setContinuousRepainting(false);
    this->openGLContext.attachTo (*this);
}

IRLeftBar::~IRLeftBar()
{
    this->objectMenuComponent.reset();
    this->objectSlideSwitchButton.reset();
    this->openGLContext.detach();
}

//==================================================

void IRLeftBar::resized()
{
    int s = this->buttonSize;
    int y = this->topMarge;
    //this->toNavigatorButton.setBounds(this->leftMarge, y, s, s);
    //this->toObjectMenuButton.setBounds(this->leftMarge, y, s, s);
    
    this->objectSlideSwitchButton->setBounds(0, 0, getWidth(), s + this->yMarge * 2);
    
    y += s + (this->yMarge * 2);
    
    this->objectMenuComponent->setBounds(0, y,
                                         this->ordinaryWidth,
                                         getHeight() - y);
    
    this->slideMenuComponent->setBounds(0, y,
                                        this->ordinaryWidth,
                                        getHeight() - y);
    
    if(this->isOpened)
    {
        if(this->openPrefernceSpace)
        {
            setBounds(getX(), getY(), this->preferenceMaxWidth, getHeight());
            
        }
        else if(this->openMenuSpace)
        {
            setBounds(getX(), getY(), this->maxWidth, getHeight());
        }
        
    }
    heavyWeightRepaint();
}

void IRLeftBar::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    //g.fillAll(Colours::transparentWhite);

    //g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
   //g.fillRect(0, 0, this->ordinaryWidth, getHeight());
    
    //int y = this->topMarge + this->buttonSize + (this->yMarge * 2);

    //g.setColour(getStr()->SYSTEMCOLOUR.contents);
    //g.drawLine(0, (y - 2), getWidth(), (y - 2), 2);
       heavyWeightRepaint();

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

//==================================================
void IRLeftBar::bringToFrontCompleted()
{
    this->objectSlideSwitchButton->bringThisToFront();
    this->objectMenuComponent->bringToFrontCompleted();
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

void IRLeftBar::toNavigatorAction(bool notify)
{
    if(notify) this->objectSlideSwitchButton->switchToObjectMenuButton();
    this->objectMenuComponent->toNavigatorAction();
    
    removeChildComponent(this->objectMenuComponent.get());
    addAndMakeVisible(this->slideMenuComponent.get());
}

void IRLeftBar::toObjectMenuAction(bool notify)
{
    if(notify) this->objectSlideSwitchButton->switchToNavigatorButton();
    this->objectMenuComponent->toObjectMenuAction();
    
    removeChildComponent(this->slideMenuComponent.get());
    addAndMakeVisible(this->objectMenuComponent.get());
}

// ==================================================

void IRLeftBar::openDefaultMenu()
{
    if(this->openMenuSpace)
     {
         // stop animation when current width reaches the maxWidth
         setBounds(getX(), getY(), this->maxWidth, getHeight());
         this->isOpened = true;
     }else{
       
        setBounds(getX(), getY(), this->ordinaryWidth, getHeight());
        this->isOpened = false;
        this->previousMenuType = objectCategory::NONE;

    }
}
void IRLeftBar::openPreferenceMenu()
{
        // open menu space
     if(this->openPrefernceSpace)
     {
         // stop animation when current width reaches the maxWidth
         setBounds(getX(), getY(), this->preferenceMaxWidth, getHeight());
         this->isOpened = true;
         stopAnimation();
     }else{
        setBounds(getX(), getY(), this->ordinaryWidth, getHeight());
        this->isOpened = false;
        stopAnimation();
        this->previousMenuType = objectCategory::NONE;
    }
}

void IRLeftBar::updateAnimationFrame()
{
        
    switch(this->currentMenuType)
   {
       case objectCategory::INSPECTORMENU:
       case objectCategory::PREFERENCEMENU:
           //openPreferenceMenu();
           break;
       default:
          // openDefaultMenu();
           break;
   }
}

// ==================================================

void IRLeftBar::changeListenerCallback (ChangeBroadcaster* source)
{
     if(source == this->objectMenuComponent.get())
     {
         auto type = this->objectMenuComponent->getSelectedButtonType();
         this->currentMenuType = type;

         if(this->currentMenuType == this->previousMenuType)
         {
             closeMenu();
         }else{
             openMenu(type);
         }
         
         this->previousMenuType = type;
     }
}

// ==================================================

void IRLeftBar::openMenu(objectCategory type)
{
    // if menuSpace is not opened, then open it.
    this->openMenuSpace = true;
    this->openPrefernceSpace = true;
    this->isOpened = false;
    
    bringThisToFront();
    
    startAnimation();
    
}

void IRLeftBar::closeMenu()
{
    // if already opened, then close
    this->openMenuSpace = false;
    this->openPrefernceSpace = false;
    this->isOpened = false;
    startAnimation();
    this->objectMenuComponent->resetSelection();
}

// ==================================================

void IRLeftBar::addNewWorkspaceSlide(IRWorkspace* space)
{
    this->slideMenuComponent->addNewWorkspaceSlide(space);
    // switch to slide emenu
    toNavigatorAction(true);
}

// ==================================================
void IRLeftBar::workspaceSelectedAction(IRWorkspace* space)
{
    if(this->workspaceSelectedCallback != nullptr)
        this->workspaceSelectedCallback(space);
}
// ==================================================

void IRLeftBar::attachObjectMenu()
{
    
}

void IRLeftBar::removeObjectMenu()
{
    
}

void IRLeftBar::attachPreferenceMenu()
{
    
}

void IRLeftBar::removePreferenceMenu()
{
    
}
// ==================================================
