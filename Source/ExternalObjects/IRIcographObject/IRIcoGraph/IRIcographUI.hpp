//
//  IRIcographUI.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#ifndef IRIcographUI_hpp
#define IRIcographUI_hpp

#include "IRFoundation.h"
#include "IRUIFoundation.hpp"
#include "OpenGLVertexNoise.hpp"
#include "KeAnimationComponent.h"


class IRIcographUI : public IRUIFoundation,
private ChangeListener,
public ChangeBroadcaster,
private KeAnimationComponent
{
public:
    //==============================================================================

    IRIcographUI(IRNodeObject* parent);
    ~IRIcographUI();
    
    //==============================================================================
    
    void paint (Graphics&) override;
    void resized() override;
    void updateAnimationFrame() override;
    
    //==============================================================================
    

    void vertexSetup();

    // ==================================================
    void OffsetXChanged(float x);
    void OffsetYChanged(float y);
    void OffsetZChanged(float z);
    
    
    //==============================================================================

private:
    
    float amplitude = 0;
    float centroid = 0;
    
    float inteisity = 0;
    KMoveTo intensity_smoother;
    float fineness = 0;
    KMoveTo fineness_smoother;
    
    
    OpenGLVertexNoise* vertex;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRIcographUI)
    
};
#endif /* IRIcographUI_hpp */
