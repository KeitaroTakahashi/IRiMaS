//
//  IRUIFoundation.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 04/03/2019.
//

#include "IRUIFoundation.hpp"

IRUIFoundation::IRUIFoundation()
{
    
}
// --------------------------------------------------
IRUIFoundation::~IRUIFoundation()
{
    
}
// --------------------------------------------------
void IRUIFoundation::IRKeyPressed()
{
    
}
// --------------------------------------------------
void IRUIFoundation::IRKeyReleased()
{
    
}
// --------------------------------------------------
bool IRUIFoundation::keyStateChanged(bool isKeyDown, Component* originatingComponent)
{
    
    std::cout << "IRUIFoundation Key state changed = " << isKeyDown << std::endl;
    
    if(isKeyDown) IRKeyPressed();
    else IRKeyReleased();
    
    return true;
    
}
// --------------------------------------------------

bool IRUIFoundation::keyPressed(const KeyPress &key,
                                Component* originatingComponent)
{
    
    
    std::cout << "IRUIFoundation keyPressed = " << key.getKeyCode() << std::endl;

    return true;
}
