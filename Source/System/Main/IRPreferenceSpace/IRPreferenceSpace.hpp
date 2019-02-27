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
#include "Preferences.h"
#include "IRPreferenceObject.hpp"





class IRPreferenceSpace : public Component,
public ChangeBroadcaster,
public ChangeListener
{
    
public:
    
    IRPreferenceSpace(String title, Rectangle<int> frameRect);
    ~IRPreferenceSpace();
    
    void resized() override;
    void paint(Graphics& g) override;
    
    String getTitle();
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    void setPreferenceObj(IRPreferenceObject* preferenceObj);
    IRPreferenceObject* getPreferenceObj();

private:
    
    String title;
    bool isPrefereneObjAlreadySet = false;
    
    IRPreferenceObject* preferenceObj;
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRPreferenceSpace)
};

#endif /* IRPreferenceSpace_hpp */



