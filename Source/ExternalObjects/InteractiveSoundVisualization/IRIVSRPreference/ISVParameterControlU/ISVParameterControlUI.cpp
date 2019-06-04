//
//  ISVParameterControlUI.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 02/06/2019.
//

#include "ISVParameterControlUI.hpp"

ISVParameterControlUI::ISVParameterControlUI() :
stretch_circle  (200, 50, 400, 400),
stretch_x_sei   (this, "Stretch X positive axis", 0.0, 5.0, 1.0),
stretch_x_fu    (this, "Stretch X negative axis", 0.0, 5.0, 1.0),
stretch_y_sei   (this, "Stretch Y positive axis", 0.0, 5.0, 1.0),
stretch_y_fu    (this, "Stretch Y negative axis", 0.0, 5.0, 1.0),
stretch_z_sei   (this, "Stretch Z positive axis", 0.0, 5.0, 1.0),
stretch_z_fu    (this, "Stretch Z negative axis", 0.0, 5.0, 1.0),
stretch_amount  (this, "Stretch Amount", 0.0, 1.0, 1.0),
zIndex          (this, "Z Index", -100.0, 0.0, -18.0)
{
    
    addAndMakeVisible(&this->stretch_x_sei);
    addAndMakeVisible(&this->stretch_x_fu);
    
    addAndMakeVisible(&this->stretch_y_sei);
    addAndMakeVisible(&this->stretch_y_fu);
    
    addAndMakeVisible(&this->stretch_z_sei);
    addAndMakeVisible(&this->stretch_z_fu);
    
    addAndMakeVisible(&this->stretch_amount);
    addAndMakeVisible(&this->zIndex);

    

}

ISVParameterControlUI::~ISVParameterControlUI()
{
    
}
// ==================================================

void ISVParameterControlUI::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.contents.brighter());
    g.drawEllipse(this->stretch_circle, 2);
}

void ISVParameterControlUI::resized()
{
    Rectangle<float> c = this->stretch_circle;
    int w = 300;
    int h = 70;
    
    this->stretch_x_sei.setBounds(20, 40,
                                  w, h);
    
    this->stretch_x_fu.setBounds(20, 110,
                                 w, h);
    
    
    this->stretch_y_sei.setBounds(20, 180,
                                  w, h);
    
    this->stretch_y_fu.setBounds(20, 250,
                                 w, h);
    
    this->stretch_z_sei.setBounds(20, 320,
                                  w, h);
    
    this->stretch_z_fu.setBounds(20, 390,
                                 w, h);
    
    this->stretch_amount.setBounds(20, 460,
                                   w, h);
    
    this->zIndex.setBounds  (20, 530,
                                   w, h);

}
// ==================================================

void ISVParameterControlUI::sliderUIValueChanged(sliderUI1* obj)
{
    this->status = SliderValueChanged;
    sendChangeMessage();
}

// ==================================================


