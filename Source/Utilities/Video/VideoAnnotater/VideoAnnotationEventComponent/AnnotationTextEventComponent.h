//
//  AnnotationTextEventComponent.h
//  IRiMaS
//
//  Created by Keitaro on 12/12/2019.
//

#ifndef AnnotationTextEventComponent_h
#define AnnotationTextEventComponent_h

#include "VideoAnnotationEventComponent.hpp"
#include "TimeCodeInput.h"

class AnnotationTextEventComponent : public VideoAnnotationEventComponent
{
public:
    AnnotationTextEventComponent(IRStr *str) : VideoAnnotationEventComponent(str),
    timeCodeUI(str)
    {
        addAndMakeVisible(&this->timeCodeUI);
        addAndMakeVisible(&this->textContents);
        this->textContents.setEditable(true);
    }
    
    ~AnnotationTextEventComponent()
    {
        
    }
    // ==================================================
    void paint(Graphics& g ) override
    {
        g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    }
    
    void resized() override
    {
        this->timeCodeUI.setBounds(0, 0, 200, getHeight());
        this->textContents.setBounds(200, 5, getWidth() - 200, 30);
    }
    // ==================================================
    // ==================================================
    // ==================================================
    // ==================================================

private:
    // ==================================================
    
    TimeCodeInput timeCodeUI;
    // ==================================================
    Label textContents;
    // ==================================================
    // ==================================================

    
    
};
#endif /* AnnotationTextEventComponent_h */
