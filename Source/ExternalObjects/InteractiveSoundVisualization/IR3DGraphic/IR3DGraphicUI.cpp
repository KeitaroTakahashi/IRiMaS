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
    
    vertexSetup();
}

IR3DGraphicUI::~IR3DGraphicUI()
{
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
    this->vertex.setBounds(0, 0, getWidth(), getHeight());
}

void IR3DGraphicUI::vertexSetup()
{
   
    this->vertex.setBounds(0, 0, getWidth(), getHeight());
    addAndMakeVisible(&this->vertex);
    
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
    this->vertex.startRendering();
}
void IR3DGraphicUI::stopRendering()
{
    this->vertex.stopRendering();
}
