//
//  IRWaveformController2.cpp
//  IRiMaS
//
//  Created by Keitaro on 10/10/2019.
//

#include "IRWaveformController2.hpp"


IRWaveformController2::IRWaveformController2(IRStr* str) : IRObjectController("Waveform", str),
audioController(str)
{
    
    
    addAndMakeVisible(&this->LabelTitle);
    this->LabelTitle.setText("Waveform", dontSendNotification);
    this->LabelTitle.setFont(getStr()->SYSTEMCOLOUR.h3);
    this->LabelTitle.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    this->LabelTitle.setJustificationType(Justification::left);
    
    addAndMakeVisible(&this->audioController);

}

IRWaveformController2::~IRWaveformController2()
{
    
}


void IRWaveformController2::ControllerResized()
{
    int y = 10;
    //int yIncrement = 30;
    int yBigIncrement = 40;
    
    this->LabelTitle.       setBounds(10, y, 300, 30);
    
    y += yBigIncrement;
    
    this->audioController.setBounds (0, y, getWidth(), 300);
    
    
}

void IRWaveformController2::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    g.setColour(getStr()->SYSTEMCOLOUR.fundamental);
    g.drawLine(0,42.5,getWidth(),42.5, 2);
    
}

void IRWaveformController2::changeListenerCallback(ChangeBroadcaster* source)
{
   
}
