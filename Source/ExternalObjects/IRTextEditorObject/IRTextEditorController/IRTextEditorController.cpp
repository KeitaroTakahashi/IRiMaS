//
//  IRTextEditorController.cpp
//  IRiMaS
//
//  Created by Keitaro on 02/10/2019.
//

#include "IRTextEditorController.hpp"


IRTextEditorController::IRTextEditorController(IRStr* str) : IRObjectController(str)
{
    addAndMakeVisible(&this->LabelTitle);
    this->LabelTitle.setText("TextEditor", dontSendNotification);
    this->LabelTitle.setFont(getStr()->SYSTEMCOLOUR.h3);
    this->LabelTitle.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    this->LabelTitle.setJustificationType(Justification::left);
    
    
    this->fontController = std::make_unique<FontController>(str);
    addAndMakeVisible(this->fontController.get());
    this->fontController->setVisible(true);
    this->fontController->addChangeListener(this);
    
    this->arrangeController = std::make_unique<ArrangeController>(str);
    addAndMakeVisible(this->arrangeController.get());
    this->arrangeController->setVisible(false);
    this->arrangeController->addChangeListener(this);
}

IRTextEditorController::~IRTextEditorController()
{
    this->fontController.reset();
    this->arrangeController.reset();
}

void IRTextEditorController::resized()
{
    
    IRObjectController::resized();
    int y = 10;
    int yIncrement = 30;
    int yBigIncrement = 40;
    
    this->LabelTitle.       setBounds(10, y, 300, 30);
    
    y += yBigIncrement;
    y += yIncrement;
    this->fontController->setBounds(0, y, getWidth(), getHeight() - y);
    this->arrangeController->setBounds(0, y, getWidth(), getHeight() - y);
}

void IRTextEditorController::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    g.setColour(Colours::black);
    g.drawLine(0,42.5,getWidth(),42.5);
    //g.drawLine(0,250, getWidth(), 250);
    //g.drawLine(0,400,getWidth(),400);
}


FontController* IRTextEditorController::getFontController()
{
    return this->fontController.get();
}

void IRTextEditorController::changeListenerCallback (ChangeBroadcaster* source)
{
    
}


void IRTextEditorController::mainControllerSelected()
{
    this->fontController->setVisible(true);
    this->arrangeController->setVisible(false);
}

void IRTextEditorController::arrangeControllerSelected()
{
    this->fontController->setVisible(false);
    this->arrangeController->setVisible(true);
}
