//
//  IRMainSpace.cpp
//  IRiMaS
//
//  Created by Keitaro on 27/08/2019.
//

#include "IRMainSpace.hpp"

IRMainSpace::IRMainSpace(IRStr* str)
{
    this->ir_str = str;
    createNewWorkspace();
}

IRMainSpace::~IRMainSpace()
{
    
}
//==================================================

void IRMainSpace::paint(Graphics& g)
{
    
}

void IRMainSpace::resized()
{
    
    
    
    
    for(auto space : this->workspaces)
    {
        space->setBounds(getLocalBounds());
    }
}
//==================================================

void IRMainSpace::changeListenerCallback (ChangeBroadcaster* source)
{
    
    
}

//==================================================

void IRMainSpace::updateAnimationFrame()
{
    
}

// ==================================================
// WORKSPACE
// ==================================================

void IRMainSpace::createNewWorkspace()
{
    
    std::cout << "create IRWOrkspace\n";
    
    String title = this->ir_str->projectName + "_" + String(this->workspaces.size() + 1);
    std::cout << "create IRWOrkspace\n";
    
    //Rectangle<int> frameRect = this->mainComp->getBounds();
    
    std::cout << "create IRWOrkspace\n";
    IRWorkSpace* space = new IRWorkSpace(title, this->ir_str, nullptr);
    
}

// ==================================================
// MOUSE EVENT
// ==================================================
void IRMainSpace::mouseDown(const MouseEvent& e)
{
    std::cout << "MainSpace mouseDown\n";
}
