//
//  IRWIndowComponentKeyEvent.cpp
//  IRiMaS
//
//  Created by Keitaro on 15/11/2019.
//

#include "IRWindowComponent.hpp"

void IRWindowComponent::DeleteKeyPressed()
{
    
}

void IRWindowComponent::CommandEPressed()
{

     setEditMode(!this->isEditMode());

}

void IRWindowComponent::CommandSPressed()
{
    saveProject();
    
}

void IRWindowComponent::CommandNPressed()
{
    createNewNodeObject();
}


