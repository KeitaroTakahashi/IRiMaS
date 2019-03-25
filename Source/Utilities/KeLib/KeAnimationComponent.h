//
//  KeAnimationComponent.h
//  NewProject
//
//  Created by Keitaro on 03/12/2018.
//

#ifndef KeAnimationComponent_h
#define KeAnimationComponent_h

class KeAnimationComponent : public Timer
{
    
public:
    
    KeAnimationComponent()
    {
        
    }
    
    
    void setFps(int fps)
    {
        jassert (fps > 0 && fps < 1000);
        this->fps = fps;
    }
    
    void startAnimation()
    {
        startTimerHz(this->fps);
    }
    
    void stopAnimation()
    {
        stopTimer();
    }
    virtual void updateAnimationFrame() = 0;
    
    
private:
    
    void timerCallback() override
    {
        updateAnimationFrame();
    }
    
    int fps;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeAnimationComponent)
    
};

#endif /* KeAnimationComponent_h */
