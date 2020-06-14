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

void IRWindowComponent::giveKeyFocusOnTopWorkspace()
{
    if(this->mainSpace.get() != nullptr)
    {
        auto w = this->mainSpace->getTopWorkspace();
        if(w != nullptr)
        {
            this->leftBar->setWantsKeyboardFocus(false);
            w->setWantsKeyboardFocus(true);
        }
    }
}


