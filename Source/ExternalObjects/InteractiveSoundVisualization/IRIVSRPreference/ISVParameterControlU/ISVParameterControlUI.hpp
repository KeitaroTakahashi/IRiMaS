//
//  ISVParameterControlUI.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/06/2019.
//

#ifndef ISVParameterControlUI_hpp
#define ISVParameterControlUI_hpp

#include "ISVParameterSliders.hpp"

class ISVParameterControlUI : public Component,
public ChangeBroadcaster,
private ChangeListener
{
public:
    ISVParameterControlUI();
    ~ISVParameterControlUI();
    // ==================================================
    ISVPresetDataStr getPreset1() const;
    ISVPresetDataStr getPreset2() const;
    ISVPresetDataStr getCurrentPreset() const;

    String getPresetSaveName() const;

    void setCurrentPreset(ISVPresetDataStr newPreset);
    
    void setPreset(ISVPresetDataStr newPreset);
    
    void setToPreset(ISVPresetDataStr newPreset);
    
    ISVPresetDataStr getDeltaPreset() const { return this->param_sliders.getDeltaPreset(); }

    // ==================================================

    std::function<void()> paramChanged;
    std::function<void()> colourChanged;
    std::function<void()> saveAction;
private:
    
    void paint(Graphics& g) override;
    void resized() override;
    void changeListenerCallback(ChangeBroadcaster* source) override;

    // ==================================================

    ISVParameterSliders param_sliders;
    
    // ==================================================

    // ==================================================

    
};
#endif /* ISVParameterControlUI_hpp */
