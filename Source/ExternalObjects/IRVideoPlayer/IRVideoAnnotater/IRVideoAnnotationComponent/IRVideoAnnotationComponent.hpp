//
//  IRVideoAnnotationComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 19/01/2020.
//

#ifndef IRVideoAnnotationComponent_hpp
#define IRVideoAnnotationComponent_hpp

#include "JuceHeader.h"
#include "IRStrComponent.hpp"
#include "IRHeavyWeightComponent.h"
#include "VideoAnnotationEventComponent.hpp"

class IRVideoAnnotationComponent : public Component,
                                   public IRStrComponent,
                                   public IRHeavyWeightComponent
{
public:
    IRVideoAnnotationComponent(IRStr* str, VideoAnnotationEventComponent* event);
    ~IRVideoAnnotationComponent();
    // --------------------------------------------------
    
    void setTimeCode(float beginTime, float endTime);
    
    // --------------------------------------------------
    // second
    float getBeginTimeInSec() const { return this->beginTime; }
    float getEndTimeInSec()   const { return this->endTime; }

    // --------------------------------------------------
    VideoAnnotationEventComponent* getEvent() { return this->event; }

private:
    // --------------------------------------------------
    
    VideoAnnotationEventComponent* event = nullptr;
    float beginTime;
    float endTime;
    // --------------------------------------------------
    // --------------------------------------------------
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoAnnotationComponent)

};

#endif /* IRVideoAnnotationComponent_hpp */
