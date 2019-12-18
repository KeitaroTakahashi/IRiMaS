//
//  IRProgressBar.h
//  IRiMaS
//
//  Created by Keitaro on 17/12/2019.
//

#ifndef IRProgressBar_h
#define IRProgressBar_h

#include "IRHeavyWeightComponent.h"
#include "LoadingBar.hpp"

class IRProgressBar : public Component,
public IRHeavyWeightComponent,
public IRStrComponent

{
public:
    IRProgressBar(IRStr* str) : IRStrComponent(str)
    {
        this->title.setText("Loading...", dontSendNotification);
        addAndMakeVisible(&this->title);
        this->subTitle.setText("test...", dontSendNotification);
        addAndMakeVisible(&this->subTitle);

        
        addAndMakeVisible(&this->bar);
        
        
        setSize(200, 90);
    }
    
    ~IRProgressBar()
    {
        
    }
    
    void resized() override
    {
        this->title.setBounds       (5, 5, 190, 20);
        this->subTitle.setBounds    (5, 30, 190, 20);
        this->bar.setBounds         (5, 55, 190, 25);
    }
private:
    
    Label title;
    Label subTitle;
    LoadingBar bar;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRProgressBar)

};
#endif /* IRProgressBar_h */
