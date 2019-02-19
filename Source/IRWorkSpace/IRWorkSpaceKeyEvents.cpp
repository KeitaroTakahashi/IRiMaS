//
//  IRWorkSpaceKeyEvents.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 23/08/2018.
//

#include "IRWorkSpace.hpp"

//bool IRWorkSpace::keyPressed (const KeyPress& key)
bool IRWorkSpace::keyPressed (const KeyPress& key, Component* originatingComponent)
{
    std::cout << "IRWorkSpace keyPressed() : " << key.getKeyCode() << " : " << key.getTextDescription() << ", " << key.getTextCharacter() <<   std::endl;
    /*
     switch (key.getTextCharacter())
     {
     case 'command + C':
     if(this->isCommandPressed) copySelectedObjects();
     case 'command + V':
     if(this->isCommandPressed) pasteSelectedObjects();
     case 'command + D':
     if(this->isCommandPressed) duplicateSelectedObjects();
     default:
     break;
     }
     */
    
    
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
    
    if(key.getTextDescription() == "command + E")
    {
        // switch edit mode
        setEditMode(! this->editModeFlag);
        return true;
    }
    
    return false;
}
