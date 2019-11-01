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
    
    this->textLabel.setText("Texts", dontSendNotification);
    this->textLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    addAndMakeVisible(&this->textLabel);
    
    this->imageLabel.setText("Graphics", dontSendNotification);
    this->imageLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    addAndMakeVisible(&this->imageLabel);
    
    this->audioLabel.setText("Audio", dontSendNotification);
    this->audioLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    addAndMakeVisible(&this->audioLabel);
    
    this->chartLabel.setText("Charts", dontSendNotification);
    this->chartLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    addAndMakeVisible(&this->chartLabel);
    
    this->playerLabel.setText("Players", dontSendNotification);
    this->playerLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    addAndMakeVisible(&this->playerLabel);
    
    this->objectLabel.setText("Objects", dontSendNotification);
    this->objectLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    addAndMakeVisible(&this->objectLabel);
    
    this->inspectorLabel.setText("Inspector", dontSendNotification);
    this->inspectorLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    addAndMakeVisible(&this->inspectorLabel);
    
    this->preferenceLabel.setText("Preference", dontSendNotification);
    this->preferenceLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    addAndMakeVisible(&this->preferenceLabel);
    
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
    
    int t_d = 5;
    
    this->textButton.setBounds(x, y, s, s);
    this->textLabel.setBounds(x + this->buttonSize + t_d, y + 8, 80, 24);
    
    y += s + this->yMarge * 2;
    this->imageButton.setBounds(x, y, s, s);
    this->imageLabel.setBounds(x + this->buttonSize + t_d, y + 8, 80, 24);

    
    y += s + this->yMarge * 2;
    this->audioButton.setBounds(x, y, s, s);
    this->audioLabel.setBounds(x + this->buttonSize + t_d, y + 8, 80, 24);
    
    y += s + this->yMarge * 2;
    this->chartButton.setBounds(x, y, s, s);
    this->chartLabel.setBounds(x + this->buttonSize + t_d, y + 8, 80, 24);
    
    y += s + this->yMarge * 2;
    this->playerButton.setBounds(x, y, s, s);
    this->playerLabel.setBounds(x + this->buttonSize + t_d, y + 8, 80, 24);
    
    y += s + this->yMarge * 2;
    this->objectButton.setBounds(x, y, s, s);
    this->objectLabel.setBounds(x + this->buttonSize + t_d, y + 8, 80, 24);
    
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
    
    if(this->currentMenu != nullptr)
    {
        this->currentMenu->setBounds(this->menuSpaceRect);
    }
}
void LeftBarObjectMenu::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    
    g.setColour(getStr()->SYSTEMCOLOUR.contents);
    int y = this->topMarge + this->buttonSize * 2 + this->yMarge * 4;
    //g.drawLine(0, 0, getWidth(), 0, 2);
    
    y = getHeight() - this->buttomSpace;
    g.drawLine(0, y, getWidth(), y, 2);
    
    paintSelectedItem(g);
}

void LeftBarObjectMenu::paintSelectedItem(Graphics& g)
{
    if(this->selectedButtonType != NONE)
    {
        g.setColour(getStr()->SYSTEMCOLOUR.contents);
        int index = (int)this->selectedButtonType;
        int x = this->leftMarge / 2;
        //int y = this->topMarge + index * (this->buttonSize + this->yMarge*2) - this->yMarge/2;
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
    
    //button->setDrawCircle(false);
    addAndMakeVisible(button);
}

void LeftBarObjectMenu::addButtons()
{
    createButton(&this->textButton, getStr()->ICONBANK.icon_text);
    this->textButton.onClick = [this]{ textAction(); };
    createButton(&this->imageButton, getStr()->ICONBANK.icon_image);
    this->imageButton.onClick = [this]{ imageAction(); };
    createButton(&this->audioButton, getStr()->ICONBANK.icon_wav);
    this->audioButton.onClick = [this]{ audioAction(); };
    createButton(&this->chartButton, getStr()->ICONBANK.icon_chart);
    this->chartButton.onClick = [this]{ chartAction(); };
    createButton(&this->playerButton, getStr()->ICONBANK.icon_play);
    this->playerButton.onClick = [this]{ playerAction(); };
    createButton(&this->objectButton, getStr()->ICONBANK.icon_object);
    this->objectButton.onClick = [this]{ objectAction(); };

    createButton(&this->inspectorButton, getStr()->ICONBANK.icon_inspector);
    this->inspectorButton.onClick = [this] { inspectorAction(); };
    createButton(&this->preferenceButton, getStr()->ICONBANK.icon_preference);
    this->preferenceButton.onClick = [this] { preferenceAction(); };


}
// ==================================================

void LeftBarObjectMenu::textAction()
{
    showExtraMenu(TEXTMENU);
    
    this->textMenu.reset(new TextMenuComponent(getStr(), this->menuSpaceRect));

    addAndMakeVisible(this->textMenu.get());
    replaceCurrentMenu(this->textMenu.get());

}
void LeftBarObjectMenu::imageAction()
{
    showExtraMenu(IMAGEMENU);
    this->imageMenu.reset(new ImageMenuComponent(getStr(), this->menuSpaceRect));
    addAndMakeVisible(this->imageMenu.get());
    replaceCurrentMenu(this->imageMenu.get());


}
void LeftBarObjectMenu::audioAction()
{
    showExtraMenu(AUDIOMENU);
    
    this->audioMenu.reset(new AudioMenuComponent(getStr(), this->menuSpaceRect));
    addAndMakeVisible(this->audioMenu.get());
    replaceCurrentMenu(this->audioMenu.get());
   
}
void LeftBarObjectMenu::chartAction()
{
    showExtraMenu(CHARTMENU);
    this->chartMenu.reset(new ChartMenuComponent(getStr(), this->menuSpaceRect));
    addAndMakeVisible(this->chartMenu.get());
    replaceCurrentMenu(this->chartMenu.get());

}
void LeftBarObjectMenu::playerAction()
{
    showExtraMenu(PLAYERMENU);
    this->playerMenu.reset(new PlayerMenuComponent(getStr(), this->menuSpaceRect));
    addAndMakeVisible(this->playerMenu.get());
    replaceCurrentMenu(this->playerMenu.get());
}
void LeftBarObjectMenu::objectAction()
{
    showExtraMenu(OBJECTMENU);
    this->objectMenu.reset(new ObjectsMenuComponent(getStr(), this->menuSpaceRect));
    addAndMakeVisible(this->objectMenu.get());
    replaceCurrentMenu(this->objectMenu.get());
}

void LeftBarObjectMenu::inspectorAction()
{
    showExtraMenu(INSPECTORMENU);

    this->inspectorMenu.reset(new InspectorMenuComponent(getStr(), this->menuSpaceRect));
    addAndMakeVisible(this->inspectorMenu.get());
    replaceCurrentMenu(this->inspectorMenu.get());
}
void LeftBarObjectMenu::preferenceAction()
{
    showExtraMenu(PREFERENCEMENU);

    this->preferenceMenu.reset(new PreferenceMenuComponent(getStr(), this->menuSpaceRect));
    addAndMakeVisible(this->preferenceMenu.get());
    replaceCurrentMenu(this->preferenceMenu.get());
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
