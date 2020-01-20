//
//  AnnotationShapeEventComponent.h
//  IRiMaS
//
//  Created by Keitaro on 12/12/2019.
//

#ifndef AnnotationShapeEventComponent_h
#define AnnotationShapeEventComponent_h

#include "VideoAnnotationEventComponent.hpp"

class AnnotationShapeEventComponent : public VideoAnnotationEventComponent
{
public:
    AnnotationShapeEventComponent(IRStr *str, int videoLengthInSecond = 0) : VideoAnnotationEventComponent(str, videoLengthInSecond)
    {
        setType(VideoAnnotationEventComponent::SHAPE);

    }
    
    ~AnnotationShapeEventComponent()
    {
        
    }
    // ==================================================
    void paint(Graphics& g ) override
    {
        VideoAnnotationEventComponent::paint(g);
    }
    
    void resized() override
    {
        VideoAnnotationEventComponent::resized();
    }
    // ==================================================
    srtWriter::SRT_STRUCT getSRT() override
    {
        return srtWriter::SRT_STRUCT();
    }
    // ==================================================
    // ==================================================
    // ==================================================

private:
    // ==================================================
    // ==================================================
    // ==================================================
    // ==================================================

    
    
};
#endif /* AnnotationShapeEventComponent_h */
