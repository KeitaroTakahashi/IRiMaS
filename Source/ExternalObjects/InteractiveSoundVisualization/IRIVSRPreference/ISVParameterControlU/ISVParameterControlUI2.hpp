//
//  ISVParameterControlUI2.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 02/06/2019.
//

#ifndef ISVParameterControlUI2_hpp
#define ISVParameterControlUI2_hpp

#include "ISVParameterSliders.hpp"

class ISVParameterControlUI2 : public Component,
public ChangeBroadcaster
{
public:
    
    enum ISVParameterControlUI2Status
    {
        NONE,
        SliderValueChanged
    };
    ISVParameterControlUI2();
    ~ISVParameterControlUI2();
    // ==================================================

    void paint(Graphics& g) override;
    void resized() override;
    
    // ==================================================
    //ISVPresetDataStr* getPreset1() const { return this->parameterSliders1.getPreset1(); }
    //void setPreset1(ISVPresetDataStr newPreset) { this->preset1 = newPreset; }
    // ==================================================
    
    
    
    float getStretch_x_sei() const { return this->stretch_x_sei.getValue(); }
    float getStretch_x_fu() const { return this->stretch_x_fu.getValue(); }

    float getStretch_y_sei() const { return this->stretch_y_sei.getValue(); }
    float getStretch_y_fu() const { return this->stretch_y_fu.getValue(); }
    
    float getStretch_z_sei() const { return this->stretch_z_sei.getValue(); }
    float getStretch_z_fu() const { return this->stretch_z_fu.getValue(); }
    
    float getStretch_x_1() const { return this->stretch_x_1.getValue(); }
    float getStretch_x_2() const { return this->stretch_x_2.getValue(); }
    float getStretch_x_3() const { return this->stretch_x_3.getValue(); }
    float getStretch_x_4() const { return this->stretch_x_4.getValue(); }
    float getStretch_y_1() const { return this->stretch_y_1.getValue(); }
    float getStretch_y_2() const { return this->stretch_y_2.getValue(); }
    float getStretch_y_3() const { return this->stretch_y_3.getValue(); }
    float getStretch_y_4() const { return this->stretch_y_4.getValue(); }
    
    float getStretchAmount() const { return this->stretch_amount.getValue(); }
    float getXIndex() const { return this->xIndex.getValue(); }
    float getYIndex() const { return this->yIndex.getValue(); }
    float getZIndex() const { return this->zIndex.getValue(); }
    // ==================================================

    ISVParameterControlUI2Status getStatus() const { return this->status; }
private:
    
    ISVParameterControlUI2Status status;
    void stretchUI();
    // ==================================================
    
    //std::shared_ptr<ISVParameterSliders> parameterSliders1;
    // ==================================================


    
    // slider component
    class sliderUI1 : public Component,
    private Slider::Listener
    {
    public:
        sliderUI1(ISVParameterControlUI2* parent, String title, float min = 0.0, float max = 1.0, float initialVal = 1.0, float increment = 0.001)
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
        ISVParameterControlUI2* parent;
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
    
    ISVPresetDataStr preset1;
    
    // ==================================================
    
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ISVParameterControlUI2)

};

#endif /* ISVParameterControlUI2_hpp */
