//
//  IROnVideoController.h
//  IRiMaS
//
//  Created by Keitaro on 30/06/2020.
//

#ifndef IROnVideoController_h
#define IROnVideoController_h

#include "JuceHeader.h"

class IROnVideoController : public Component
{
public:
    IROnVideoController()
    {
        
    }
    
    ~IROnVideoController()
    {
        
    }
    // --------------------------------------------------

    void resized() override
    {
        
    }
    
    void paint(Graphics &g) override
    {
        
    }
    
    // --------------------------------------------------
    void setCurrentPlayPosition(float sec)
    {
        this->currentPlayPosition = sec;
    }
    
    float getCurrentPlayPosition() const { return this->currentPlayPosition; }
    
    
    // --------------------------------------------------

private:
    
    
    float currentPlayPosition = 0;
    
};
#endif /* IROnVideoController_h */
