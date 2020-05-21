//
//  IRWorkspaceComponentKeyEvents.cpp
//  IRiMaS
//
//  Created by Keitaro on 18/09/2019.
//

#include "IRWorkspaceComponent.hpp"

//bool IRWorkspaceComponent::keyPressed (const KeyPress& key)
bool IRWorkspaceComponent::keyPressed (const KeyPress& key, Component* originatingComponent)
{
    std::cout << "IRWorkspaceComponent keyPressed() : " << key.getKeyCode() << " : " << key.getTextDescription() << ", " << key.getTextCharacter() <<   std::endl;
    
  // delete key
    if(key.getKeyCode() == key.deleteKey || key.getKeyCode() == key.backspaceKey)
    {
        if(isEditMode())
            deleteSelectedObjects();
        return true;
    }
    
    if(key.getTextDescription() == "command + C")
    {
        if(this->isEditMode())
        {
            copySelectedObjects();
        }
        return true;
    }
    
    if(key.getTextDescription() == "command + V")
    {
        if(this->isEditMode())
        {
            pasteSelectedObjects();
        }
        return true;
    }
    
    if(key.getTextDescription() == "command + D")
    {
        if(this->isEditMode())
        {
            duplicateSelectedObjects();
        }
        return true;
    }
    
    return false;
}



// ==================================================


void IRWorkspaceComponent::registerKeyListener(KeyListener* key)
{
    addKeyListener(key);
}
void IRWorkspaceComponent::registerMouseListener(MouseListener* mouse)
{
    addMouseListener(mouse, false);
}
