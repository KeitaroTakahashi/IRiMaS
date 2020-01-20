//
//  IRVideoAnnotationComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 19/01/2020.
//

#include "IRVideoAnnotationComponent.hpp"

IRVideoAnnotationComponent::IRVideoAnnotationComponent(IRStr* str, VideoAnnotationEventComponent* event) :
IRStrComponent(str),
IRHeavyWeightComponent(this),
event(event)
{
    this->beginTime = event->getBeginTimeCode();
    this->endTime = event->getEndTimeCode();
}

IRVideoAnnotationComponent::~IRVideoAnnotationComponent()
{
    
}


void IRVideoAnnotationComponent::setTimeCode(float beginTime, float endTime)
{
    this->beginTime = beginTime;
    this->endTime = endTime;

}
