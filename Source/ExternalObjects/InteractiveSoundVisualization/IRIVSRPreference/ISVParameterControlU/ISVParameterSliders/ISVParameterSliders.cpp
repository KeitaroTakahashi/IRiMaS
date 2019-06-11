//
//  ISVParameterSliders.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/06/2019.
//

#include "ISVParameterSliders.hpp"

ISVParameterSliders::ISVParameterSliders() :
stretch_x_sei       (this, "Stretch X positive axis", 0.0, 5.0, 1.0),
stretch_x_fu        (this, "Stretch X negative axis", 0.0, 5.0, 1.0),
stretch_y_sei       (this, "Stretch Y positive axis", 0.0, 5.0, 1.0),
stretch_y_fu        (this, "Stretch Y negative axis", 0.0, 5.0, 1.0),
stretch_z_sei       (this, "Stretch Z positive axis", 0.0, 5.0, 1.0),
stretch_z_fu        (this, "Stretch Z negative axis", 0.0, 5.0, 1.0),
stretch_x_1         (this, "Stretch X Top left", 0.8, 1.5, 1.0),
stretch_x_2         (this, "Stretch X Top right", 0.8, 1.5, 1.0),
stretch_x_3         (this, "Stretch X Bottom left", 0.8, 1.5, 1.0),
stretch_x_4         (this, "Stretch X Bottom right", 0.8, 1.5, 1.0),
stretch_y_1         (this, "Stretch Y Top left", 0.8, 1.5, 1.0),
stretch_y_2         (this, "Stretch Y Top right", 0.8, 1.5, 1.0),
stretch_y_3         (this, "Stretch Y Bottom left", 0.8, 1.5, 1.0),
stretch_y_4         (this, "Stretch Y Bottom right", 0.8, 1.5, 1.0),
stretch_amount      (this, "Stretch Amount", 0.0, 1.0, 1.0),
xIndex              (this, "X Index", -10.0, 10.0, 0.0),
yIndex              (this, "Y Index", -10.0, 10.0, 0.0),
zIndex              (this, "Z Index", -100.0, 0.0, -18.0),
intensity           (this, "Intensity", 0.0, 5.0, 0.0),
fineness            (this, "Fineness", 0.0, 1.0, 0.0),
vertexNoiseSpeed    (this, "VertexNoise Speed", 0.0, 0.1, 0.0),
lightPosition_x     (this, "Light Position", -100.0, 100.0, 5.0),
lightPosition_y     (this, "Light Position", -100.0, 100.0, 1.0),
lightPosition_z     (this, "Light Position", -50.0, 50.0, 15.0)

{
    
    this->resetButton.setButtonText("Reset All");
    this->resetButton.onClick = [this] { resetAction(); };
    addAndMakeVisible(&this->resetButton);
    
    
    addAndMakeVisible(&this->stretch_x_sei);
    addAndMakeVisible(&this->stretch_x_fu);
    addAndMakeVisible(&this->stretch_y_sei);
    addAndMakeVisible(&this->stretch_y_fu);
    addAndMakeVisible(&this->stretch_z_sei);
    addAndMakeVisible(&this->stretch_z_fu);
    
    addAndMakeVisible(&this->stretch_amount);
    
    addAndMakeVisible(&this->xIndex);
    addAndMakeVisible(&this->yIndex);
    addAndMakeVisible(&this->zIndex);
    
    addAndMakeVisible(&this->lightPosition_x);
    addAndMakeVisible(&this->lightPosition_y);
    addAndMakeVisible(&this->lightPosition_z);
    
    addAndMakeVisible(&this->stretch_x_1);
    addAndMakeVisible(&this->stretch_x_2);
    addAndMakeVisible(&this->stretch_x_3);
    addAndMakeVisible(&this->stretch_x_4);
    addAndMakeVisible(&this->stretch_y_1);
    addAndMakeVisible(&this->stretch_y_2);
    addAndMakeVisible(&this->stretch_y_3);
    addAndMakeVisible(&this->stretch_y_4);
    
    addAndMakeVisible(&this->intensity);
    addAndMakeVisible(&this->fineness);
    addAndMakeVisible(&this->vertexNoiseSpeed);
    
    this->LightingColourLabel.setText("Lighting", dontSendNotification);
    addAndMakeVisible(&this->LightingColourLabel);
    addAndMakeVisible(&this->lightingColour);
    this->lightingColour.addChangeListener(this);
    
    this->MaterialColourLabel.setText("Material", dontSendNotification);
    addAndMakeVisible(&this->MaterialColourLabel);

    addAndMakeVisible(&this->materialColour);
    this->materialColour.addChangeListener(this);
    
    this->SavePresetInput.setEditable(true);
    this->SavePresetInput.setColour(Label::backgroundColourId, Colours::white);
    this->SavePresetInput.setColour(Label::outlineColourId, Colours::black);
    this->SavePresetInput.setColour(Label::textColourId, Colours::black);
    this->SavePresetInput.setColour(Label::textWhenEditingColourId, Colours::grey);

    addAndMakeVisible(&this->SavePresetInput);
    this->SaveButton.setButtonText("Save");
    addAndMakeVisible(&this->SaveButton);
    this->SaveButton.onClick = [this] { saveAction(); };
    

}

ISVParameterSliders::~ISVParameterSliders()
{
    
}
// ==================================================

void ISVParameterSliders::paint(Graphics& g)
{
    
}

void ISVParameterSliders::resized()
{
    int w = this->sliderW;
    int h = this->sliderH;
    
    int x = this->sliderMarginX;
    int y = this->sliderMarginY;
    
    this->stretch_x_sei.setBounds(x, y,
                                  w, h);
    y += 60;
    this->stretch_x_fu.setBounds(20, y,
                                 w, h);
    y += 60;
    this->stretch_y_sei.setBounds(20, y,
                                  w, h);
    y += 60;
    this->stretch_y_fu.setBounds(20, y,
                                 w, h);
    y += 60;
    this->stretch_z_sei.setBounds(20, y,
                                  w, h);
    y += 60;
    this->stretch_z_fu.setBounds(20, y,
                                 w, h);
   
    y += 60;
    this->xIndex.setBounds        (20, y,
                                   w, h);
    y += 60;
    this->yIndex.setBounds        (20, y,
                                   w, h);
    y += 60;
    this->zIndex.setBounds        (20, y,
                                   w, h);
    
    y += 60;
    this->lightPosition_x.setBounds     (20, y,
                                         w, h);
    y += 60;
    this->lightPosition_y.setBounds     (20, y,
                                         w, h);
    y += 60;
    this->lightPosition_z.setBounds     (20, y,
                                         w, h);
    y += 60;
    this->MaterialColourLabel.setBounds(20, y, w, 30);
    y += 30;
    this->materialColour.setBounds   (20, y, w, 150);

    // second line
    int sideX = w + 30;
    y = 0;
    this->stretch_x_1.setBounds         (sideX, y,
                                         w, h);
    y += 60;
    this->stretch_x_2.setBounds         (sideX, y,
                                         w, h);
    y += 60;
    this->stretch_x_3.setBounds         (sideX, y,
                                         w, h);
    y += 60;
    this->stretch_x_4.setBounds         (sideX, y,
                                         w, h);
    y += 60;
    this->stretch_y_1.setBounds         (sideX, y,
                                         w, h);
    y += 60;
    this->stretch_y_2.setBounds         (sideX, y,
                                         w, h);
    y += 60;
    this->stretch_y_3.setBounds         (sideX, y,
                                         w, h);
    y += 60;
    this->stretch_y_4.setBounds         (sideX, y,
                                         w, h);
    
    y += 60;
    this->intensity.setBounds            (sideX, y,
                                          w, h);
    
    y += 60;
    this->fineness.setBounds            (sideX, y,
                                        w, h);
    
    y += 60;
    this->vertexNoiseSpeed.setBounds    (sideX, y,
                                         w, h);
    
    y += 60;
    this->stretch_amount.setBounds      (sideX, y,
                                         w, h);
   
    y += 60;
    this->LightingColourLabel.setBounds(sideX, y, w, 30);
    y += 30;
    this->lightingColour.setBounds      (sideX, y, w, 150);
    
    // Third Line
    y = 30;
    sideX += w + 30;
    this->resetButton.setBounds(sideX, y, 100, 30);
    
    y += 40;
    this->SavePresetInput.setBounds(sideX, y, 100, 30);
    y += 40;
    this->SaveButton.setBounds(sideX, y, 60, 30);
    

    
}
// ==================================================
void ISVParameterSliders::sliderUIValueChanged(sliderUI1* obj)
{
    
    this->preset1.lightPosition = KVector<float> (this->lightPosition_x.getValue(),
                                                    this->lightPosition_y.getValue(),
                                                    this->lightPosition_z.getValue(),
                                                    0.0);
    this->preset1.quaterStretchX = KVector<float> (this->stretch_x_1.getValue(),
                                                     this->stretch_x_2.getValue(),
                                                     this->stretch_x_3.getValue(),
                                                     this->stretch_x_4.getValue());
    this->preset1.quaterStretchY = KVector<float> (this->stretch_y_1.getValue(),
                                                     this->stretch_y_2.getValue(),
                                                     this->stretch_y_3.getValue(),
                                                     this->stretch_y_4.getValue());
    this->preset1.halfStretchX = KVector<float> (this->stretch_x_sei.getValue(),
                                               this->stretch_x_fu.getValue());
    this->preset1.halfStretchY = KVector<float> (this->stretch_y_sei.getValue(),
                                               this->stretch_y_fu.getValue());
    this->preset1.halfStretchZ = KVector<float> (this->stretch_z_sei.getValue(),
                                               this->stretch_z_fu.getValue());
    this->preset1.transform = KVector<float> (this->xIndex.getValue(),
                                                this->yIndex.getValue(),
                                                this->zIndex.getValue(),
                                                0.0);
    this->preset1.intensity = this->intensity.getValue();
    this->preset1.fineness = this->fineness.getValue();
    this->preset1.vertexNoiseSpeed = this->vertexNoiseSpeed.getValue();
    
    this->preset1.amount = this->stretch_amount.getValue();
    

    this->status = SliderValueChanged;
    sendChangeMessage();
}
// ==================================================
void ISVParameterSliders::changeListenerCallback(ChangeBroadcaster* source)
{
    if( source == &this->lightingColour)
    {
        this->preset1.lightingColour = this->lightingColour.getCurrentColour();
        
        
        
        this->status = ColourChanged;
        sendChangeMessage();
    }else if( source == &this->materialColour)
    {
        this->preset1.materialColour = this->materialColour.getCurrentColour();
        this->status = ColourChanged;
        sendChangeMessage();
    }
}

// ==================================================
void ISVParameterSliders::saveAction()
{
    std::cout << "saveAction\n";
    
    int inputTextSize = (int)this->SavePresetInput.getText().toStdString().size();
    if(inputTextSize > 0)
    {
        this->status = SavePresetAction;
        sendChangeMessage();
    }
}
// ==================================================


// ==================================================

void ISVParameterSliders::setCurrentPreset(ISVPresetDataStr newPreset)
{
    this->currentPreset = newPreset;
    
    KVector<float> lp = this->currentPreset.lightPosition;
    this->lightPosition_x.setValue(lp.getVal1(), dontSendNotification);
    this->lightPosition_y.setValue(lp.getVal2(), dontSendNotification);
    this->lightPosition_z.setValue(lp.getVal3(), dontSendNotification);
    
    KVector<float> mc = this->currentPreset.materialColour;
    this->materialColour.setCurrentColour((Colour::fromFloatRGBA(mc.getVal1(), mc.getVal2(), mc.getVal3(), mc.getVal4())), dontSendNotification);
    
    KVector<float> lc = this->currentPreset.lightingColour;
    this->lightingColour.setCurrentColour(Colour::fromFloatRGBA(lc.getVal1(), lc.getVal2(), lc.getVal3(), lc.getVal4()), dontSendNotification);
    
    KVector<float> qX = this->currentPreset.quaterStretchX;
    this->stretch_x_1.setValue(qX.getVal1(), dontSendNotification);
    this->stretch_x_2.setValue(qX.getVal2(), dontSendNotification);
    this->stretch_x_3.setValue(qX.getVal3(), dontSendNotification);
    this->stretch_x_4.setValue(qX.getVal4(), dontSendNotification);
    
    KVector<float> qY = this->currentPreset.quaterStretchY;
    this->stretch_y_1.setValue(qY.getVal1(), dontSendNotification);
    this->stretch_y_2.setValue(qY.getVal2(), dontSendNotification);
    this->stretch_y_3.setValue(qY.getVal3(), dontSendNotification);
    this->stretch_y_4.setValue(qY.getVal4(), dontSendNotification);
    
    KVector<float> hX = this->currentPreset.halfStretchX;
    this->stretch_x_sei.setValue(hX.getVal1(), dontSendNotification);
    this->stretch_x_fu.setValue(hX.getVal2(), dontSendNotification);
    
    KVector<float> hY = this->preset1.halfStretchY;
    this->stretch_y_sei.setValue(hY.getVal1(), dontSendNotification);
    this->stretch_y_fu.setValue(hY.getVal2(), dontSendNotification);
    
    KVector<float> tm = this->currentPreset.transform;
    this->xIndex.setValue(tm.getVal1(), dontSendNotification);
    this->yIndex.setValue(tm.getVal2(), dontSendNotification);
    this->zIndex.setValue(tm.getVal3(), dontSendNotification);
    
    this->intensity.setValue(this->currentPreset.intensity, dontSendNotification);
    this->fineness.setValue(this->currentPreset.fineness, dontSendNotification);
    this->stretch_amount.setValue(this->currentPreset.amount, dontSendNotification);
    this->vertexNoiseSpeed.setValue(this->currentPreset.vertexNoiseSpeed, dontSendNotification);
}

void ISVParameterSliders::setPreset1(ISVPresetDataStr newPreset)
{
    std::cout << "setPreset1\n";
    this->preset1 = newPreset;
    
    // initially, currentPreset is the same with preset1
    setCurrentPreset(newPreset);
    calcDeltaPreset();

}

void ISVParameterSliders::setPreset2(ISVPresetDataStr newPreset)
{
    this->preset2 = newPreset;
    calcDeltaPreset();
}

void ISVParameterSliders::calcDeltaPreset()
{
    std::cout <<"calcDeltaPreset : \n";
    std::cout << "preset1 "; this->preset1.show();
    std::cout << "preset2 "; this->preset2.show();

    
    this->deltaPreset.presetTitle = this->preset1.presetTitle + "_" + this->preset2.presetTitle;
    
    ISVPresetDataStr d = this->preset1;
    ISVPresetDataStr p = this->preset2;
    
    this->deltaPreset = this->preset1 - this->preset2;
    
}

void ISVParameterSliders::resetAction()
{
    this->stretch_x_sei.resetButtonClicked();
    this->stretch_x_fu.resetButtonClicked();
    this->stretch_y_sei.resetButtonClicked();
    this->stretch_y_fu.resetButtonClicked();
    this->stretch_z_sei.resetButtonClicked();
    this->stretch_z_fu.resetButtonClicked();
    
    this->stretch_x_1.resetButtonClicked();
    this->stretch_x_2.resetButtonClicked();
    this->stretch_x_3.resetButtonClicked();
    this->stretch_x_4.resetButtonClicked();
    this->stretch_y_1.resetButtonClicked();
    this->stretch_y_2.resetButtonClicked();
    this->stretch_y_3.resetButtonClicked();
    this->stretch_y_4.resetButtonClicked();
    
    this->stretch_amount.resetButtonClicked();
    this->xIndex.resetButtonClicked();
    this->yIndex.resetButtonClicked();
    this->zIndex.resetButtonClicked();
    
    this->intensity.resetButtonClicked();
    this->fineness.resetButtonClicked();
    this->vertexNoiseSpeed.resetButtonClicked();
    
    this->lightPosition_x.resetButtonClicked();
    this->lightPosition_y.resetButtonClicked();
    this->lightPosition_z.resetButtonClicked();

    this->materialColour.setCurrentColour(Colours::white);
    this->lightingColour.setCurrentColour(Colours::black);

}
