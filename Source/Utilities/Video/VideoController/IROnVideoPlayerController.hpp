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
#include "IRPlayPauseButton.h"

class IROnVideoPlayerController : public Component,
public IRStrComponent,
public IRHeavyWeightComponent
{
public:
    
    enum IROnVideoPlayerControllerStatus
    {
        NONE,
        PLAY,
        PAUSE
    };
    
    IROnVideoPlayerController(IRStr* str);
    ~IROnVideoPlayerController();
    // ==================================================

    void resized() override;
    
    void paint(Graphics& g) override;
    
    // ==================================================
        
    void mouseDown(const MouseEvent& e) override
    {
        std::cout << "IROnVideoPlayerController : mouseDown\n";
    }
    // ==================================================

    void setCurrentPlayPosition(float sec);
    float getCurrentPlayPosition() const { return this->currentPlayPosition; }
    // ==================================================
    
    void playAction();
    void pauseAction();
    
    IROnVideoPlayerControllerStatus getStatus() const { return this->status; }
    void setStatus(IROnVideoPlayerControllerStatus status);

private:
    
    IROnVideoPlayerControllerStatus status = NONE;
    // ==================================================

    float currentPlayPosition = 0;
    
    // ==================================================
    
    IRPlayPauseButton playPauseButton;
    void playPauseButtonClickedAction();
    // ==================================================
    // ==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IROnVideoPlayerController)

};
#endif /* IROnVideoPlayerController_hpp */
