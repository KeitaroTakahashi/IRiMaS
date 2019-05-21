//
//  IRAutomation.cpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 02/05/2019.
//

#include "IRAutomationUI.hpp"

IRAutomationUI::IRAutomationUI(IRNodeObject* nodeObject) : IRUIFoundation(nodeObject)
{
    this->automation = std::make_shared<InteractiveAutomation>(nodeObject);
    addAndMakeVisible(&this->automationView);
    this->automationView.setViewedComponent(this->automation.get());
    
    this->automation->zoomInClickedCallback = [this]{ zoomInClicked(); };
    this->automation->zoomOutClickedCallback = [this]{ zoomOutClicked(); };
    this->automation->movableClickedCallback = [this](IRAutomation::movableStatus status){ movableClicked(status); };
    
}


IRAutomationUI::~IRAutomationUI()
{
    
}

// ==================================================

void IRAutomationUI::resized()
{
    int w = getWidth() - this->xMargin*2;
    int h = getHeight() - this->yMargin*2;
    
    this->automationView.setBounds(this->xMargin, this->yMargin, w, h);
    this->automation->setBounds(this->xMargin, this->yMargin, w * this->automation_width_ratio, h - 20);
    
}
// ==================================================

void IRAutomationUI::zoomInClicked()
{
    std::cout << "zoomInClicked\n";
    this->automation_width_ratio *= 2;
    resized();
    this->automation->reCalcPos();
}

void IRAutomationUI::zoomOutClicked()
{
    std::cout << "zoomOutClicked\n";
    this->automation_width_ratio /= 2;
    resized();
    this->automation->reCalcPos();
    
}

void IRAutomationUI::movableClicked(IRAutomation::movableStatus status)
{
    std::cout << "movableClicked : " << status << std::endl;
    
    switch(status)
    {
        case NOTMOVABLE:
            setMovable(false, false, false);
            break;
        case HORIZONTALMOVABLE:
            setMovable(true, false, true);
            break;
        case VERTICALMOVABLE:
            setMovable(true, true, false);
            break;
        case CROSSMOVABLE:
            setMovable(true, true, true);
            break;
        default:
            break;
            
    }
    
    
    
}

void IRAutomationUI::setMovable(bool movable, bool verticalMovable, bool horizontalMovable)
{
    
    Array<vertex*> vs = this->automation->getVerteces();
    
    for(auto v : vs)
    {
        v->setMovable(movable, verticalMovable, horizontalMovable);
    }
    
    this->automation->setMovable(movable, verticalMovable, horizontalMovable);
    
}

void IRAutomationUI::verticalMovableClicked()
{
    
}

void IRAutomationUI::horizontalMovableClicked()
{
    
}
// ==================================================


void IRAutomationUI::demoData(int num)
{
    this->automation->demoData(num);
    //this->controller->setMovableStatus(IRAutomation::movableStatus::VERTICALMOVABLE);
}
