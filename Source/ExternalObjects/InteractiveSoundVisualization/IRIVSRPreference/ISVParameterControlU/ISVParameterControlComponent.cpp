//
//  ISVParameterControlComponent.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/06/2019.
//

#include "ISVParameterControlComponent.hpp"

ISVParameterControlComonent::ISVParameterControlComonent()
{
    addAndMakeVisible(this->ui);
}

ISVParameterControlComonent::~ISVParameterControlComonent()
{
    
}

// ==================================================
void ISVParameterControlComonent::paint(Graphics& g)
{
    
}

void ISVParameterControlComonent::resized()
{
    this->ui->setBounds(0, 0, getWidth(), getHeight());
}

// ==================================================
