//
//  IRVideoAnnotaterKeyEvents.cpp
//  IRiMaS
//
//  Created by Keitaro on 01/03/2020.
//

#include "IRVideoAnnotater.hpp"


bool IRVideoAnnotater::keyPressed(const KeyPress& key, Component* originatingComponent)
{
    
    std::cout << "IRVideoAnnotater::keyPressed\n";
    /*
    if(key.getKeyCode() == key.deleteKey || key.getKeyCode() == key.backspaceKey)
    {
       DeleteKeyPressed();
        return true;
    }
*/
    // open Annotation Event Menu
    if(key.getTextDescription() == "A")
    {
        AKeyPressed();
        return true;
    }
    
    // close window
    if(key.getTextDescription() == "Command + W")
    {
        CommandWKeyPressed();
        return true;
    }
    
    if(key.getTextDescription() == "command + E")
    {
        CommandEPressed();
        return true;
    }
    
    if(key.getTextDescription() == "command + S")
    {
        CommandSPressed();
        return true;
    }

    return false;
}


void IRVideoAnnotater::DeleteKeyPressed()
{
    
}

void IRVideoAnnotater::AKeyPressed()
{
    openAnnotationMenu();
}

void IRVideoAnnotater::CommandWKeyPressed()
{
    closeAnnotationWindow();
}


void IRVideoAnnotater::CommandEPressed()
{
    
    std::cout << "IRVideoAnnotater::CommandEPressed()\n" << std::endl;
    auto w = this->myVideoPlayerObject->getWorkspace();
    w->setEditMode(!w->isEditMode());
}

void IRVideoAnnotater::CommandSPressed()
{
    
}
