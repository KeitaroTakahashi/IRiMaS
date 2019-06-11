//
//  IR3DGraphicUI.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#ifndef IR3DGraphicUI_hpp
#define IR3DGraphicUI_hpp

#include "IRFoundation.h"
#include "IRUIFoundation.hpp"
#include "OpenGLVertexNoise.hpp"
#include "KeAnimationComponent.h"


class IR3DGraphicUI : public IRUIFoundation,
public ChangeBroadcaster
{
public:
    //==============================================================================

    IR3DGraphicUI(IRNodeObject* parent);
    ~IR3DGraphicUI();
    
    //==============================================================================
    
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    

    void vertexSetup();
    
    void startRendering();
    void stopRendering();
    
    // ==================================================
    void OffsetXChanged(float x);
    void OffsetYChanged(float y);
    void OffsetZChanged(float z);
    
    
    //==============================================================================
    bool isRendering() const { return this->vertex.isRendering(); }
    //==============================================================================

    //OpenGLVertexNoise* getOpenGLContents() const { return this->vertex.get(); }
    OpenGLVertexNoise vertex;

private:
    
    float amplitude = 0;
    float centroid = 0;
    
    float inteisity = 0;
    KMoveTo intensity_smoother;
    float fineness = 0;
    KMoveTo fineness_smoother;
    
    //std::shared_ptr<OpenGLVertexNoise> vertex;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IR3DGraphicUI)
    
};
#endif /* IR3DGraphicUI_hpp */
