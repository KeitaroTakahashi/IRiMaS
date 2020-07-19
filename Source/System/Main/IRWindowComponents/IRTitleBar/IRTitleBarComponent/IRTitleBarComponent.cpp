//
//  IRTitleBarComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 01/10/2019.
//

#include "IRTitleBarComponent.hpp"

IRTitleBarComponent::IRTitleBarComponent(IRStr* str, String title) :
IRStrComponent(str),
title(title), editModeButton("to Control")
{
    setOpaque(false);
    this->titleLabel.setText(title, dontSendNotification);
    this->titleLabel.setJustificationType(Justification::centred);
    this->titleLabel.setFont(getStr()->SYSTEMCOLOUR.h4);
    this->titleLabel.addMouseListener(this, true);
    addAndMakeVisible(&this->titleLabel);
    
    /*
    createButton(&this->closeButton, getStr()->ICONBANK.icon_close);
    this->closeButton.onClick = [this] { closeButtonAction(); };
    this->closeButton.setDrawCircle(false);
    this->closeButton.setDrawRoundedSquare(false);
    addAndMakeVisible(&this->closeButton);
     */
    
    createButton(&this->rightBarButton, getStr()->ICONBANK->icon_rightBar);
    this->rightBarButton.onClick = [this]{ rightBarButtonAction(); };
    this->rightBarButton.setDrawCircle(false);
    this->rightBarButton.setDrawRoundedSquare(true);
    addAndMakeVisible(&this->rightBarButton);
    
    this->rightBarLabel.setText("Controller", dontSendNotification);
    addAndMakeVisible(&this->rightBarLabel);
    this->rightBarLabel.setJustificationType(Justification::centred);
    this->rightBarLabel.setFont(getStr()->SYSTEMCOLOUR.h5);

    this->projectButtonComponent.reset( new IRTitleBarProjectButtonArea(getStr()) );
    addAndMakeVisible(this->projectButtonComponent.get());

    /*
    
    createButton(&this->newProjectButton, getStr()->ICONBANK.icon_newProject);
    this->newProjectButton.onClick = [this]{ newProjectButtonAction(); };
    this->newProjectButton.setDrawRoundedSquare(true);
    addAndMakeVisible(&this->newProjectButton);
    this->newProjectButton.setDrawCircle(false);
    
    this->newProjectLabel.setText("New", dontSendNotification);
    addAndMakeVisible(&this->newProjectLabel);
    this->newProjectLabel.setJustificationType(Justification::centred);
    this->newProjectLabel.setFont(getStr()->SYSTEMCOLOUR.h5);
    
    this->newSlideButton.reset(new IRImageButton("newSlide"));
    createButton(this->newSlideButton.get(), getStr()->ICONBANK.icon_newSlide);
    this->newSlideButton->onClick = [this]{ newSlideButtonAction(); };
    this->newSlideButton->setDrawRoundedSquare(true);
    addAndMakeVisible(this->newSlideButton.get());
    this->newSlideButton->setDrawCircle(false);
   
    this->newSlideLabel.setText("Add", dontSendNotification);
    addAndMakeVisible(&this->newSlideLabel);
    this->newSlideLabel.setJustificationType(Justification::centred);
    this->newSlideLabel.setFont(getStr()->SYSTEMCOLOUR.h5);

    createButton(&this->saveProjectButton, getStr()->ICONBANK.icon_saveProject_arrow);
    this->saveProjectButton.onClick = [this]{ saveProjectButtonAction(); };
    this->saveProjectButton.setDrawRoundedSquare(true);
    addAndMakeVisible(&this->saveProjectButton);
    this->saveProjectButton.setDrawCircle(false);
    
    this->saveProjectLabel.setText("Save", dontSendNotification);
    addAndMakeVisible(&this->saveProjectLabel);
    this->saveProjectLabel.setJustificationType(Justification::centred);
    this->saveProjectLabel.setFont(getStr()->SYSTEMCOLOUR.h5);
    
    createButton(&this->saveasProjectButton, getStr()->ICONBANK.icon_saveasProject_arrow);
    this->saveasProjectButton.onClick = [this]{ saveasProjectButtonAction(); };
    addAndMakeVisible(&this->saveasProjectButton);
    this->saveasProjectButton.setDrawCircle(false);
    this->saveasProjectButton.setDrawRoundedSquare(true);
    
    this->saveasProjectLabel.setText("Save As", dontSendNotification);
    addAndMakeVisible(&this->saveasProjectLabel);
    this->saveasProjectLabel.setJustificationType(Justification::centred);
    this->saveasProjectLabel.setFont(getStr()->SYSTEMCOLOUR.h5);

    createButton(&this->openProjectButton, getStr()->ICONBANK.icon_openProject_arrow);
    this->openProjectButton.onClick = [this]{ openProjectButtonAction(); };
    addAndMakeVisible(&this->openProjectButton);
    this->openProjectButton.setDrawCircle(false);
    this->openProjectButton.setDrawRoundedSquare(true);
    
    this->openProjectLabel.setText("Open", dontSendNotification);
    addAndMakeVisible(&this->openProjectLabel);
    this->openProjectLabel.setJustificationType(Justification::centred);
    this->openProjectLabel.setFont(getStr()->SYSTEMCOLOUR.h5);

      */
    addAndMakeVisible(&this->editModeButton);
    this->editModeButton.onClick = [this]{ editModeButtonAction(); };
    this->editModeLabel.setText("Mode", dontSendNotification);
    addAndMakeVisible(&this->editModeLabel);
    this->editModeLabel.setJustificationType(Justification::centred);
    this->editModeLabel.setFont(getStr()->SYSTEMCOLOUR.h5);

}

IRTitleBarComponent::~IRTitleBarComponent()
{
    
}


//==================================================
void IRTitleBarComponent::paint (Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    /*
    g.setColour(getStr()->SYSTEMCOLOUR.contents);
    g.drawLine(0, getHeight() , getWidth(), getHeight(), 2);
    g.setColour(getStr()->SYSTEMCOLOUR.text);
    Font f("Avenir Next",34, Font::plain);
    g.setFont(f);
    g.drawText("TiAALS", 30, 10, 200, getHeight()*0.8, dontSendNotification);
    
    g.setColour(getStr()->SYSTEMCOLOUR.contents);
    g.drawLine(135, getHeight(), 135, 0, 2);
    g.drawLine(304, getHeight(), 304, 0, 2);
    */
    
    //g.drawImage(getStr()->ICONBANK.icon_newSlide.white, Rectangle<int> (0, 0, 100,100));
    
    //g.drawImage(getStr()->ICONBANK.icon_newProject.white, Rectangle<float> (0, 0, 100,100));
    
    paintSelectedItem(g);

    //heavyWeightRepaint("----------- IRTitleBarComponent repaint");

    //std::cout << " ++++++++ IRTitleBarComponent repainted! ++++++++\n";

}

void IRTitleBarComponent::resized()
{
    
    this->titleLabel.setBounds(0, 0, getWidth(), 30);
    this->titleLabel.setCentrePosition(getWidth()/2, 15);
    
    int x = 10;
    //int w = getHeight() * 0.9;
    int h = getHeight() * 0.41;

    //this->buttonSize =juce::Point<int> (w, (float)w * this->rightBarButton.getAspectRatio());
    this->buttonSize =juce::Point<int> (((float)h / this->rightBarButton.getAspectRatio()) * 1.2, h);
    
    //this->closeButton.setBounds(5, 5, 20, 20);

    x = getWidth();
    x -= (this->buttonSize.getX() + marginX);
    int marginY = (getHeight() - this->buttonSize.getY())/2;
    int labelY = getHeight() - marginY + 3;
    int labelHeight = marginY * 0.5;

    this->rightBarButton.setBounds(x,
                                   marginY,
                                   this->buttonSize.getX(),
                                   this->buttonSize.getY());
    
    
    int halfMarginX = marginX / 2;
    this->rightBarLabel.setBounds(x - halfMarginX,
                                  labelY,
                                  this->buttonSize.getX() + marginX,
                                  labelHeight);
    
    x -= (this->buttonSize.getX()*1.6 + marginX);
    this->editModeButton.setBounds(x,
                                   marginY,
                                   this->buttonSize.getX() * 1.6,
                                   this->buttonSize.getY());
    this->editModeLabel.setBounds(x,
                                  labelY,
                                  this->buttonSize.getX() * 1.6,
                                  labelHeight);
    
    x = 145; // leftBar width + 10
    //Point<int> bs =juce::Point<int> ((float)h / this->newProjectButton.getAspectRatio(), h);
    
    this->projectButtonComponent->setBounds(0, 0, 600, getHeight());
    
}


void IRTitleBarComponent::paintSelectedItem(Graphics& g)
{
    if(this->selectedButtonType != NONE)
    {
        int marginY = (getHeight() - this->buttonSize.getY())/2;

        g.setColour(getStr()->SYSTEMCOLOUR.contents);
        int index = (int)this->selectedButtonType;
        int y = marginY / 2;
        int x = getWidth() - (this->marginX/2 + (index + 1) * (this->buttonSize.getX() + this->marginX));
        // width should be bigger to hide the right side rounded corner
        int h = getHeight() * 1.2;
        int w = this->buttonSize.getX() + marginX;
        g.fillRoundedRectangle(x, y, w, h, 5);
    }
}

//==================================================


void IRTitleBarComponent::createButton(IRImageButton* button, IRIconBank::IRIconImage img)
{
    if(getStr()->SYSTEMCOLOUR.isWhiteBased)
        button->setImage(img.black);
    else
        button->setImage(img.white);
    
    //button->setDrawCircle(false);
    addAndMakeVisible(button);
}
//==================================================


void IRTitleBarComponent::rightBarButtonAction()
{
    
    this->rightBarButton.setSelected(!this->rightBarButton.isSelected());
    
    if(this->rightBarButton.isSelected())
        this->selectedButtonType = rightBar;
    else
        this->selectedButtonType = NONE;
 
    if(this->rightBarButtonCallback != nullptr)
        this->rightBarButtonCallback(this->rightBarButton.isSelected());
    repaint();
}

void IRTitleBarComponent::editModeButtonAction()
{
    if(this->editModeButtonCallback != nullptr)
        this->editModeButtonCallback();
}
/*
void IRTitleBarComponent::closeButtonAction()
{
    if(this->closeButtonCallback != nullptr)
        this->closeButtonCallback();
}



void IRTitleBarComponent::saveProjectButtonAction()
{
    if(this->saveButtonCallback != nullptr)
        this->saveButtonCallback();
}

void IRTitleBarComponent::saveasProjectButtonAction()
{
    if(this->saveasButtonCallback != nullptr)
        this->saveasButtonCallback();
}

void IRTitleBarComponent::newProjectButtonAction()
{
    if(this->newProjectButtonCallback != nullptr)
        this->newProjectButtonCallback();
}

void IRTitleBarComponent::newSlideButtonAction()
{
    if(this->newSlideButtonCallback != nullptr)
        this->newSlideButtonCallback();
}

void IRTitleBarComponent::openProjectButtonAction()
{
    if(this->openButtonCallback != nullptr)
           this->openButtonCallback();
}
*/
//==================================================


void IRTitleBarComponent::setEditMode(bool flag)
{
    if(flag)
    {
        this->editModeButton.setText("to Control");
    }else
    {
        this->editModeButton.setText("to Edit");
    }
}

//==================================================


void IRTitleBarComponent::setTitle(String newTitle)
{
    this->title = newTitle;
    this->titleLabel.setText(newTitle, dontSendNotification);
    repaint();
}
