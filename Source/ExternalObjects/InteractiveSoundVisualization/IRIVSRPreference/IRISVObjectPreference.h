//
//  IRISVPreference.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 01/06/2019.
//

#ifndef IRISVPreference_h
#define IRISVPreference_h

#include "IRPreferenceObject.hpp"
#include "ISVPreferenceUI.hpp"


class IRISVObjectPreference : public IRPreferenceObject
{
public:
    IRISVObjectPreference(String title, Rectangle<int> frameRect) :
    IRPreferenceObject(title, frameRect)
    {
        this->UI = std::make_shared<ISVPreferenceUI>(title);
        addAndMakeVisible(this->UI.get());
    }
    
    ~IRISVObjectPreference()
    {

    }
    //===============================================================
    
    void resized() override
    {
        this->UI->setBounds(0,0,getWidth(), getHeight());

    }
    
    
    //===============================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }
    
    ISVPreferenceUI* getUI() { return this->UI.get(); }
    
private:
    
    std::shared_ptr<ISVPreferenceUI> UI;
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRISVObjectPreference)

    
};
#endif /* IRISVPreference_h */
