//
//  IROnVideoPlayerController.cpp
//  IRiMaS
//
//  Created by Keitaro on 30/06/2020.
//

#include "IROnVideoPlayerController.hpp"


IROnVideoPlayerController::IROnVideoPlayerController(IRStr* str) :
IRStrComponent(str),
IRHeavyWeightComponent(this, "IROnVideoPlayerController")
{
    
}


IROnVideoPlayerController::~IROnVideoPlayerController()
{
    
}
// ==================================================
void IROnVideoPlayerController::resized()
{
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
// ==================================================
// ==================================================
// ==================================================
