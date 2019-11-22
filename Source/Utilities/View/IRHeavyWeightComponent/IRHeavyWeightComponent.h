//
//  IROpenGLRenderer.h
//  IRiMaS
//
//  Created by Keitaro on 17/11/2019.
//

#ifndef IROpenGLRenderer_h
#define IROpenGLRenderer_h

#include "JuceHeader.h"

/*
    This class converts a light-weight juce::Component to heavy-weight Component by the power of OpenGL.
    It uses Objective-C++ to access NSOpenGLView->NSView and thus, it only supports macOS.
    Use this class by inheriting in your juce::Component based objects.
    Keitaro 2019 Nov
 */

class IRHeavyWeightComponent : OpenGLRenderer,
ComponentMovementWatcher
{
public:
    IRHeavyWeightComponent(juce::Component* component, String name = "") :
    ComponentMovementWatcher(component),
    name(name),
    component ( component )
    {
        this->ctx.attachTo(*component);
        this->ctx.setRenderer(this);
        this->ctx.setContinuousRepainting(false);
    }
    
    ~IRHeavyWeightComponent()
    {
        this->ctx.detach();
    }
   //==================================================
    virtual void componentCreated() {};
    //==================================================


    void setHeavyWeightBackgroundAlpha(float alpha) { this->alpha = alpha; }
    //==================================================
    //OpenGL
    OpenGLContext ctx;
    
    void bringThisToFront(String flag = "") {
        std::cout << flag << std::endl;
        bringToFront(this->component);
        //callback
        bringToFrontCompleted();
    }
    
    void heavyWeightRepaint(String flag = "")
    {
        
        std::cout << flag << std::endl;
        this->ctx.triggerRepaint();
    }
    
    //==================================================
    
    virtual void bringToFrontCompleted() {}
    
    juce::Component* component;
    
private:
    
    // for background transparency
    float alpha = 0.0;
    String name;
    //==================================================

    // using Objective-C++ macOS only!!
    // set up alpha value of NSOpenGLView to make transparent background
    void setComponentAlpha(GLint aValue = 0);
    // bring the juce::Component inheriting this class to the foreground of any other juce::Component or heavy-weight Components
    void bringToFront(juce::Component* owner);
   
    //==================================================
    void newOpenGLContextCreated()override
    {
        setComponentAlpha(this->alpha);
        componentCreated();
    }
    
    virtual void renderOpenGL()override
    {
        
        std::cout << " ==================== rendering..\n";
        // transparent background
        OpenGLHelpers::clear(Colours::transparentBlack);
    }
    
    virtual void openGLContextClosing() override {}
    
    // ComponentMovementWatcher
    virtual void componentMovedOrResized(bool wasMoved, bool wasResized) override
    {
        //setComponentAlpha(this->alpha);
    }
    
    // here we need to set the Alpha of NSOpenGLView
    virtual void componentVisibilityChanged() override
    {
        std::cout << "componentVisibilityChanged : " << this->name << std::endl;
        //setComponentAlpha(this->alpha);
    }
    
    virtual void componentPeerChanged() override {
        //setComponentAlpha(this->alpha);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRHeavyWeightComponent)

};

#endif /* IROpenGLRenderer_h */
