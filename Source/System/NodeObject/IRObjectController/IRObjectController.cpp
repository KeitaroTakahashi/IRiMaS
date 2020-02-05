//
//  IRObjectController.cpp
//  IRiMaS
//
//  Created by Keitaro on 02/10/2019.
//

#include "IRObjectController.hpp"

IRObjectController::IRObjectController(IRStr* str) : IRStrComponent(str),
switchButton(str)
{
    addMouseListener(str->projectOwner, false);
    this->switchButton.createButton("Main", this);
    this->switchButton.createButton("Arrange", this);
    
    addAndMakeVisible(this->switchButton);
   
}

IRObjectController::~IRObjectController()
{
    
}


void IRObjectController::resized()
{
    this->switchButton.setBounds(0, 42, getWidth(), 30);
}

void IRObjectController::buttonClicked(IRSquareButton* clickedButton)
{
    
    this->switchButton.selectAButton(clickedButton);
    
    if(clickedButton == this->switchButton.getSquareButtonAt("Main"))
    {
        mainControllerSelected();
    }else if(clickedButton == this->switchButton.getSquareButtonAt("Arrange"))
    {
        arrangeControllerSelected();
    }
}


