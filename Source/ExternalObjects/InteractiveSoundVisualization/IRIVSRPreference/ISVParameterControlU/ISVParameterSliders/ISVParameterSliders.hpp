//
//  ISVParameterSliders.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/06/2019.
//

#ifndef ISVParameterSliders_hpp
#define ISVParameterSliders_hpp

#include "JuceHeader.h"
#include "ISVPresetDataStr.h"
#include "ColourLib.h"

class ISVParameterSliders : public Component,
public ChangeBroadcaster
{
public:
    
    enum ISVParameterSlidersStatus
    {
        NONE,
        SliderValueChanged
    };
    
    ISVParameterSliders();
    ~ISVParameterSliders();
    // ==================================================
    // ==================================================
    // ==================================================
    ISVPresetDataStr getPreset1() const { return this->preset1; }
    void setPreset1(ISVPresetDataStr newPreset) { this->preset1 = newPreset; }
    // ==================================================

    ISVParameterSlidersStatus getStatus() const { return this->status; }

    // ==================================================

private:
    
    void paint(Graphics& g) override;
    void resized() override;
    
    
    
    // ==================================================


    // slider component
    class sliderUI1 : public Component,
    private Slider::Listener
    {
    public:
        sliderUI1(ISVParameterSliders* parent, String title, float min = 0.0, float max = 1.0, float initialVal = 1.0, float increment = 0.001)
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
        
    private:
        ISVParameterSliders* parent;
        Label label;
        Slider slider;
        TextButton button;
        float initialVal = 0;
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    };
    
    // ==================================================

    // slider appearance
    int sliderW = 300;
    int sliderH = 70;
    
    int sliderMarginX = 20;
    int sliderMarginY = 0;
    
    // sliders
    sliderUI1 stretch_x_sei;
    sliderUI1 stretch_x_fu;
    
    sliderUI1 stretch_y_sei;
    sliderUI1 stretch_y_fu;
    
    sliderUI1 stretch_z_sei;
    sliderUI1 stretch_z_fu;
    
    sliderUI1 stretch_x_1;
    sliderUI1 stretch_x_2;
    sliderUI1 stretch_x_3;
    sliderUI1 stretch_x_4;
    
    sliderUI1 stretch_y_1;
    sliderUI1 stretch_y_2;
    sliderUI1 stretch_y_3;
    sliderUI1 stretch_y_4;
    
    sliderUI1 stretch_amount;
    
    sliderUI1 xIndex;
    sliderUI1 yIndex;
    sliderUI1 zIndex;
    
    // data
    ISVPresetDataStr preset1;

    
    // ==================================================
    
    ISVParameterSlidersStatus status;
    
    void sliderUIValueChanged(sliderUI1* obj);

    
    // ==================================================

    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ISVParameterSliders)

};

#endif /* ISVParameterSliders_hpp */
