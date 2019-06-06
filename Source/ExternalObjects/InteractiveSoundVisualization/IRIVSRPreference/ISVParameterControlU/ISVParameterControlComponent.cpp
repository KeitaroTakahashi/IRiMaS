//
//  ISVParameterControlComponent.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/06/2019.
//

#include "ISVParameterControlComponent.hpp"

ISVParameterControlComponent::ISVParameterControlComponent()
{

}

ISVParameterControlComponent::~ISVParameterControlComponent()
{
    
}

// ==================================================
void ISVParameterControlComponent::paint(Graphics& g)
{
    
}

void ISVParameterControlComponent::resized()
{
    this->ui->setBounds(0, 0, getWidth(), getHeight());
}

// ==================================================

void ISVParameterControlComponent::setParameterUI(ISVParameterControlUI* ui)
{
    this->ui = ui;
}
void ISVParameterControlComponent::addParameterUI()
{
    if(this->ui != nullptr) addAndMakeVisible(this->ui);
}

void ISVParameterControlComponent::removeParameterUI()
{
    if(this->ui != nullptr) removeChildComponent(this->ui);
}
// ==================================================
