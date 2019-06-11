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
        toPresetSelected,
        TransitionToInitialSphereChanged,
        TransitionBetweenPresetsChanged,
        TransitionOperatePressed,
        TransitionReversePressed,
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
    int getSelectedToPresetIndex() const { return this->toPreset.getSelectedId(); }
    
    float getTransitionToInitialSphereValue() const { return this->transitionToInitialSphere.getValue(); }
    float getTransitionBetweenPresetsValue() const { return this->transitionBetweenPresets.getValue(); }
    
    float getTransitionDuration() const { return this->durationSlider.getValue(); }
    
    void setTransitionBetweenPresets(float newVal);

    // ==================================================
    void clearAllPresetsAction();
    void operateTransitionAction();
    void opearateReverseTransitionAction();
    
    // ==================================================
    ISVPreferenceStatus getStatus() const { return this->status; }
    // ==================================================

    void addPresetItem(Array<ISVPresetDataStr> presets);
    // ==================================================

   
private:
    
    // slider component
    class sliderUI1 : public Component,
    private Slider::Listener
    {
    public:
        sliderUI1(ISVPreferenceUI* parent, String title, float min = 0.0, float max = 1.0, float initialVal = 1.0, float increment = 0.001)
        {
            this->parent = parent;
            this->initialVal = initialVal;
            label.setText(title, dontSendNotification);
            label.setColour(Label::textColourId, SYSTEMCOLOUR.titleText);
            addAndMakeVisible(&label);
            slider.setRange(min, max, increment);
            slider.setValue(initialVal);
            slider.addListener(this);
            addAndMakeVisible(&slider);
            button.setButtonText("reset");
            button.onClick = [this]{ resetButtonClicked(); };
            addAndMakeVisible(&button);
        }
        ~sliderUI1() {}
        
        void resized() override
        {
            label.setBounds(0, 0, getWidth(), 30);
            slider.setBounds(0, 30, getWidth() - 30, 30);
            button.setBounds(getWidth() - 30, 30, 30, 30);
        }
        
        void sliderValueChanged(Slider* slider) override
        {
            if(slider == &this->slider)
            {
                this->parent->sliderUIValueChanged(this);
            }
        }
        
        void resetButtonClicked()
        {
            slider.setValue(this->initialVal);
        }
        
        float getValue() const { return slider.getValue(); }
        void setValue(float val) { slider.setValue(val); }
        
    private:
        ISVPreferenceUI* parent;
        Label label;
        Slider slider;
        TextButton button;
        float initialVal = 0;
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    };
    
    // ==================================================
    
    void sliderUIValueChanged(sliderUI1 *obj);

    
    void changeListenerCallback(ChangeBroadcaster* source) override;

    ISVPreferenceStatus status;
    // ==================================================
    
    Label       LabelTitle;
    Label       PresetsLabel;
    ComboBox    PresetsBox;
    
    Label TransitionLabel;
    
    Label fromLabel;
    ComboBox fromPreset;
    Label toLabel;
    ComboBox toPreset;
    
    sliderUI1 transitionToInitialSphere;
    sliderUI1 transitionBetweenPresets;
    
    Label TransitionSpeedLabel;
    Label durationLabel;
    sliderUI1 durationSlider;
    
    TextButton TransitionButton;
    TextButton TransitionReverseButton;
    Label fpsLabel;
    Label RenderingStatusLabel;
    
    
    TextButton clearAllPresetsButton;

    // ==================================================
    
    ISVParameterControlWindow* controllerWindow = nullptr;

    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ISVPreferenceUI)

    
};
#endif /* ISVPreferenceUI_hpp */
