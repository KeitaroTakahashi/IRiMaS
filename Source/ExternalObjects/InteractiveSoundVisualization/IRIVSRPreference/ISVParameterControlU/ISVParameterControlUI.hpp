//
//  ISVParameterControlUI.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 02/06/2019.
//

#ifndef ISVParameterControlUI_hpp
#define ISVParameterControlUI_hpp

#include "JuceHeader.h"
#include "ColourLib.h"

class ISVParameterControlUI : public Component,
public ChangeBroadcaster
{
public:
    
    enum ISVParameterControlUIStatus
    {
        NONE,
        SliderValueChanged
    };
    ISVParameterControlUI();
    ~ISVParameterControlUI();
    // ==================================================

    void paint(Graphics& g) override;
    void resized() override;
    
    // ==================================================

    // ==================================================
    
    float getStretch_x_sei() const { return this->stretch_x_sei.getValue(); }
    float getStretch_x_fu() const { return this->stretch_x_fu.getValue(); }

    float getStretch_y_sei() const { return this->stretch_y_sei.getValue(); }
    float getStretch_y_fu() const { return this->stretch_y_fu.getValue(); }
    
    float getStretch_z_sei() const { return this->stretch_z_sei.getValue(); }
    float getStretch_z_fu() const { return this->stretch_z_fu.getValue(); }
    
    float getStretchAmount() const { return this->stretch_amount.getValue(); }
    float getZIndex() const { return this->zIndex.getValue(); }
    // ==================================================

    ISVParameterControlUIStatus getStatus() const { return this->status; }
private:
    
    ISVParameterControlUIStatus status;
    void stretchUI();
    // ==================================================

    
    // slider component
    class sliderUI1 : public Component,
    private Slider::Listener
    {
    public:
        sliderUI1(ISVParameterControlUI* parent, String title, float min = 0.0, float max = 1.0, float initialVal = 1.0, float increment = 0.001)
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
                //callSliderUIValueChanged();
                this->parent->sliderUIValueChanged(this);
            }
        }
        
        void resetButtonClicked()
        {
            slider.setValue(this->initialVal);
        }
        
        float getValue() const { return slider.getValue(); }
    
    private:
        ISVParameterControlUI* parent;
        Label label;
        Slider slider;
        TextButton button;
        float initialVal = 0;
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    };
    
    // listener
    void sliderUIValueChanged(sliderUI1* obj);

    
    Rectangle<float> stretch_circle;
    
    sliderUI1 stretch_x_sei;
    sliderUI1 stretch_x_fu;
    
    sliderUI1 stretch_y_sei;
    sliderUI1 stretch_y_fu;
    
    sliderUI1 stretch_z_sei;
    sliderUI1 stretch_z_fu;
    
    sliderUI1 stretch_amount;
    
    sliderUI1 zIndex;
    
    // ==================================================
    
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ISVParameterControlUI)

};

#endif /* ISVParameterControlUI_hpp */
