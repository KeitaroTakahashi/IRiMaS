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
    AnnotationShapeEventComponent(IRStr *str) : VideoAnnotationEventComponent(str)
    {
        
    }
    
    ~AnnotationShapeEventComponent()
    {
        
    }
    // ==================================================
    void paint(Graphics& g ) override
    {
        g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    }
    
    void resized() override
    {
        
    }
    // ==================================================
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
