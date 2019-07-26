//
//  IRSpectrogramPreferenceUI.hpp
//  IRiMaS
//
//  Created by Keitaro on 07/07/2019.
//

#ifndef IRSpectrogramPreferenceUI_hpp
#define IRSpectrogramPreferenceUI_hpp
#include "JuceHeader.h"
#include "ColourLib.h"

class IRSpectrogramPreferenceUI : public Component,
public ChangeBroadcaster,
private ChangeListener
{
public:
    
    enum PreferenceStatus
    {
        magnitudeAmountChanged,
        verticalScaleChanged,
        horizontalScaleChanged
    };
    
    IRSpectrogramPreferenceUI(String title);
    ~IRSpectrogramPreferenceUI();
    
    void resized() override;
    
    void paint(Graphics& g) override;
    
    // --------------------------------------------------
    float getMagnitudeAmount() const { return this->magnitudeAmount.getValue(); }
    float getVerticalScale()   const { return this->verticalScale.getValue(); }
    float getHorizontalScale() const { return this->horizontalScale.getValue(); }

    // --------------------------------------------------

    PreferenceStatus getStatus() const { return this->status; }
private:
    
    // slider component
    class sliderUI1 : public Component,
    private Slider::Listener
    {
    public:
        sliderUI1(IRSpectrogramPreferenceUI* parent, String title, float min = 0.0, float max = 1.0, float initialVal = 1.0, float increment = 0.001)
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
        IRSpectrogramPreferenceUI* parent;
        Label label;
        Slider slider;
        TextButton button;
        float initialVal = 0;
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    };
    // ==================================================
    
    void sliderUIValueChanged(sliderUI1 *obj);
    
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    PreferenceStatus status;
    // ==================================================
    
    Label LabelTitle;
    sliderUI1 magnitudeAmount;
    sliderUI1 verticalScale;
    sliderUI1 horizontalScale;
    // ==================================================
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRSpectrogramPreferenceUI)

};
#endif /* IRSpectrogramPreferenceUI_hpp */
