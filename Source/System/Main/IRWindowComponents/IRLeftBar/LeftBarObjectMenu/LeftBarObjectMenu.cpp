//
//  LeftBarObjectMenu.cpp
//  IRiMaS
//
//  Created by Keitaro on 10/08/2019.
//

#include "LeftBarObjectMenu.hpp"

LeftBarObjectMenu::LeftBarObjectMenu(IRStr* str,
                                     int buttonSize,
                                     int topMarge,
                                     int leftMarge,
                                     int yMarge,
                                     int menuSpace,
                                     int preferenceMenuSpace,
                                     int buttomSpace,
                                     Component* parent) :
IRStrComponent(str),
buttonSize(buttonSize),
topMarge(topMarge),
menuSpace(menuSpace),
preferenceMenuSpace(preferenceMenuSpace),
buttomSpace(buttomSpace),
yMarge(yMarge),
leftMarge(leftMarge),
parent(parent)
{
    addButtons();
  
    this->inspectorLabel.setText("Inspector", dontSendNotification);
    this->inspectorLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    addAndMakeVisible(&this->inspectorLabel);
    
    this->preferenceLabel.setText("Preference", dontSendNotification);
    this->preferenceLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    addAndMakeVisible(&this->preferenceLabel);
    
    this->objectMenuIconArea.reset( new ObjectMenuIconArea(str, this->buttonSize, this->leftMarge) );
    addAndMakeVisible(this->objectMenuIconArea.get());
    this->objectMenuIconArea->textActionCallback = [this]{ textAction(); };
    this->objectMenuIconArea->imageActionCallback = [this]{ imageAction(); };
    this->objectMenuIconArea->audioActionCallback = [this]{ audioAction(); };
    this->objectMenuIconArea->chartActionCallback = [this]{ chartAction(); };
    this->objectMenuIconArea->playerActionCallback = [this]{ playerAction(); };
    this->objectMenuIconArea->objectActionCallback = [this]{ objectAction(); };

}

LeftBarObjectMenu::~LeftBarObjectMenu()
{
    this->objectMenuIconArea.reset();
   
    this->objectMenuSpaceArea.reset();
    
    this->textMenu.reset();
    this->imageMenu.reset();
    this->audioMenu.reset();
    this->chartMenu.reset();
    this->playerMenu.reset();
    this->objectMenu.reset();
    this->inspectorMenu.reset();
    this->preferenceMenu.reset();
}
// ==================================================

void LeftBarObjectMenu::resized()
{
    //top
    int x = this->leftMarge;
    int y = this->topMarge;
    int s = this->buttonSize;
    
    int t_d = 5;
    
    this->objectMenuIconArea->setBounds(0, 0, getWidth(), 350);
    
    // objectMenuSpaceArea is added on the IRWindowComponent
    if(this->objectMenuSpaceArea.get() != nullptr)
    {
        if(this->selectedButtonType == PREFERENCEMENU || this->selectedButtonType == INSPECTORMENU)
        {
            this->objectMenuSpaceArea->setBounds(getWidth(),
                                                 getStr()->projectOwner->barHeight,
                                                 this->preferenceMenuSpace,
                                                 getHeight() + getStr()->projectOwner->barHeight);
        }else
        {
            this->objectMenuSpaceArea->setBounds(getWidth(),
                                                 getStr()->projectOwner->barHeight,
                                                 150,
                                                 getHeight() + getStr()->projectOwner->barHeight);
        }
    }

    //Inspector Buttons
    y = getHeight() - this->buttomSpace + this->yMarge * 2;
    this->inspectorButton.setBounds(x, y, s, s);
    this->inspectorLabel.setBounds(x + this->buttonSize + t_d, y + 8, 80, 24);
    
    y += s + this->yMarge * 2;
    this->preferenceButton.setBounds(x, y, s, s);
    this->preferenceLabel.setBounds(x + this->buttonSize + t_d, y + 8, 80, 24);
    
    
    if(this->selectedButtonType != PREFERENCEMENU || this->selectedButtonType != INSPECTORMENU)
    {
        this->menuSpaceRect = Rectangle<int> (getWidth(), 0, this->preferenceMenuSpace, getParentHeight());
    }else
    {
        this->menuSpaceRect = Rectangle<int> (getWidth(), 0, this->menuSpace, getParentHeight());
    }
    
    if(this->objectMenuSpaceAreaBringToFrontFlag)
    {
        this->objectMenuSpaceArea->bringThisToFront();
        this->objectMenuSpaceAreaBringToFrontFlag = false;
    }
}
void LeftBarObjectMenu::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    
    g.setColour(getStr()->SYSTEMCOLOUR.contents);
    int y = this->topMarge + this->buttonSize * 2 + this->yMarge * 4;
    //g.drawLine(0, 0, getWidth(), 0, 2);
    
    y = getHeight() - this->buttomSpace;
    //g.drawLine(0, y, getWidth(), y, 2);
    
    paintSelectedItem(g);
}

void LeftBarObjectMenu::paintSelectedItem(Graphics& g)
{
    if(this->selectedButtonType != NONE)
    {
        g.setColour(getStr()->SYSTEMCOLOUR.contents);
        int x = this->leftMarge / 2;
        int y = getButtonFromType().getBounds().getY() - this->yMarge/2;
        
        // width should be bigger to hide the right side rounded corner
        int w = getWidth() * 1.2;
        g.fillRoundedRectangle(x, y, w, this->buttonSize + this->yMarge, 5);
    }
}

// ==================================================

void LeftBarObjectMenu::createButton(IRImageButton* button, IRIconBank::IRIconImage img)
{
    if(getStr()->SYSTEMCOLOUR.isWhiteBased)
        button->setImage(img.black);
    else
        button->setImage(img.white);
    
    addAndMakeVisible(button);
}

void LeftBarObjectMenu::addButtons()
{
    createButton(&this->inspectorButton, getStr()->ICONBANK.icon_inspector);
    this->inspectorButton.onClick = [this] { inspectorAction(); };
    createButton(&this->preferenceButton, getStr()->ICONBANK.icon_preference);
    this->preferenceButton.onClick = [this] { preferenceAction(); };
}
// ==================================================

void LeftBarObjectMenu::textAction()
{
    showExtraMenu(TEXTMENU);
    if(this->textMenu.get() == nullptr)
        this->textMenu.reset(new TextMenuComponent(getStr(), this->menuSpaceRect));
    bindObjectMenuOnParent(this->textMenu.get());

}
void LeftBarObjectMenu::imageAction()
{
    showExtraMenu(IMAGEMENU);
    if(this->imageMenu.get() == nullptr)
        this->imageMenu.reset(new ImageMenuComponent(getStr(), this->menuSpaceRect));
    bindObjectMenuOnParent(this->imageMenu.get());
}
void LeftBarObjectMenu::audioAction()
{
    showExtraMenu(AUDIOMENU);
    if(this->audioMenu.get() == nullptr)
        this->audioMenu.reset(new AudioMenuComponent(getStr(), this->menuSpaceRect));
    bindObjectMenuOnParent(this->audioMenu.get());

   
}
void LeftBarObjectMenu::chartAction()
{
    showExtraMenu(CHARTMENU);
    if(this->chartMenu.get() == nullptr)
        this->chartMenu.reset(new ChartMenuComponent(getStr(), this->menuSpaceRect));
    bindObjectMenuOnParent(this->chartMenu.get());

}
void LeftBarObjectMenu::playerAction()
{
    showExtraMenu(PLAYERMENU);
    if(this->playerMenu.get() == nullptr)
        this->playerMenu.reset(new PlayerMenuComponent(getStr(), this->menuSpaceRect));
    bindObjectMenuOnParent(this->playerMenu.get());

}
void LeftBarObjectMenu::objectAction()
{
    showExtraMenu(OBJECTMENU);
    if(this->objectMenu.get() == nullptr)
        this->objectMenu.reset(new ObjectsMenuComponent(getStr(), this->menuSpaceRect));
    bindObjectMenuOnParent(this->objectMenu.get());

}

void LeftBarObjectMenu::inspectorAction()
{
    showExtraMenu(INSPECTORMENU);
    if(this->inspectorMenu.get() == nullptr)
        this->inspectorMenu.reset(new InspectorMenuComponent(getStr(), this->menuSpaceRect));
    bindObjectMenuOnParent(this->inspectorMenu.get());

}
void LeftBarObjectMenu::preferenceAction()
{
    showExtraMenu(PREFERENCEMENU);
    if(this->preferenceMenu.get() == nullptr)
        this->preferenceMenu.reset(new PreferenceMenuComponent(getStr(), this->menuSpaceRect));
    bindObjectMenuOnParent(this->preferenceMenu.get());
}
// ==================================================

void LeftBarObjectMenu::replaceCurrentMenu(ObjectMenuComponent* obj)
{
    if(this->currentMenu != nullptr)
        this->parent->removeChildComponent(this->currentMenu);
    
    this->currentMenu = obj;
    this->currentMenu->toFront(true);
    this->parent->addAndMakeVisible(this->currentMenu);
    
    resized();
    repaint();

}

void LeftBarObjectMenu::createObjectMenuAreaAndShow(ObjectMenuComponent* obj)
{
    this->currentMenu = obj;
    // if area is not yet created, then create it.
    if(this->objectMenuSpaceArea.get() == nullptr)
        this->objectMenuSpaceArea.reset(new ObjectMenuSpaceArea(getStr()));
    
    this->objectMenuSpaceArea->setHidden(false);
    this->objectMenuSpaceArea->setMenuComponent(this->currentMenu);
    getStr()->projectOwner->addAndMakeVisible(this->objectMenuSpaceArea.get());

    this->objectMenuSpaceAreaBringToFrontFlag = true;
}


void LeftBarObjectMenu::bindObjectMenuOnParent(ObjectMenuComponent* obj)
{
    std::cout << "current Menu = " << this->currentMenu << " : " << obj << std::endl;
    
    if(this->objectMenuSpaceArea.get() != nullptr)
    {
        if(this->currentMenu != nullptr)
            this->objectMenuSpaceArea->removeMenuComponent();
        
        if(!this->objectMenuSpaceArea->isHidden())
        {
            if(this->currentMenu == obj) // remove
            {
                getStr()->projectOwner->removeChildComponent(this->objectMenuSpaceArea.get());
                // for some reason we need explicitly set if objectMenuSpaceArea is hidden or not.
                this->objectMenuSpaceArea->setHidden(true);
                this->currentMenu = nullptr;
            }else{
                // replace
                this->currentMenu = obj;
                this->objectMenuSpaceArea->setMenuComponent(this->currentMenu);
                this->objectMenuSpaceArea->bringThisToFront();
                
                this->objectMenuSpaceArea->setHidden(false);

            }
        }else{
            createObjectMenuAreaAndShow(obj);
        }
    }else{
        
        createObjectMenuAreaAndShow(obj);


    }
        resized();
}

void LeftBarObjectMenu::closeObjectMenu()
{
    std::cout << "closeObjectMenu\n";
    /*
    if(this->objectMenuSpaceArea.get() != nullptr)
    {
        this->objectMenuSpaceArea->removeMenuComponent();
        getStr()->projectOwner->removeChildComponent(this->objectMenuSpaceArea.get());
        
        this->objectMenuSpaceArea.reset();
    }*/
}


// ==================================================

void LeftBarObjectMenu::showExtraMenu(objectCategory type)
{
    if(this->selectedButtonType == type)
    {
        sendChangeMessage();
    }else
    {
        this->selectedButtonType = type;
        sendChangeMessage();
        
    }
    
    repaint();
}

// ==================================================

// ==================================================

void LeftBarObjectMenu::toNavigatorAction()
{
    
}

void LeftBarObjectMenu::toObjectMenuAction()
{
    
}

// ==================================================

IRImageButton& LeftBarObjectMenu::getButtonFromType()
{
    switch(this->selectedButtonType)
    {
        case TEXTMENU:
            return this->textButton;
            break;
        case IMAGEMENU:
            return this->imageButton;
            break;
        case AUDIOMENU:
            return this->audioButton;
            break;
        case CHARTMENU:
            return this->chartButton;
            break;
        case PLAYERMENU:
            return this->playerButton;
            break;
        case OBJECTMENU:
            return this->objectButton;
            break;
        case INSPECTORMENU:
            return this->inspectorButton;
            break;
        case PREFERENCEMENU:
            return this->preferenceButton;
            break;
        default:
            return this->preferenceButton;
            break;
    }
}

void LeftBarObjectMenu::bringToFrontCompleted()
{
    if(this->objectMenuIconArea != nullptr)
        this->objectMenuIconArea->bringThisToFront();
    
    if(this->objectMenuSpaceArea != nullptr)
        this->objectMenuSpaceArea->bringThisToFront();
   
}
