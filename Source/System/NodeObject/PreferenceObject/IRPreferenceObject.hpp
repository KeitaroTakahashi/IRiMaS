
#ifndef IRPreferenceObject_hpp
#define IRPreferenceObject_hpp


#include "JuceHeader.h"
#include "IRComponents.hpp"

class IRPreferenceObject : public Component,
public IRComponents,
public ChangeBroadcaster,
public ChangeListener
{
    
public:
    
    IRPreferenceObject(String title, Rectangle<int> frameRect);
    ~IRPreferenceObject();
    
    virtual void resized() override;
    virtual void paint(Graphics& g) override;
    
    virtual void changeListenerCallback(ChangeBroadcaster* source) override;
    
private:
    Rectangle<int> frameRect;
};



#endif /* IRPreferenceObject_hpp */




