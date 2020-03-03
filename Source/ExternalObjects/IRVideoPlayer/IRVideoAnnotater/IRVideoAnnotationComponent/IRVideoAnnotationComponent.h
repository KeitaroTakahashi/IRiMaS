
#ifndef IRVideoAnnotationComponent_h
#define IRVideoAnnotationComponent_h

//
//  IRVideoAnnotationComponent.h
//  IRiMaS
//
//  Created by Keitaro on 19/01/2020.
//

#ifndef IRVideoAnnotationComponent_hpp
#define IRVideoAnnotationComponent_hpp

#include "JuceHeader.h"
#include "IRStrComponent.hpp"
#include "IRHeavyWeightComponent.h"
#include "VideoAnnotationEventComponent.h"

class IRVideoAnnotationComponent : public Component,
                                   public IRStrComponent,
                                   public IRHeavyWeightComponent
{
public:
    IRVideoAnnotationComponent(IRStr* str, VideoAnnotationEventComponent* event) :
    IRStrComponent(str),
    IRHeavyWeightComponent(this),
    event(event)
    {
        this->beginTime = event->getBeginTimeCode();
        this->endTime = event->getEndTimeCode();
    }
    ~IRVideoAnnotationComponent() {}
    // --------------------------------------------------
    
    void setTimeCode(float beginTime, float endTime)
    {
        this->beginTime = beginTime;
        this->endTime = endTime;
    }
    
    // --------------------------------------------------
    // second
    float getBeginTimeInSec() const { return this->beginTime; }
    float getEndTimeInSec()   const { return this->endTime; }

    // --------------------------------------------------
    VideoAnnotationEventComponent* getEvent() { return this->event; }
    
    void setPositionRatio(Rectangle<float> ratio) { this->positionRatio = ratio; }
    Rectangle<float> getPositionRatio() const { return this->positionRatio; }
    // --------------------------------------------------

    void setParentEvent(VideoAnnotationEventComponent* parentEvent) { this->parentEvent = parentEvent; }
    VideoAnnotationEventComponent* getParentEvent() const { return this->parentEvent; }
    
    
    // --------------------------------------------------
    //fired when this component's size is changed
    // so that the parent class using this class can reorganize the positioning.
    std::function<void()> sizeChanged;
    // store previous bounds to avoid emitting sizeChanged() when size is not changed/
    void setPreviousBounds(int w, int h) { this->previousSize = Point<int>(w, h); }
    
    bool isSizeChanged() const
    {
        std::cout << "isSizeChanged ; " << this->getWidth() << ", " << this->getHeight() << " : " << this->previousSize.getX() << ", " << this->previousSize.getY() << std::endl;
        
        if(this->getWidth() == this->previousSize.getX() &&
           this->getHeight() == this->previousSize.getY())
        {
            return false;
        }else return true;
    }
private:
    // --------------------------------------------------
    
    VideoAnnotationEventComponent* event = nullptr;
    float beginTime;
    float endTime;
    // --------------------------------------------------

    Point<int> previousSize;

    // represent the position of this component as ratio to the video size.
    Rectangle<float> positionRatio;
    // --------------------------------------------------
    
    // this is used as reference ONLY, do not try to get values or modify it!!
    VideoAnnotationEventComponent* parentEvent = nullptr;
    
    // --------------------------------------------------
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoAnnotationComponent)

};

#endif /* IRVideoAnnotationComponent_hpp */

#endif /* IRVideoAnnotationComponent_h */
