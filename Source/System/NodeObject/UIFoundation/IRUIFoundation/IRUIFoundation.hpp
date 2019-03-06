//
//  IRUIFoundation.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 04/03/2019.
//

#ifndef IRUIFoundation_hpp
#define IRUIFoundation_hpp

#include "JuceHeader.h"
#include "IRNodeObject.hpp"

class IRUIFoundation : public Component,
public IRNodeObject::Listener,
public KeyListener
{
public:
    IRUIFoundation();
    ~IRUIFoundation();
    
    
    // --------------------------------------------------
    virtual void IRKeyPressed(int keyCode);
    virtual void IRKeyReleased(int keyCode);
    bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override;
    
    bool keyPressed(const KeyPress &key,
                    Component* originatingComponent) override;
    
    // --------------------------------------------------
private:
    
    //key event
    int pressedKeyCode;
    
};

#endif /* IRUIFoundation_hpp */
