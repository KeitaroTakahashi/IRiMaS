//
//  IRSpectrogramObjectPreference.h
//  IRiMaS
//
//  Created by Keitaro on 07/07/2019.
//

#ifndef IRSpectrogramObjectPreference_h
#define IRSpectrogramObjectPreference_h

#include "IRPreferenceObject.hpp"
#include "IRSpectrogramPreferenceUI.hpp"

class IRSpectrogramObjectPreference : public IRPreferenceObject
{
public:
    IRSpectrogramObjectPreference(String title, Rectangle<int> frameRect) :
    IRPreferenceObject(title, frameRect)
    {
        this->UI = std::make_shared<IRSpectrogramPreferenceUI>(title);
        addAndMakeVisible(this->UI.get());
    }

    ~IRSpectrogramObjectPreference()
    {
        
    }
    
    void resized() override
    {
        IRPreferenceObject::resized();
        this->UI->setBounds(0,0,getWidth(), getHeight());

    }
    //===============================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }
    
    IRSpectrogramPreferenceUI* getUI() { return this->UI.get(); }

private:
    
    std::shared_ptr<IRSpectrogramPreferenceUI> UI;
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramObjectPreference)

};


#endif /* IRSpectrogramObjectPreference_h */
