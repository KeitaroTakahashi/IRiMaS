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
    IRUIFoundation();
    ~IRUIFoundation();
};

#endif /* IRUIFoundation_hpp */
