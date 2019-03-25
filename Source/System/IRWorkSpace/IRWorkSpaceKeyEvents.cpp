
#include "IRWorkSpace.hpp"




//bool IRWorkSpace::keyPressed (const KeyPress& key)
bool IRWorkSpace::keyPressed (const KeyPress& key, Component* originatingComponent)
{
    std::cout << "IRWorkSpace keyPressed() : " << key.getKeyCode() << " : " << key.getTextDescription() << ", " << key.getTextCharacter() <<   std::endl;
    
    
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
        setEditMode(!this->editModeFlag);
        
        // notify it to IRProject
        if(this->notifyEditModeChanged != nullptr)
        {
            this->notifyEditModeChanged();
        }
        
        return true;
    }
    
    // in case no key
    if(! this->isEditMode())
    {
        std::cout << "IRWworkspace : The workspace can not receive any key event because it is in Control Mode, if you want to control Object interface, please click an object you want to control and give a KeyEventFocus on it." << std::endl;
    }
    else {
        std::cout << "IRWorkspace : Unknown KeyEvent received." << std::endl;
    }
    
    return false;
}



