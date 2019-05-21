//
//  AutomationController.cpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 02/05/2019.
//

#include "AutomationController.hpp"

AutomationController::AutomationController()
{
    this->zoomInButton.setImage(this->ImageBank.getZoomIn());
    addAndMakeVisible(&this->zoomInButton);
    this->zoomInButton.onClick = [this] {zoomInClicked(); };
    
    this->zoomOutButton.setImage(this->ImageBank.getZoomOut());
    addAndMakeVisible(&this->zoomOutButton);
    this->zoomOutButton.onClick = [this] {zoomOutClicked(); };
    
    addAndMakeVisible(&this->movableButton);
    
    
    std::cout << "zoomIn width = " << this->ImageBank.getZoomIn().getWidth() << std::endl;

}

AutomationController::~AutomationController()
{
    
}

// ==================================================

void AutomationController::resized()
{
    int x = this->buttonInterval;
    int y = 0;
    int s = getHeight();
    if(s > 100) s = 100;
    
    this->zoomInButton.setBounds(x, y, s, s);
    this->zoomOutButton.setBounds((x * 2) + s, y, s, s);
    this->movableButton.setBounds((x * 3) + (s * 2), y, s, s);
    
    
}
void AutomationController::paint(Graphics& g)
{
    g.setColour(Colours::grey);
    g.setOpacity(0.6);
    g.fillRect(getLocalBounds());}

// ==================================================

void AutomationController::zoomInClicked()
{
    
}

void AutomationController::zoomOutClicked()
{
    
}

// ==================================================

void AutomationController::setZoomInEvent(std::function<void()> callBack)
{
    this->zoomInButton.onClick = callBack;
}
// --------------------------------------------------
void AutomationController::setZoomOutEvent(std::function<void()> callBack)
{
    this->zoomOutButton.onClick = callBack;

}

// --------------------------------------------------
void AutomationController::setMovableEvent(std::function<void(IRAutomation::movableStatus)> callBack)
{
    this->movableButton.statusClicked = callBack;
}


// ==================================================

void AutomationController::setMovableStatus(IRAutomation::movableStatus status)
{
    this->movableButton.setMovableStatus(status);
}
