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
    std::cout << "create IRWorkspace\n";
    String title = this->ir_str->projectName + "_" + String(this->workspaces.size() + 1);    
    IRWorkspace* space = new IRWorkspace(title, this->ir_str, nullptr);
    this->workspaces.push_back(space);
    
    this->topWorkspace = space;
    
    // store a pointer of the current top workspace
    this->ir_str->TopWorkspace = this->topWorkspace;
    
    addAndMakeVisible(space);
}

// ==================================================
// MOUSE EVENT
// ==================================================
void IRMainSpace::mouseDown(const MouseEvent& e)
{
    std::cout << "MainSpace mouseDown\n";
}
