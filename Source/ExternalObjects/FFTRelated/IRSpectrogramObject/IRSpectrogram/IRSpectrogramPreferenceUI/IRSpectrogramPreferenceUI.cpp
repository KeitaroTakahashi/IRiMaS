//
//  IRSpectrogramPreferenceUI.cpp
//  IRiMaS
//
//  Created by Keitaro on 07/07/2019.
//

#include "IRSpectrogramPreferenceUI.hpp"

IRSpectrogramPreferenceUI::IRSpectrogramPreferenceUI(String title) :
magnitudeAmount(this, "magnitude", 0.0, 10.0, 1.0, 0.1),
verticalScale  (this, "vertical scale", 0.1, 50.0, 8.0, 1.0),
horizontalScale  (this, "horizontal scale", 1.0, 32.0, 1.0, 1.0)

{
    addAndMakeVisible(this->LabelTitle);
    this->LabelTitle.setText(title, dontSendNotification);
    this->LabelTitle.setColour(Label::textColourId, Colours::black);
    this->LabelTitle.setJustificationType(Justification::left);
    
    addAndMakeVisible(&this->magnitudeAmount);
    
    addAndMakeVisible(&this->verticalScale);
    addAndMakeVisible(&this->horizontalScale);

}

IRSpectrogramPreferenceUI::~IRSpectrogramPreferenceUI()
{
    
}

void IRSpectrogramPreferenceUI::resized()
{
    this->LabelTitle.setBounds          (20, 10, 300, 30);

    this->magnitudeAmount.setBounds     (20, 50, getWidth() - 40, 70);
    this->verticalScale.  setBounds(20, 130, getWidth() - 40, 70);
    this->horizontalScale.  setBounds(20, 210, getWidth() - 40, 70);

}

void IRSpectrogramPreferenceUI::paint(Graphics &g)
{
    g.fillAll(SYSTEMCOLOUR.contents.brighter());
    g.setColour(Colours::black);
    g.drawLine(0,42.5,getWidth(),42.5);
    
    g.drawLine(0,130, getWidth(), 130);
}
// ==================================================

void IRSpectrogramPreferenceUI::sliderUIValueChanged(sliderUI1 *obj)
{
    if(obj == &this->magnitudeAmount)
    {
        this->status = magnitudeAmountChanged;
        sendChangeMessage();
    }else if(obj == &this->verticalScale)
    {
        this->status = verticalScaleChanged;
        sendChangeMessage();
    }else if(obj == &this->horizontalScale)
    {
        this->status = horizontalScaleChanged;
        sendChangeMessage();
    }
}
// ==================================================

void IRSpectrogramPreferenceUI::changeListenerCallback(ChangeBroadcaster* source)
{
    
}
// ==================================================
