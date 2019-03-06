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
    virtual void IRKeyPressed();
    virtual void IRKeyReleased();
    bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override;
    
    bool keyPressed(const KeyPress &key,
                    Component* originatingComponent) override;
    
    // --------------------------------------------------
private:
    
    
};

#endif /* IRUIFoundation_hpp */
