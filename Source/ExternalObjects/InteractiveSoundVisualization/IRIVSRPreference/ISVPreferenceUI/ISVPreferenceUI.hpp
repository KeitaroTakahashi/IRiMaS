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
        MaterialColourChanged
    };
    
    ISVPreferenceUI(String title);
    ~ISVPreferenceUI();
    
    void resized() override;
    void paint(Graphics& g) override;
    
    // ==================================================
    void sliderValueChanged (Slider* slider) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

    // ==================================================

    float getIntensitySliderValue() const;
    float getFinenessSliderValue() const;
    Colour getLightingColour() const;
    Colour getMaterialColour() const;
    
    float getStretch_x_sei() const;
    float getStretch_x_fu() const;
    
    float getStretch_y_sei() const;
    float getStretch_y_fu() const;
    
    float getStretch_z_sei() const;
    float getStretch_z_fu() const;
    
    float getStretch_x_1() const;
    float getStretch_x_2() const;
    float getStretch_x_3() const;
    float getStretch_x_4() const;
    float getStretch_y_1() const;
    float getStretch_y_2() const;
    float getStretch_y_3() const;
    float getStretch_y_4() const;

    float getStretchAmount() const;
    float getXIndex() const;
    float getYIndex() const;
    float getZIndex() const;
    
    // ==================================================
    ISVPreferenceStatus getStatus() const { return this->status; }
    // ==================================================

    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void intensitySliderChanged(ISVPreferenceUI* obj) {}
        virtual void finenessSliderChanged(ISVPreferenceUI* obj) {}
        virtual void lightingColourChanged(ISVPreferenceUI* obj) {}
        virtual void materialColourChanged(ISVPreferenceUI* obj) {}
        
        virtual void stretchValueChanged(ISVPreferenceUI* obj) {}

    };
    
    virtual void addListener(Listener* newListener) { this->listeners.add(newListener); }
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener);}
    ListenerList<Listener> listeners;
    
    void callIntentisySliderChanged(ISVPreferenceUI* obj);
    void callFinenessSliderChanged(ISVPreferenceUI* obj);
    void callLightingColourChanged(ISVPreferenceUI* obj);
    void callMaterialColourChanged(ISVPreferenceUI* obj);

    void callStretchValueChanged(ISVPreferenceUI* obj);
    // ==================================================

    

private:
    // ==================================================

    void openControllerAction();
    
    void changeListenerCallback(ChangeBroadcaster* source) override;

    ISVPreferenceStatus status;
    // ==================================================
    
    TextButton openControllerButton;

    Label       LabelTitle;
    Label       PresetsLabel;
    ComboBox    PresetsBox;
    
    Label       IntensityLabel;
    Slider      IntensitySlider;
    Label       FinenessLabel;
    Slider      FinenessSlider;
    
    Label       LightingColourLabel;
    
    ColourSelector lightingColour
    {
        ColourSelector::showSliders
        | ColourSelector::showColourspace
        | ColourSelector::showAlphaChannel
    };
    
    Label       MaterialColourLabel;
    
    ColourSelector materialColour
    {
        ColourSelector::showSliders
        | ColourSelector::showColourspace
        | ColourSelector::showAlphaChannel
    };
    
    // ==================================================
    
    ISVParameterControlWindow* controllerWindow = nullptr;

    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ISVPreferenceUI)

    
};
#endif /* ISVPreferenceUI_hpp */
