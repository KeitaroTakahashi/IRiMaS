//
//  ISVParameterControlUI.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/06/2019.
//

#include "ISVParameterControlUI.hpp"

ISVParameterControlUI::ISVParameterControlUI()
{
    addAndMakeVisible(&this->param_sliders);
    this->param_sliders.addChangeListener(this);
}

ISVParameterControlUI::~ISVParameterControlUI()
{
    
}

// ==================================================

void ISVParameterControlUI::paint(Graphics& g)
{
    
}

void ISVParameterControlUI::resized()
{
    this->param_sliders.setBounds(0, 0, getWidth(), getHeight());
}
// ==================================================

void ISVParameterControlUI::changeListenerCallback(ChangeBroadcaster* source)
{
    if(source == &this->param_sliders)
    {
        ISVParameterSliders::ISVParameterSlidersStatus status = this->param_sliders.getStatus();
        
        if(status == ISVParameterSliders::ISVParameterSlidersStatus::SliderValueChanged)
        {
            if(this->paramChanged != nullptr) this->paramChanged();
        }else if(status == ISVParameterSliders::ISVParameterSlidersStatus::ColourChanged)
        {
            if(this->colourChanged != nullptr) this->colourChanged();
        }else if(status == ISVParameterSliders::ISVParameterSlidersStatus::SavePresetAction)
        {
            if(this->saveAction != nullptr) this->saveAction();
        }
    }
}
// ==================================================

ISVPresetDataStr ISVParameterControlUI::getPreset1() const
{
    return this->param_sliders.getPreset1();
}

String ISVParameterControlUI::getPresetSaveName() const
{
    return this->param_sliders.getPresetName();
}

void ISVParameterControlUI::setPreset(ISVPresetDataStr newPreset)
{
    this->param_sliders.setPreset1(newPreset);
}

