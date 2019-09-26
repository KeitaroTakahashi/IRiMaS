//
//  AudioMenuComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 27/08/2019.
//

#include "AudioMenuComponent.hpp"

AudioMenuComponent::AudioMenuComponent(IRStr* str, Rectangle<int> frameRect) : ObjectMenuComponent(str, frameRect)
{
    setTitleText("Audio");
    auto objects = getStr()->OBJECTFACTORY.getRegisteredObjectOfCategory(objectCategory::AUDIOMENU);
    
    for(auto o : objects)
    {
        std::cout << o->name << std::endl;
        createItem(o);
    }
}

AudioMenuComponent::~AudioMenuComponent()
{
    
}

void AudioMenuComponent::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
}

void AudioMenuComponent::resized()
{
    ObjectMenuComponent::resized();
}
// ==================================================

void AudioMenuComponent::itemSelected(IRObjectFactory2::t_object* obj)
{
    
}

void AudioMenuComponent::itemReleased(IRObjectFactory2::t_object* obj)
{
    
    
    auto topSpace = static_cast<IRWorkspace* >(getStr()->TopWorkspace);
    
    auto nodeObj = getStr()->OBJECTFACTORY.createObject(obj->id, topSpace);
    nodeObj->setCentrePosition(topSpace->getWidth()/2,
                               topSpace->getHeight()/2);
    topSpace->createObject(nodeObj);
    
    
}

