//
//  IRUIFoundation.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 04/03/2019.
//

#include "IRUIFoundation.hpp"

IRUIFoundation::IRUIFoundation(IRNodeObject* parent)
{
    this->parent = parent;
    this->parent->statusChangeCompleted = [this](IRNodeComponentStatus status){ NodeObjectStatusChanged(status); };
    
}
// --------------------------------------------------
IRUIFoundation::~IRUIFoundation()
{
    
}
// --------------------------------------------------
void IRUIFoundation::IRKeyPressed(int keyCode)
{
    std::cout << "IRUIFoundation IRKeyPressed = " << keyCode << std::endl;

}
// --------------------------------------------------
void IRUIFoundation::IRKeyReleased(int keyCode)
{
    std::cout << "IRUIFoundation IRKeyReleased = " << keyCode << std::endl;

}
// --------------------------------------------------
bool IRUIFoundation::keyStateChanged(bool isKeyDown, Component* originatingComponent)
{
    
    ///std::cout << "IRUIFoundation Key state changed = " << isKeyDown << std::endl;
    
    // IRKeyPressed() should be callsed in keyPressed event
    if(!isKeyDown) IRKeyReleased(this->pressedKeyCode);
    
    return true;
    
}
// --------------------------------------------------

bool IRUIFoundation::keyPressed(const KeyPress &key,
                                Component* originatingComponent)
{
    // reserved key commands
    if(key.getTextDescription() == "command + E")
    {
        // inform parent the change of editMode
        parent->setEditMode(!parent->isEditMode());
        parent->callEditModeChangedInNodeObject();
        
        return true;
    }
    // save project key command
    else if(key.getTextDescription() == "command + S")
    {
        //parent->callSaveProject();
        return true;
    }
    // close project key command
    else if(key.getTextDescription() == "command + W")
    {
        
        return true;
    }
    // open project key command
    else if(key.getTextDescription() == "command + O")
    {
        
        return true;
    }
    
    // user defined key commands
    this->pressedKeyCode = key.getKeyCode();
    IRKeyPressed(this->pressedKeyCode);
    //std::cout << "IRUIFoundation keyPressed = " << key.getKeyCode() << std::endl;

    return true;
}

void IRUIFoundation::NodeObjectStatusChanged(IRNodeComponentStatus status)
{
    std::cout << "NodeObjectStatusChanged : status = " << status << std::endl;
    
    switch (status)
    {
        case IRNodeComponentStatus::EditModeStatus:
            
            // inform parent the change of editMode
            //parent->setEditMode(!parent->isEditMode());
            //parent->callEditModeChangedInNodeObject();
            
            break;
            
        default:
            break;
    }
    
}
