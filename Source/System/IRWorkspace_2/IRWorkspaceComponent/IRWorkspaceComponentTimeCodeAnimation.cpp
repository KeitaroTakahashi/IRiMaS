//
//  IRWorkspaceComponentTimeCodeAnimation.cpp
//  IRiMaS
//
//  Created by Keitaro on 13/03/2020.
//

#include "IRWorkspaceComponent.hpp"

void IRWorkspaceComponent::resetAnimatedObjectList()
{
    for(auto obj : this->currentlyAnimatedObjects)
    {
        obj->setAnimated(false);
    }
    
    this->currentlyAnimatedObjects.clear();
}

void IRWorkspaceComponent::enableTimeCodeAnimation(bool flag)
{
    this->timeCodeAnimationEnable = flag;
}

void IRWorkspaceComponent::setCurrentTimeCode(float currentTime)
{
    this->currentTime = currentTime;
}


void IRWorkspaceComponent::startAnimated(IRNodeObject* obj)
{
    std::cout << obj << " animation starts!\n";
    // Triggered!
    addCurrentlyAnimatedObjectList(obj);
    obj->actionStart();
    obj->setAnimated(true);
}

void IRWorkspaceComponent::endAnimated(IRNodeObject* obj)
{
    std::cout << obj << " animation ends!\n";

    removeFromCurrentlyAnmatedObjectList(obj);
    obj->actionEnd();
    obj->setAnimated(false);
}

void IRWorkspaceComponent::updateCurrentAnimation()
{
    updateCurrentAnimation(this->currentTime);
}

void IRWorkspaceComponent::updateCurrentAnimation(float currentTime)
{
    
    if(this->timeCodeAnimationEnable) return;

    for(auto obj : this->objects)
    {
        if(obj->isAnimationActive())
        {
            std::cout << "updateCurrentAnimation of " << obj << " : animated = " << obj->isAnimated() << std::endl;
            // check if this is the first time that the object is set animation
            if(!obj->isAnimated())
            {
                if(obj->tryTrigerAnimation(currentTime))
                    startAnimated(obj); // START
            }else{
                
                std::cout << obj << " alreasdy animated!\n";
                // check if the object is already set animation and now switched off
                if(!obj->tryTrigerAnimation(currentTime))
                    endAnimated(obj); // END
            }
            
        }else endAnimated(obj); // END
    }
    
    // then update each object animation behaviors
    for(auto obj : this->currentlyAnimatedObjects)
    {
        obj->setCurrentTimeCode(currentTime);
    }
}


void IRWorkspaceComponent::addCurrentlyAnimatedObjectList(IRNodeObject* obj)
{
    // make sure that the object is NOT registered yet
    if(this->currentlyAnimatedObjects.indexOf(obj) == -1)
    {
        this->currentlyAnimatedObjects.add(obj);
        
    }
}
void IRWorkspaceComponent::removeFromCurrentlyAnmatedObjectList(IRNodeObject* obj)
{
    int index = this->currentlyAnimatedObjects.indexOf(obj);
    if(index > -1)
    {
        this->currentlyAnimatedObjects.remove(index);
    }
}
