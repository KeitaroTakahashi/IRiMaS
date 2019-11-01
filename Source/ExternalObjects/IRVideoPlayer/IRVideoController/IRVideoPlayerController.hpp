//
//  IRVideoController.hpp
//  IRiMaS
//
//  Created by Keitaro on 24/10/2019.
//

#ifndef IRVideoController_hpp
#define IRVideoController_hpp

#include "IRObjectController.hpp"
#include "FontController.h"
#include "videoController.h"

class IRVideoPlayerController : public IRObjectController,
public ChangeListener,
public ChangeBroadcaster
{
public:
    
    enum VideoPlayerControllerStatus
    {
        OpenMovieFile,
        WidthChange,
        HeightChange,
        W_H_Sync,
        NONE
    };
    
    IRVideoPlayerController(IRStr* str);
    ~IRVideoPlayerController();

    void resized() override;
    void paint(Graphics& g) override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    VideoController::VideoControllerStatus getStatus() const { return this->UI.getStatus(); }
    
    void setChangeListener(ChangeListener* listener) { this->UI.addChangeListener(listener); }
    
private:

    Label LabelTitle;
    VideoController UI;

    VideoPlayerControllerStatus status = NONE;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRVideoPlayerController)

};
#endif /* IRVideoController_hpp */
