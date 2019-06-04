//
//  IR3DGraphicUI.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#include "IR3DGraphicUI.hpp"

IR3DGraphicUI::IR3DGraphicUI(IRNodeObject* parent) : IRUIFoundation(parent)
{
    setSize(600, 400);
    setFps(33);
    startAnimation();
    
    vertexSetup();
}

IR3DGraphicUI::~IR3DGraphicUI()
{
    stopAnimation();
}

//==============================================================================
void IR3DGraphicUI::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void IR3DGraphicUI::resized()
{
    if(this->vertex.get() != nullptr)
        this->vertex->setBounds(0, 0, getWidth(), getHeight());
}

void IR3DGraphicUI::vertexSetup()
{
   
    this->vertex = std::make_shared<OpenGLVertexNoise>();
    this->vertex->setBounds(0, 0, getWidth(), getHeight());
    addAndMakeVisible(this->vertex.get());
    
}

/*

void IR3DGraphicUI::sliderValueChanged (Slider* slider)
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
void IR3DGraphicUI::updateAnimationFrame()
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

void IR3DGraphicUI::OffsetXChanged(float x)
{
}

void IR3DGraphicUI::OffsetYChanged(float y)
{
}

void IR3DGraphicUI::OffsetZChanged(float z)
{
}


// ==================================================
void IR3DGraphicUI::startRendering()
{
    this->vertex->startRendering();
}
void IR3DGraphicUI::stopRendering()
{
    this->vertex->stopRendering();
}
