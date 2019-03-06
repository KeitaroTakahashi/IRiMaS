//
//  IRLabelObjectPreference.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 22/10/2018.
//

#ifndef IRLabelObjectPreference_h
#define IRLabelObjectPreference_h

#include "IRLabelObject.hpp"
#include "IRPreferenceObject.hpp"
#include "FontGUI.hpp"

class IRLabelObjectPreference : public IRPreferenceObject
{
public:
    IRLabelObjectPreference(String title, Rectangle<int> frameRect) :
    IRPreferenceObject(title,frameRect)
    {
        this->fontGUI = new FontGUI(title);
        addAndMakeVisible(this->fontGUI);
        this->fontGUI->addChangeListener(this);
    }
    
    ~IRLabelObjectPreference()
    {
        
    }
    // --------------------------------------------------------------

    void resized() override
    {
        IRPreferenceObject::resized();

        this->fontGUI->setBounds(0,0,getWidth(), getHeight());
    }
    //===============================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if(source == this->fontGUI)
        {
            switch (this->fontGUI->getChangeStatus()) {
                case FontChanged:
                    break;
                case FontStyleChanged:
                    break;
                case FontSizeChanged:
                    break;
                case FontAlignChanged:
                    break;
                case FontColourChanged:
                    break;
                default:
                    break;
            }
            
        }
    }
    
    //===============================================================
    
    FontGUI* getFontGUI() { return this->fontGUI; }

private:

    FontGUI* fontGUI;
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRLabelObjectPreference)
};

#endif /* IRLabelObjectPreference_h */
