
#ifndef PreferenceWindow_hpp
#define PreferenceWindow_hpp


#include "JuceHeader.h"
#include "IRPreferenceSpace.hpp"
#include "PreferenceWindow.hpp"
#include "ColourLib.h"
#include "singletonClass.hpp"




class PreferenceWindow : public DocumentWindow,
                         public ChangeListener
{
    
public:
    
    PreferenceWindow(String name);
    ~PreferenceWindow();
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    void closeButtonPressed() override;
    
    IRPreferenceSpace* getPreferenceSpace();
    
    
private:
    
    IRPreferenceSpace *preferenceSpace;
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PreferenceWindow)
};





#endif /* PreferenceWindow_hpp */




