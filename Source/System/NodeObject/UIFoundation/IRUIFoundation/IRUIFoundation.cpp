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
    
    this->pressedKeyCode = key.getKeyCode();
    IRKeyPressed(this->pressedKeyCode);
    //std::cout << "IRUIFoundation keyPressed = " << key.getKeyCode() << std::endl;

    return true;
}
