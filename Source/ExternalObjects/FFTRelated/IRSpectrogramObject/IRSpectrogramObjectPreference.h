//
//  IRSpectrogramObjectPreference.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 23/10/2018.
//

#ifndef IRSpectrogramObjectPreference_h
#define IRSpectrogramObjectPreference_h

#include "IRPreferenceObject.hpp"
#include "FFTPreferenceUI.h"

class IRSpectrogramObjectPreference : public IRPreferenceObject
{
public:
    IRSpectrogramObjectPreference(String title, Rectangle<int> frameRect) :
    IRPreferenceObject(title,frameRect)
    {
        this->FFTUI = new FFTPreferenceUI(title);
        addAndMakeVisible(this->FFTUI);
    }
    
    ~IRSpectrogramObjectPreference()
    {
        delete this->FFTUI;
    }
    //===============================================================
    void resized() override
    {
        IRPreferenceObject::resized();
        this->FFTUI->setBounds(0,0,getWidth(), getHeight());

    }
    
    //===============================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }
private:
    
    FFTPreferenceUI *FFTUI;
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramObjectPreference)
};

#endif /* IRSpectrogramObjectPreference_h */
