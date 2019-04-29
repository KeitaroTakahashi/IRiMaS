//
//  IRIcographUI.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#include "IRIcographUI.hpp"

IRIcographUI::IRIcographUI(IRNodeObject* parent) : IRUIFoundation(parent)
{
    setSize (1200, 900);
    setFps(33);
    startAnimation();
    
    int w = getWidth() * 0.9;
    int h = getHeight() * 0.9;
    
    int x = getWidth() / 20;
    int y = getHeight() / 20;
    
    vertexSetup();
}

IRIcographUI::~IRIcographUI()
{
    stopAnimation();
}

//==============================================================================
void IRIcographUI::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void IRIcographUI::resized()
{

}

void IRIcographUI::vertexSetup()
{
    int w = getWidth() * 0.95;
    int h = 600;
    
    int x = getWidth() / 40;
    int y = 20;
    
    this->vertex = new OpenGLVertexNoise();
    this->vertex->setBounds(x, y, w, h);
    addAndMakeVisible(this->vertex);
    
}

/*

void IRIcographUI::sliderValueChanged (Slider* slider)
{
    if(slider == &this->offsetX)
    {
        if(this->vertex != nullptr)
            this->vertex->setOffsetX((float)this->offsetX.getValue());
    }
    else if (slider == &this->offsetY)
    {
        if(this->vertex != nullptr)
            this->vertex->setOffsetY((float)this->offsetY.getValue());
    }
    else if (slider == &this->offsetZ)
    {
        if(this->vertex != nullptr)
            this->vertex->setOffsetZ((float)this->offsetZ.getValue());
    }
    else if (slider == &this->fineness)
    {
        if(this->vertex != nullptr)
            this->vertex->setFineness((float)this->fineness.getValue());
    }
    else if (slider == &this->intensity)
    {
        if(this->vertex != nullptr)
            this->vertex->setIntensity((float)this->intensity.getValue());
    }
}
*/
void IRIcographUI::updateAnimationFrame()
{
    
    /*
    this->offsetX.setValue(this->vertex->getOffsetX());
    this->offsetY.setValue(this->vertex->getOffsetY());
    this->offsetZ.setValue(this->vertex->getOffsetZ());
    
    
    
    if(this->fftPlayer != nullptr)
    {
        this->amplitude = this->fftPlayer->getAmplitude();
        this->centroid = this->fftPlayer->getCentroid();
        this->intensity_smoother.setValue(this->amplitude * this->intensityInteraction.getValue());
        this->vertex->setIntensity(this->intensity_smoother.getValue());
        
        this->intensity.setValue(this->intensity_smoother.getValue());
        

        
        this->intensity_smoother.animation();
        this->fineness_smoother.animation();
    }
 */
}

void IRIcographUI::OffsetXChanged(float x)
{
}

void IRIcographUI::OffsetYChanged(float y)
{
}

void IRIcographUI::OffsetZChanged(float z)
{
}
