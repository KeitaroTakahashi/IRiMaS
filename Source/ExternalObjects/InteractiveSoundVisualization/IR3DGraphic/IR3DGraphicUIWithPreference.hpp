//
//  IR3DGraphicUIWithPreference.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 01/06/2019.
//

#ifndef IR3DGraphicUIWithPreference_hpp
#define IR3DGraphicUIWithPreference_hpp

#include "IR3DGraphicUI.hpp"
#include "IRISVObjectPreference.h"
#include "KeAnimationComponent.h"

class IR3DGraphicUIWithPreference : public IR3DGraphicUI,
private ChangeListener,
private KeAnimationComponent
{
public:
    IR3DGraphicUIWithPreference(IRNodeObject*nodeObject);
    ~IR3DGraphicUIWithPreference();
    // ==================================================

    IRISVObjectPreference * getPreference() const { return this->preference.get(); }
    // ==================================================
    void updateAnimationFrame() override;

    void resized() override;
    // ==================================================

    Array<ISVPresetDataStr> getPresetData() const { return this->presetData; }
    
    void addPreset(ISVPresetDataStr newPreset, bool isShowDialog = true);

private:
    
    enum TransitionDirection
    {
        FORWARD,
        REVERSE
    };
    
    TransitionDirection direction = FORWARD;
    
    
    void loadPreset(int index);
    void loadToPreset(int index);

    void clearAllPresets();
    // ==================================================
    void uiUpdated();
    void colourUpdated();
    void saveAction();
    // ==================================================
    void setSliderParams(ISVPresetDataStr preset);
    void setColourParams(ISVPresetDataStr preset);
    // ==================================================

    void changeListenerCallback(ChangeBroadcaster* source) override;

    // ==================================================

    std::shared_ptr<IRISVObjectPreference> preference;
    ISVParameterControlUI controlUI;
    
    Array<ISVPresetDataStr> presetData;
    
    bool isOperatingTransitionFlag = false;
    float transitionIncrement = 0.0;
    float transitionVal = 0.0;
    
    int animationFps = 50;
};
#endif /* IR3DGraphicUIWithPreference_hpp */
