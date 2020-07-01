//
//  IROnVideoPlayerController.cpp
//  IRiMaS
//
//  Created by Keitaro on 30/06/2020.
//

#include "IROnVideoPlayerController.hpp"


IROnVideoPlayerController::IROnVideoPlayerController(IRStr* str) :
IRStrComponent(str),
IRHeavyWeightComponent(this, "IROnVideoPlayerController"),
playPauseButton(str)
{
    addAndMakeVisible(&this->playPauseButton);
    this->playPauseButton.onClick = [this]{ playPauseButtonClickedAction(); };
    
}


IROnVideoPlayerController::~IROnVideoPlayerController()
{
    
}
// ==================================================
void IROnVideoPlayerController::resized()
{
    int x = 10;
    int y = 10;
    
    int yIncrement = 30;
    
    this->playPauseButton.setBounds(x, y, 30, 30);
    this->playPauseButton.setCentrePosition(x + this->playPauseButton.getWidth()/2,
                                            getHeight()/2);
    
    y += yIncrement;
    
}

void IROnVideoPlayerController::paint(Graphics& g)
{
    
    std::cout << "IROnVideoPlayerController paint\n";

    g.fillAll(Colours::transparentWhite);
    g.setColour(Colours::lightgrey);
    g.setOpacity(0.6);
    g.fillRoundedRectangle(getLocalBounds().reduced(6).toFloat(), 2);
}

// ==================================================

void IROnVideoPlayerController::setCurrentPlayPosition(float sec)
{
    
}
// ==================================================

void IROnVideoPlayerController::playAction()
{
    
}
void IROnVideoPlayerController::pauseAction()
{
    
}
// ==================================================

void IROnVideoPlayerController::playPauseButtonClickedAction()
{
    auto status = this->playPauseButton.getStatus();
    
    switch(status)
    {
        case IRPlayPauseButton::PLAY:
            break;
        case IRPlayPauseButton::PAUSE:
            break;
        default:
            break;
    }
    
}
// ==================================================

void IROnVideoPlayerController::setStatus(IROnVideoPlayerControllerStatus status)
{
    this->status = status;
}

// ==================================================
// ==================================================
