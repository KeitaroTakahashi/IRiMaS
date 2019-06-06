//
//  ISVPreferenceUI.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 01/06/2019.
//

#ifndef ISVPreferenceUI_hpp
#define ISVPreferenceUI_hpp

#include "JuceHeader.h"
#include "IRComponents.hpp"
#include "ISVParameterControlWindow.hpp"

class ISVPreferenceUI : public Component,
public ChangeBroadcaster,
private ChangeListener,
private Slider::Listener,
private ComboBox::Listener
{
public:
    
    enum ISVPreferenceStatus
    {
        PresetChanged,
        IntensitySliderChanged,
        FinenessSliderChanged,
        LightingColourChanged,
        MaterialColourChanged,
        PresetSelected,
        ClearAllPresets
    };
    
    ISVPreferenceUI(String title);
    ~ISVPreferenceUI();
    
    void resized() override;
    void paint(Graphics& g) override;
    
    // ==================================================
    void sliderValueChanged (Slider* slider) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    int getSelectedPresetIndex() const { return this->PresetsBox.getSelectedId(); }

    // ==================================================
    void clearAllPresetsAction();
    
    // ==================================================
    ISVPreferenceStatus getStatus() const { return this->status; }
    // ==================================================

    void addPresetItem(Array<ISVPresetDataStr> presets);
    // ==================================================

   
private:
    // ==================================================
    
    void changeListenerCallback(ChangeBroadcaster* source) override;

    ISVPreferenceStatus status;
    // ==================================================
    
    Label       LabelTitle;
    Label       PresetsLabel;
    ComboBox    PresetsBox;
    
    TextButton clearAllPresetsButton;

    // ==================================================
    
    ISVParameterControlWindow* controllerWindow = nullptr;

    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ISVPreferenceUI)

    
};
#endif /* ISVPreferenceUI_hpp */
