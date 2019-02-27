
#ifndef IRObjectCreaterBase_hpp
#define IRObjectCreaterBase_hpp

#include "JuceHeader.h"

#include "IRNodeObject.hpp"


class IRObjectCreaterBase
{
    
public:
    IRObjectCreaterBase();
    virtual ~IRObjectCreaterBase();
    
    virtual IRNodeObject* create(Component* parent) = 0;
    std::string objectName;
    
};


#endif /* IRObjectCreaterBase_hpp */
