//
//  IRPreferenceSpace.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 17/10/2018.
//

#ifndef IRPreferenceSpace_hpp
#define IRPreferenceSpace_hpp

#include <stdio.h>
#include "JuceHeader.h"
// #include "juce_audio_utils/juce_audio_utils.h"
#include "Preferences.h"
#include "IRPreferenceObject.hpp"

class IRPreferenceSpace : public Component,
public ChangeBroadcaster,
public ChangeListener
{
public:
    IRPreferenceSpace(String title, Rectangle<int> frameRect)
    {
        this->title = title;
        setSize (frameRect.getWidth(), frameRect.getHeight());
        

    }
    
    ~IRPreferenceSpace()
    {
        
    }
    
    void resized() override
    {
        if(this->isPrefereneObjAlreadySet == true){
            preferenceObj->setBounds(10, 50, 100, 400);
        }
    }
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::white);
        
    }
    String getTitle() { return this->title; }
    

    // ---------------------------------------------------------------------------
    //==============================================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override {}
    
    //==============================================================================
    
    void setPreferenceObj(IRPreferenceObject* preferenceObj)
    {
        removeAllChildren();
        addAndMakeVisible(preferenceObj);
        this->preferenceObj = preferenceObj;
        preferenceObj->setBounds(10, 50, 100, 400);

        this->isPrefereneObjAlreadySet = true;

    }
    IRPreferenceObject* getPreferenceObj()
    {
        return this->preferenceObj;
    }
    
    //==============================================================================

private:
    String title;
    bool isPrefereneObjAlreadySet = false;
    
    IRPreferenceObject* preferenceObj;
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRPreferenceSpace)

};

#endif /* IRPreferenceSpace_hpp */
