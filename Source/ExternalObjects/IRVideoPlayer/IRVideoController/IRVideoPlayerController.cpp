//
//  IRVideoController.cpp
//  IRiMaS
//
//  Created by Keitaro on 24/10/2019.
//

#include "IRVideoPlayerController.hpp"


IRVideoPlayerController::IRVideoPlayerController(IRStr* str) : IRObjectController(str),
UI(str)
{
    addAndMakeVisible(&this->LabelTitle);
    this->LabelTitle.setText("VideoPlayer", dontSendNotification);
    this->LabelTitle.setFont(getStr()->SYSTEMCOLOUR.h3);
    this->LabelTitle.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    this->LabelTitle.setJustificationType(Justification::left);
    
    addAndMakeVisible(&this->UI);
    this->UI.addChangeListener(this);
}
IRVideoPlayerController::~IRVideoPlayerController()
{
    
}

void IRVideoPlayerController::resized()
{
    int y = 10;
    int yIncrement = 30;
    int yBigIncrement = 40;
    
    this->LabelTitle.       setBounds(10, y, getWidth() - 20, 30);
    
    y += yBigIncrement;
    
    this->UI.               setBounds(0, y, getWidth(), getHeight() - y);
}
void IRVideoPlayerController::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    g.setColour(Colours::black);
    g.drawLine(0,42.5,getWidth(),42.5);
}

void IRVideoPlayerController::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == &this->UI)
    {
        switch (this->UI.getStatus())
        {
            case VideoController::OpenMovieFile:
                this->status = OpenMovieFile;
                sendChangeMessage();
                break;
                
            default:
                break;
        }
    }
}
