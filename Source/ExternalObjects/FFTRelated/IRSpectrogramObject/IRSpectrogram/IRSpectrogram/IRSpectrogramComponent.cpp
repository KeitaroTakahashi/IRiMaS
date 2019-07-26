//
//  IRSpectrogramComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 25/07/2019.
//

#include "IRSpectrogramComponent.hpp"

IRSpectrogramComponent::IRSpectrogramComponent(IRNodeObject* nodeObject)
{
    this->spectrogram = std::make_shared<IRSpectrogram>(nodeObject);
    addAndMakeVisible(this->spectrogram.get());
}

IRSpectrogramComponent::~IRSpectrogramComponent()
{
    
}

void IRSpectrogramComponent::resized()
{
    std::cout << "IRSpectrogramComponent resized " << getWidth() << std::endl;
    
    this->spectrogram->parentSizeChanged(getWidth(), getHeight());
}

void IRSpectrogramComponent::paint(Graphics& g)
{
    g.fillAll(Colours::yellow);
    
    int w = getWidth();
    int h = getHeight();
    
    g.setColour(Colours::lightgrey.brighter().brighter());
    int i = 0;
    
    Path p;
    for(i=0;i<=w;i+=50)
    {
        p.startNewSubPath(i, 0);
        p.lineTo(i, h);
        
        p.startNewSubPath(0, i);
        p.lineTo(w, i);
    }
    p.closeSubPath();
    g.strokePath(p, PathStrokeType(2));
    
    for(i=0;i<=w;i+=10)
    {
        p.startNewSubPath(i, 0);
        p.lineTo(i, h);
        
        p.startNewSubPath(0, i);
        p.lineTo(w, i);
    }
    p.closeSubPath();
    g.strokePath(p, PathStrokeType(1));

    
}





void IRSpectrogramComponent::setMainComponentBounds(Rectangle<int> area)
{
    this->spectrogram->setBounds(area);
}

void IRSpectrogramComponent::setVisibleArea(Rectangle<int> area)
{
    
    //std::cout << "iRSpectrogramComponent : size changed " << getWidth()<< std::endl;
    this->spectrogram->setVisibleArea(area, Point<int>(getWidth(), getHeight()));
    
    
    repaint();
}

