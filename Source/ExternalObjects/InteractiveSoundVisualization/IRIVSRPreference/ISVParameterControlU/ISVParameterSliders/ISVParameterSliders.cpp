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
zIndex              (this, "Z Index", -100.0, 0.0, -18.0)
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
    
    addAndMakeVisible(&this->stretch_x_1);
    addAndMakeVisible(&this->stretch_x_2);
    addAndMakeVisible(&this->stretch_x_3);
    addAndMakeVisible(&this->stretch_x_4);
    addAndMakeVisible(&this->stretch_y_1);
    addAndMakeVisible(&this->stretch_y_2);
    addAndMakeVisible(&this->stretch_y_3);
    addAndMakeVisible(&this->stretch_y_4);
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
    y += 70;
    this->stretch_x_fu.setBounds(20, y,
                                 w, h);
    y += 70;
    this->stretch_y_sei.setBounds(20, y,
                                  w, h);
    y += 70;
    this->stretch_y_fu.setBounds(20, y,
                                 w, h);
    y += 70;
    this->stretch_z_sei.setBounds(20, y,
                                  w, h);
    y += 70;
    this->stretch_z_fu.setBounds(20, y,
                                 w, h);
    y += 70;
    this->stretch_amount.setBounds(20, y,
                                   w, h);
    y += 70;
    this->xIndex.setBounds        (20, y,
                                   w, h);
    y += 70;
    this->yIndex.setBounds        (20, y,
                                   w, h);
    y += 70;
    this->zIndex.setBounds        (20, y,
                                   w, h);
    
    
    // second line
    int sideX = w + 30;
    y = 0;
    this->stretch_x_1.setBounds         (sideX, y,
                                         w, h);
    y += 70;
    this->stretch_x_2.setBounds         (sideX, y,
                                         w, h);
    y += 70;
    this->stretch_x_3.setBounds         (sideX, y,
                                         w, h);
    y += 70;
    this->stretch_x_4.setBounds         (sideX, y,
                                         w, h);
    y += 70;
    this->stretch_y_1.setBounds         (sideX, y,
                                         w, h);
    y += 70;
    this->stretch_y_2.setBounds         (sideX, y,
                                         w, h);
    y += 70;
    this->stretch_y_3.setBounds         (sideX, y,
                                         w, h);
    y += 70;
    this->stretch_y_4.setBounds         (sideX, y,
                                         w, h);
}
// ==================================================
void ISVParameterSliders::sliderUIValueChanged(sliderUI1* obj)
{
    this->status = SliderValueChanged;
    sendChangeMessage();
}
// ==================================================

// ==================================================

// ==================================================

// ==================================================
