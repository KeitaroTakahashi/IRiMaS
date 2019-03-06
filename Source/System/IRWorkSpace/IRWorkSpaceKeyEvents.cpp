
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
        // DO NOT change edit mode here, but IRProject must change the status of this object
        //setEditMode(!this->editModeFlag); <- therefore it is commented out.
        
        // notify it to IRProject
        if(this->notifyEditModeChanged != nullptr)
        {
            this->notifyEditModeChanged();
        }
        
        return true;
    }
    
    return false;
}



