//
//  IRIVSRController.hpp
//  IRiMaS
//
//  Created by Keitaro on 16/10/2019.
//

#ifndef IRIVSRController_hpp
#define IRIVSRController_hpp

#include "IRObjectController.hpp"
#include "IRColourSelector.h"

class IRIVSRController : public IRObjectController,
public ChangeBroadcaster,
private ComboBox::Listener

{
public:
    
    enum IRIVSRControllerStatus
    {
        PresetChanged,
        IntensitySliderChanged,
        FinenessSliderChanged,
        LightingColourChanged,
        MaterialColourChanged,
        PresetSelected,
        toPresetSelected,
        TransitionToInitialSphereChanged,
        TransitionBetweenPresetsChanged,
        TransitionOperatePressed,
        TransitionReversePressed,
        ClearAllPresets
    };
    
    
    IRIVSRController(IRStr* str);
    ~IRIVSRController();

    // ==================================================
    
    void ControllerResized() override;
    void paint(Graphics& g) override;
    
    // ==================================================
    
  
    IRIVSRControllerStatus getStatus() const { return this->status; }
    
private:
    
    
    // ==================================================
    void changeListenerCallback(ChangeBroadcaster* source) override {}

    
    // ==================================================

    IRIVSRControllerStatus status;
    
    
    Label LabelTitle;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRIVSRController)

};
#endif /* IRIVSRController_hpp */
