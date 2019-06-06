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
lightPosition_x     (this, "Light Position", -100.0, 100.0, 5.0),
lightPosition_y     (this, "Light Position", -100.0, 100.0, 1.0),
lightPosition_z     (this, "Light Position", -50.0, 50.0, 15.0)

{
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
    this->stretch_amount.setBounds      (sideX, y,
                                         w, h);
    y += 90;
    this->SavePresetInput.setBounds(sideX, y, w - 70, 30);
    this->SaveButton.setBounds(sideX + (w - 60), y, 60, 30);
    
    y += 30;
    this->LightingColourLabel.setBounds(sideX, y, w, 30);
    y += 30;
    this->lightingColour.setBounds      (sideX, y, w, 150);
    
    
    
}
// ==================================================
void ISVParameterSliders::sliderUIValueChanged(sliderUI1* obj)
{
    
    this->preset1.lightPosition = Rectangle<float> (this->lightPosition_x.getValue(),
                                                    this->lightPosition_y.getValue(),
                                                    this->lightPosition_z.getValue(),
                                                    0.0);
    this->preset1.quaterStretchX = Rectangle<float> (this->stretch_x_1.getValue(),
                                                     this->stretch_x_2.getValue(),
                                                     this->stretch_x_3.getValue(),
                                                     this->stretch_x_4.getValue());
    this->preset1.quaterStretchY = Rectangle<float> (this->stretch_y_1.getValue(),
                                                     this->stretch_y_2.getValue(),
                                                     this->stretch_y_3.getValue(),
                                                     this->stretch_y_4.getValue());
    this->preset1.halfStretchX = Point<float> (this->stretch_x_sei.getValue(),
                                               this->stretch_x_fu.getValue());
    this->preset1.halfStretchY = Point<float> (this->stretch_y_sei.getValue(),
                                               this->stretch_y_fu.getValue());
    this->preset1.halfStretchZ = Point<float> (this->stretch_z_sei.getValue(),
                                               this->stretch_z_fu.getValue());
    this->preset1.transform = Rectangle<float> (this->xIndex.getValue(),
                                                this->yIndex.getValue(),
                                                this->zIndex.getValue(),
                                                0.0);
    this->preset1.intensity = this->intensity.getValue();
    this->preset1.fineness = this->fineness.getValue();
    
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

void ISVParameterSliders::setPreset1(ISVPresetDataStr newPreset)
{
    this->preset1 = newPreset;
    
    Rectangle<float> lp = this->preset1.lightPosition;
    this->lightPosition_x.setValue(lp.getX());
    this->lightPosition_y.setValue(lp.getY());
    this->lightPosition_z.setValue(lp.getWidth());
    
    Colour mc = this->preset1.materialColour;
    this->materialColour.setCurrentColour(mc);
    
    Colour lc = this->preset1.lightingColour;
    this->lightingColour.setCurrentColour(lc);
    
    Rectangle<float> qX = this->preset1.quaterStretchX;
    this->stretch_x_1.setValue(qX.getX());
    this->stretch_x_2.setValue(qX.getY());
    this->stretch_x_3.setValue(qX.getWidth());
    this->stretch_x_4.setValue(qX.getHeight());
    
    Rectangle<float> qY = this->preset1.quaterStretchY;
    this->stretch_y_1.setValue(qY.getX());
    this->stretch_y_2.setValue(qY.getY());
    this->stretch_y_3.setValue(qY.getWidth());
    this->stretch_y_4.setValue(qY.getHeight());
    
    Point<float> hX = this->preset1.halfStretchX;
    this->stretch_x_sei.setValue(hX.getX());
    this->stretch_x_fu.setValue(hX.getY());
    
    Point<float> hY = this->preset1.halfStretchY;
    this->stretch_y_sei.setValue(hY.getX());
    this->stretch_y_fu.setValue(hY.getY());
    
    Rectangle<float> tm = this->preset1.transform;
    this->xIndex.setValue(tm.getX());
    this->yIndex.setValue(tm.getY());
    this->zIndex.setValue(tm.getWidth());

    this->intensity.setValue(this->preset1.intensity);
    this->fineness.setValue(this->preset1.fineness);
    this->stretch_amount.setValue(this->preset1.amount);

}

