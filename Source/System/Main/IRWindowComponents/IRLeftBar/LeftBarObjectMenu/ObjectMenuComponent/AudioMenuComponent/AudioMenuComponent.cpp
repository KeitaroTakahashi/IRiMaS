//
//  AudioMenuComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 27/08/2019.
//

#include "AudioMenuComponent.hpp"

AudioMenuComponent::AudioMenuComponent(Rectangle<int> frameRect) : ObjectMenuComponent(frameRect)
{
    setTitleText("Audio");
}

AudioMenuComponent::~AudioMenuComponent()
{
    
}

void AudioMenuComponent::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.contents);
}

void AudioMenuComponent::resized()
{
    ObjectMenuComponent::resized();
}
