//
//  IRSpectrogramObjectPreference.h
//  IRiMaS
//
//  Created by Keitaro on 07/07/2019.
//

#ifndef IRSpectrogramObjectPreference_h
#define IRSpectrogramObjectPreference_h

#include "IRPreferenceObject.hpp"

class IRSpectrogramObjectPreference : public IRPreferenceObject
{
public:
    IRSpectrogramObjectPreference(String title, Rectangle<int> frameRect) :
    IRPreferenceObejct(title, frameRect)
    {
        
    }

    ~IRSpectrogramObjectPreference()
    {
        
    }
    
    void resized() override
    {
        
    }
    //===============================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }
    
private:
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramObjectPreference)

};


#endif /* IRSpectrogramObjectPreference_h */
