
#ifndef ObjectFactoryInitializer_hpp
#define ObjectFactoryInitializer_hpp

#include "ExternalObjectHeader.h"
#include "IRNodeObject.hpp"

/*
 
 Initialize object factory.
 
 Register all IRObjects here with an indentity image file which is shown in the object menu.
 
 registerObject<object type>(object registration name, object name, menu image file)
 
 The order of the registration appears to the order of the menu.
 */



class ObjectFactoryInitializer
{
    
public:
    
    ObjectFactoryInitializer();
    
    
private:
    
    IRObjectFactory& objectFactory { singleton<IRObjectFactory>::get_instance() };
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ObjectFactoryInitializer)
    
};





#endif /* ObjectFactoryInitializer_hpp */





