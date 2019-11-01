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
    this->fontController->addChangeListener(this);
}

IRTextEditorController::~IRTextEditorController()
{
   
    
}

void IRTextEditorController::resized()
{
    int y = 10;
    int yIncrement = 30;
    int yBigIncrement = 40;
    
    this->LabelTitle.       setBounds(10, y, 300, 30);
    
    y += yBigIncrement;
    
    this->fontController->setBounds(0, y, getWidth(), getHeight() - y);
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
