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
#include "TimeCode.h"
#include "AnnotationChart.h"

class IROnVideoPlayerController : public Component,
public IRStrComponent,
public IRHeavyWeightComponent,
public ChangeBroadcaster,
private Slider::Listener
{
public:
    
    enum IROnVideoPlayerControllerStatus
    {
        NONE,
        PLAY,
        playPositionChanged,
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
    void setVideoLengthInSec(float length);
    
    void setCurrentPlayPosition(float sec);
    float getCurrentPlayPosition() const { return this->currentPlayPosition; }
    void setSliderValue(float val, bool emitChangeBroadcaster);
    // ==================================================
    
    void playAction();
    void pauseAction();
    // ==================================================

    IROnVideoPlayerControllerStatus getStatus() const { return this->status; }
    void setStatus(IROnVideoPlayerControllerStatus status);
    // ==================================================
    
    void addMouseListenerToChildren(MouseListener* l);
    
    // ==================================================
    // Annotation Chart
    void setAnnotationData(std::vector<AnnotationChart::annotationData> data);
    
    // ==================================================


private:
    // ==================================================
    
    IROnVideoPlayerControllerStatus status = NONE;
    // ==================================================
    float videoLengthInSec = 0;
    float currentPlayPosition = 0;
    bool emitChangeBroadcaster4CurrentPlayPosition = true;
    // ==================================================
    
    IRPlayPauseButton playPauseButton;
    void playPauseButtonClickedAction();
    
    AnnotationChart chart;
    void createChart();
    
    Slider timeCodeSlider;
    void createTimeCodeSlider();
    void sliderValueChanged (Slider *slider) override;
    
    TimeCode timeCode;

    // ==================================================
    // ==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IROnVideoPlayerController)

};
#endif /* IROnVideoPlayerController_hpp */
