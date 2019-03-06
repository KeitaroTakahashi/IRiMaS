
#ifndef IRObjectCreater_hpp
#define IRObjectCreater_hpp

#include "JuceHeader.h"

#include "IRObjectCreaterBase.hpp"
#include "IRNodeObject.hpp"


template<class T>
class IRObjectCreater : public IRObjectCreaterBase
{
    
public:
    IRObjectCreater() {}
    ~IRObjectCreater() {}

    IRNodeObject* create(Component* parent) override
    {
        return new T(parent);
    }
    
private:
    
};


#endif /* IRObjectCreater_hpp */




