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

class IR3DGraphicUIWithPreference : public IR3DGraphicUI,
private ChangeListener
{
public:
    IR3DGraphicUIWithPreference(IRNodeObject*nodeObject);
    ~IR3DGraphicUIWithPreference();
    // ==================================================

    IRISVObjectPreference * getPreference() const { return this->preference.get(); }
    // ==================================================

    void resized() override;
    // ==================================================

    Array<ISVPresetDataStr> getPresetData() const { return this->presetData; }
    
    void addPreset(ISVPresetDataStr newPreset);

private:
    
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
    
};
#endif /* IR3DGraphicUIWithPreference_hpp */
