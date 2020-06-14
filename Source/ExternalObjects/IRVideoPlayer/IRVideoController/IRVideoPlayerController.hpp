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
#include "IRVideoAnnotaterObject2.hpp"
#include "ObjectArranger.hpp"

class IRVideoPlayerController : public IRObjectController,
public ChangeBroadcaster
{
public:
    
    enum VideoPlayerControllerStatus
    {
        OpenMovieFile,
        OpenVideoAnnotater,
        CloseVideoAnnotater,
        ApplyAnnotation,
        WidthChange,
        HeightChange,
        W_H_Sync,
        NONE
    };
    // ==================================================

    IRVideoPlayerController(IRStr* str, IRVideoAnnotaterObject2* videoPlayerObject);
    ~IRVideoPlayerController();
    // ==================================================

    void ControllerResized() override;
    void paint(Graphics& g) override;
    // ==================================================

    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    VideoController::VideoControllerStatus getStatus() const { return this->UI.getStatus(); }
    // ==================================================

    void setChangeListener(ChangeListener* listener) { this->UI.addChangeListener(listener); }
    
    // ==================================================
    void updateAnnotater();
    void updateParentVideoPlayerObject();
    // ==================================================
    IRVideoAnnotater* getVideoAnnotaterComponent() { return this->UI.getVideoAnnotaterComponent(); }

    // ==================================================
    ArrangeController* getArrangeController();
    
private:
    // ==================================================

    Label LabelTitle;
    VideoController UI;
    std::unique_ptr<ArrangeController> arrangeController;
    // ==================================================
    
    void mainControllerSelected() override;
    void arrangeControllerSelected() override;


    VideoPlayerControllerStatus status = NONE;
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRVideoPlayerController)

};
#endif /* IRVideoController_hpp */
