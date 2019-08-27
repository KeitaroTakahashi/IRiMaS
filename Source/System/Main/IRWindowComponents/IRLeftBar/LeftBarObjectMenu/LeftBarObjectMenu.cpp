//
//  LeftBarObjectMenu.cpp
//  IRiMaS
//
//  Created by Keitaro on 10/08/2019.
//

#include "LeftBarObjectMenu.hpp"

LeftBarObjectMenu::LeftBarObjectMenu(int buttonSize, int topMarge, int leftMarge, int yMarge, int menuSpace, int buttomSpace, Component* parent) :
buttonSize(buttonSize),
topMarge(topMarge),
menuSpace(menuSpace),
buttomSpace(buttomSpace),
yMarge(yMarge),
leftMarge(leftMarge),
parent(parent)
{
    addButtons();
    setFps(60);
}

LeftBarObjectMenu::~LeftBarObjectMenu()
{
    
}
// ==================================================

void LeftBarObjectMenu::resized()
{
    //top
    int x = this->leftMarge;
    int y = this->topMarge;
    int s = this->buttonSize;
    
    //y += s + this->yMarge * 2;
    //this->toNavigatorButton.setBounds(x, y, s, s);
    
   // y += this->yMarge * 2;
    
    //y += s + this->yMarge * 2;
    this->textButton.setBounds(x, y, s, s);
    
    y += s + this->yMarge * 2;
    this->imageButton.setBounds(x, y, s, s);
    
    y += s + this->yMarge * 2;
    this->audioButton.setBounds(x, y, s, s);
    
    y += s + this->yMarge * 2;
    this->chartButton.setBounds(x, y, s, s);
    
    y += s + this->yMarge * 2;
    this->playerButton.setBounds(x, y, s, s);
    
    y += s + this->yMarge * 2;
    this->objectButton.setBounds(x, y, s, s);
    
    y = getHeight() - this->buttomSpace + this->yMarge * 2;
    this->inspectorButton.setBounds(x, y, s, s);
    
    y += s + this->yMarge * 2;
    this->preferenceButton.setBounds(x, y, s, s);
    
    this->menuSpaceRect = Rectangle<int> (getWidth(), 0, this->menuSpace, getHeight());

    if(this->currentMenu != nullptr)
    {
        this->currentMenu->setBounds(this->menuSpaceRect);
    }
}
void LeftBarObjectMenu::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.fundamental);
    
    g.setColour(SYSTEMCOLOUR.text);
    int y = this->topMarge + this->buttonSize * 2 + this->yMarge * 4;
    g.drawLine(0, 0, getWidth(), 0, 2);
    
    y = getHeight() - this->buttomSpace;
    g.drawLine(0, y, getWidth(), y, 2);
    
    paintSelectedItem(g);
}

void LeftBarObjectMenu::paintSelectedItem(Graphics& g)
{
    if(this->selectedButtonType != NONE)
    {
        g.setColour(SYSTEMCOLOUR.contents);
        int index = (int)this->selectedButtonType;
        int x = this->leftMarge / 2;
        int y = this->topMarge + index * (this->buttonSize + this->yMarge*2) - this->yMarge/2;
        // width should be bigger to hide the right side rounded corner
        int w = getWidth() * 1.2;
        g.fillRoundedRectangle(x, y, w, this->buttonSize + this->yMarge, 5);
    }
}

// ==================================================

void LeftBarObjectMenu::createButton(IRImageButton* button, IRIconBank::IRIconImage img)
{
    if(SYSTEMCOLOUR.isWhiteBased)
        button->setImage(img.black);
    else
        button->setImage(img.white);
    
    //button->setDrawCircle(false);
    addAndMakeVisible(button);
}

void LeftBarObjectMenu::addButtons()
{
    createButton(&this->textButton, ImageBank.icon_text);
    this->textButton.onClick = [this]{ textAction(); };
    createButton(&this->imageButton, ImageBank.icon_image);
    this->imageButton.onClick = [this]{ imageAction(); };
    createButton(&this->audioButton, ImageBank.icon_wav);
    this->audioButton.onClick = [this]{ audioAction(); };
    createButton(&this->chartButton, ImageBank.icon_chart);
    this->chartButton.onClick = [this]{ chartAction(); };
    createButton(&this->playerButton, ImageBank.icon_play);
    this->playerButton.onClick = [this]{ playerAction(); };
    createButton(&this->objectButton, ImageBank.icon_object);
    this->objectButton.onClick = [this]{ objectAction(); };

    createButton(&this->inspectorButton, ImageBank.icon_inspector);
    this->inspectorButton.onClick = [this] { inspectorAction(); };
    createButton(&this->preferenceButton, ImageBank.icon_preference);
    this->preferenceButton.onClick = [this] { preferenceAction(); };


}
// ==================================================

void LeftBarObjectMenu::textAction()
{
    std::cout<< "textAction\n";
    showExtraMenu(TEXTMENU);
    this->textMenu.reset(new TextMenuComponent(this->menuSpaceRect));
    addAndMakeVisible(this->textMenu.get());
    replaceCurrentMenu(this->textMenu.get());

}
void LeftBarObjectMenu::imageAction()
{
    showExtraMenu(IMAGEMENU);
    this->imageMenu.reset(new ImageMenuComponent(this->menuSpaceRect));
    addAndMakeVisible(this->imageMenu.get());
    replaceCurrentMenu(this->imageMenu.get());


}
void LeftBarObjectMenu::audioAction()
{
    showExtraMenu(AUDIOMENU);
    
    this->audioMenu.reset(new AudioMenuComponent(this->menuSpaceRect));
    addAndMakeVisible(this->audioMenu.get());
    replaceCurrentMenu(this->audioMenu.get());
   
}
void LeftBarObjectMenu::chartAction()
{
    showExtraMenu(CHARTMENU);
    

}
void LeftBarObjectMenu::playerAction()
{
    showExtraMenu(PLAYERMENU);

}
void LeftBarObjectMenu::objectAction()
{
    showExtraMenu(OBJECTMENU);

}

void LeftBarObjectMenu::inspectorAction()
{
    
}
void LeftBarObjectMenu::preferenceAction()
{
    
}
// ==================================================

void LeftBarObjectMenu::replaceCurrentMenu(ObjectMenuComponent* obj)
{
    if(this->currentMenu != nullptr)
        this->parent->removeChildComponent(this->currentMenu);
    
    this->currentMenu = obj;
    this->parent->addAndMakeVisible(this->currentMenu);
    
    resized();
    repaint();

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

void LeftBarObjectMenu::updateAnimationFrame()
{
    int max = (this->yMarge*2 + this->buttonSize + this->menuSpace);
    if(getWidth() < max)
    {
        //setBounds(getX(), getY(), getWidth() + 1, getHeight());
        setSize(getWidth() + 1, getHeight());
    }else{
        setBounds(getX(), getY(), getWidth() + this->menuSpace, getHeight());
        stopAnimation();
    }
    repaint();
}

