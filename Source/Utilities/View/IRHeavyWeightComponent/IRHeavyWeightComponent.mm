//
//  IROpenGLRenderer.m
//  IRiMaS
//
//  Created by Keitaro on 17/11/2019.
//

#import <AppKit/AppKit.h>
#include "IRHeavyWeightComponent.cpp"

void IRHeavyWeightComponent::setComponentAlpha(GLint aValue)
{
    NSOpenGLContext* context = (NSOpenGLContext*)this->ctx.getRawContext();
    
    if(context != nullptr)
        [context setValues:&aValue forParameter:NSOpenGLCPSurfaceOpacity];
    else
        std::cout << "Error : IROpenGLManager::setOpenGLContextAlpha() : could not get NSOpenGLContext : nullptr\n";
    
}

void IRHeavyWeightComponent::bringToFront(juce::Component* owner)
{
    
    NSOpenGLContext* context = (NSOpenGLContext*)this->ctx.getRawContext();
    if(context == nullptr)
    {
        std::cout << "Error : IROpenGLManager::bringOpenGLContextFront() : could not get NSOpenGLContext : nullptr\n";
        return;
    }
        
    
    auto* peer = owner->getPeer();
    if(peer != nullptr)
    {
        
        auto peerView = (NSView*) peer->getNativeHandle();
        [context.view retain];
        [context.view removeFromSuperview];
        [peerView addSubview: context.view];
        [context.view release];
        
    }
    
    [context.view setHidden: ! owner->isShowing()];
    
    // re-setup component alpha
    setComponentAlpha(this->alpha);
}



