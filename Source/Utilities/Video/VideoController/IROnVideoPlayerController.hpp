//
//  IROnVideoPlayerController.hpp
//  IRiMaS
//
//  Created by Keitaro on 30/06/2020.
//

#ifndef IROnVideoPlayerController_hpp
#define IROnVideoPlayerController_hpp

#include "JuceHeader.h"
#include "IRStrComponent.hpp"
#include "IRHeavyWeightComponent.h"
#include "IRImageButton.hpp"

class IROnVideoPlayerController : public Component,
public IRStrComponent,
public IRHeavyWeightComponent
{
public:
    IROnVideoPlayerController(IRStr* str);
    ~IROnVideoPlayerController();
    // ==================================================

    void resized() override;
    
    void paint(Graphics& g) override;
    
    // ==================================================
    void setCurrentPlayPosition(float sec);
    float getCurrentPlayPosition() const { return this->currentPlayPosition; }
    // ==================================================

private:
    
    float currentPlayPosition = 0;
    
    // ==================================================
    
    IRImageButton playPauseButton;
    // ==================================================
    // ==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IROnVideoPlayerController)

};
#endif /* IROnVideoPlayerController_hpp */
