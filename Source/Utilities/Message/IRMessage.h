//
//  IRMessage.h
//  IRiMaS
//
//  Created by Keitaro on 07/10/2019.
//

#ifndef IRMessage_h
#define IRMessage_h

#include "JuceHeader.h"
#include "KeAnimationComponent.h"
#include "IRStrComponent.hpp"
#include "IROpenGLManager.hpp"

class IRMessage : public Component,
public IRStrComponent,
public OpenGLRenderer,
public KeAnimationComponent
{
public:
    IRMessage(int stayDuration, int disappearCount, Component* parent, IRStr* str) :
    IRStrComponent(str), stayDuration(stayDuration), parent(parent), manager(&context)
    {

        setOpaque(true);
        setFps(this->fps);
        
        this->disappearSpeed = 1.0 / (float)disappearCount;
        
        addAndMakeVisible(&this->textLabel);
        this->textLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
        this->textLabel.setJustificationType(Justification::centred);
        
        
        setSize(300, 150);
        
        setWantsKeyboardFocus(false);
        
        
    }
    
    ~IRMessage()
    {
        deinitOpenGLContext();
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    }
    
    void resized() override
    {
        
        this->textLabel.setSize(getWidth(), getHeight());

        this->textLabel.setCentrePosition(getWidth()/2, getHeight()/2);
        
        
    }
    // ==================================================
    void initOpenGLContext()
    {
        //if this object contains heavy weight component, then connect to OpenGLContext
       this->context.setRenderer(this);
       this->context.setContinuousRepainting(false);
       
       this->context.attachTo(*this);
       IROpenGLManager manager(&this->context, "IRMessage");
       manager.setOpenGLContextAlpha(0.0);
        
    }
    
    void deinitOpenGLContext()
    {
        this->context.detach();

    }
    
    void newOpenGLContextCreated()override
    {
    }
       
        virtual void renderOpenGL()override
    {
        OpenGLHelpers::clear(Colours::transparentBlack);
    }
    
    virtual void openGLContextClosing() override
    {
        
    }
    // ==================================================

    void setText(String text)
    {
        this->textLabel.setText(text, dontSendNotification);
    }
    
    void run()
    {
        if(isVisible())
        {
            removeFromDesktop();
            this->parent->removeChildComponent(this);
            stopAnimation();
            deinitOpenGLContext();

        }
        
        this->stayPhase = true;
        this->alpha = 1.0;
        setAlpha(this->alpha);

        this->stayCount = 0;
        startAnimation();
        addToDesktop(ComponentPeer::windowIsTemporary);
        toFront(true);
        this->parent->addAndMakeVisible(this);
        initOpenGLContext();
    }
    
    
private:
    
    void updateAnimationFrame() override
    {
        if(this->stayPhase)
        {
            if(this->stayCount >= this->stayDuration)
            {
                this->disappearPhase = true;
                this->stayPhase = false;
            }
            this->stayCount ++;
            
        }else if(this->disappearPhase)
        {
            this->alpha -= this->disappearSpeed;
            setAlpha(this->alpha);
            if(this->alpha <= 0.00)
            {
                this->disappearPhase = false;
                stopAnimation();
                removeFromDesktop();
                this->parent->removeChildComponent(this);
            }
        }
    }
    
    
    int fps = 60;
    
    int stayDuration = 0;
    
    float alpha = 1.0;
    int stayCount = 0;
    float disappearSpeed = 0.01;
    
    bool stayPhase = true;
    bool disappearPhase = false;
    
    Label textLabel;
    
    Component* parent = nullptr;
    
    // ====
    OpenGLContext context;
    IROpenGLManager manager;
    
};
#endif /* IRMessage_h */
