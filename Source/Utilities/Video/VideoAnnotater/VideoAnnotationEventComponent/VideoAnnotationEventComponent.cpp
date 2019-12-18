//
//  VideoAnnotationEventComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 11/12/2019.
//

#include "VideoAnnotationEventComponent.hpp"

VideoAnnotationEventComponent::VideoAnnotationEventComponent(IRStr* str) :
IRStrComponent(str)
{
    
}

VideoAnnotationEventComponent::~VideoAnnotationEventComponent()
{
    
}
// ==================================================

void VideoAnnotationEventComponent::paint(Graphics& g )
{
    g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
}

void VideoAnnotationEventComponent::resized()
{
    
}
// ==================================================

void VideoAnnotationEventComponent::mouseDown(const MouseEvent& e)
{
    setSelected(true);
}



// ==================================================
// ==================================================
// ==================================================
