//
//  PreferenceWindow.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 17/10/2018.
//

#ifndef PreferenceWindow_h
#define PreferenceWindow_h


#include "JuceHeader.h"
#include "IRPreferenceSpace.hpp"
#include "Preferences.h"



class PreferenceWindow : public DocumentWindow, public ChangeListener
{
    
public:
    
    PreferenceWindow(String name);
    ~PreferenceWindow();
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void closeButtonPressed() override;
    
    IRPreferenceSpace* getPreferenceSpace();
    
private:
    
    IRPreferenceSpace *preferenceSpace;
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PreferenceWindow)
};


#endif /* PreferenceWindow_h */



