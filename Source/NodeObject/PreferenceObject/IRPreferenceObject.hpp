//
//  IRPreferenceObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 22/10/2018.
//

#ifndef IRPreferenceObject_hpp
#define IRPreferenceObject_hpp

#include <stdio.h>
#include "JuceHeader.h"
// #include "juce_audio_utils/juce_audio_utils.h"

class IRPreferenceObject : public Component,
public ChangeBroadcaster,
public ChangeListener
{
public:
    IRPreferenceObject(String title, Rectangle<int> frameRect)
    {
        this->frameRect = frameRect;
        setBounds(frameRect.getX(), frameRect.getY(), frameRect.getWidth(), frameRect.getHeight());
    }
    
    ~IRPreferenceObject()
    {
        
    }
    
    //===============================================================
    
    virtual void resized() override
    {
        setBounds(frameRect.getX(), frameRect.getY(), frameRect.getWidth(), frameRect.getHeight());

    }
    virtual void paint(Graphics& g) override
    {
        g.fillAll(Colours::white);
        
    }
    
    // --------------------------------------------------------------
    //===============================================================
    //Listener
    virtual void changeListenerCallback (ChangeBroadcaster* source) override {}
    
    //===============================================================

    
    
private:
    Rectangle<int> frameRect;
};

#endif /* IRPreferenceObject_hpp */
