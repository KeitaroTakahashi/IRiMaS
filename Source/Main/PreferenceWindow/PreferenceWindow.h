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
    PreferenceWindow(String name) : DocumentWindow(name,
                                                   Desktop::getInstance().getDefaultLookAndFeel()
                                                   .findColour (ResizableWindow::backgroundColourId),
                                                   DocumentWindow::allButtons)
    {
        
        setUsingNativeTitleBar (true);

        this->preferenceSpace = new IRPreferenceSpace("IRiMaS : Object Preference",
                                                      Rectangle<int>(0,0,400,720));
       
        this->preferenceSpace->addChangeListener(this);
        setContentOwned(this->preferenceSpace, true);
        
        setResizable(true,true);
        
        setName(this->preferenceSpace->getTitle());
        
        Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
        //int x = r.getX();
        int y = r.getY();
        int w = r.getWidth();
       // int h = r.getHeight();
        
        
        setBounds(w - getWidth(),y,getWidth(),getHeight());

        setVisible(true);

    }
    ~PreferenceWindow()
    {
        delete preferenceSpace;
        
    }
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if(source == this->preferenceSpace)
        {
        }
    }
    
    void closeButtonPressed() override
    {
        // This is called when the user tries to close this window. Here, we'll just
        // ask the app to quit when this happens, but you can change this to do
        // whatever you need.
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    IRPreferenceSpace* getPreferenceSpace(){ return this->preferenceSpace; }
    
private:
    IRPreferenceSpace *preferenceSpace;
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreferenceWindow)
    
};


#endif /* PreferenceWindow_h */
