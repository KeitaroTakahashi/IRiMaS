//
//  IRVideoAnnotaterObject.cpp
//  IRiMaS
//
//  Created by Keitaro on 13/12/2019.
//

#include "IRVideoAnnotaterObject.hpp"

IRVideoAnnotaterObject::IRVideoAnnotaterObject(Component* parent, IRStr* str, bool withOpenButton) :
IRVideoPlayerObject(parent, str, withOpenButton)
{

}

IRVideoAnnotaterObject::~IRVideoAnnotaterObject()
{
    clearVisibleAnnotationComponents();
}
// --------------------------------------------------
void IRVideoAnnotaterObject::resized()
{
    IRVideoPlayerObject::resized();
    
    int visibleIndex = 1;
    
    int videoY = getVideoPlayer()->getY();

    for(auto comp : this->visibleAnnotationComponents)
    {
        //margin x = 5
        comp->setBounds(5, getHeight() - videoY - (60 * visibleIndex), getWidth()-10, 60);
        
        visibleIndex ++;
    }
}


// --------------------------------------------------
void IRVideoAnnotaterObject::videoPlayingUpdateCallback(double pos)
{
    // first reset the list of the visible annotation components
    clearVisibleAnnotationComponents();
    
    using t = VideoAnnotationEventComponent::VideoAnnotationType;

    for(auto comp : this->annotationComponents)
    {
        if(pos >= comp->getBeginTimeInSec() && comp->getEndTimeInSec() > pos)
        {
            // check if the event is active
            if(comp->getEvent()->isActive())
            {
                // add to the list
                addVisibleAnnotationComponents(comp);
                //highlight
                comp->getEvent()->setSelected(true);
                // set visible true
                comp->setVisible(true);
            }
        }else{
            
            if(comp->isVisible())
            {
                // if the component is visible, then remove from the visible list and set it invisible
                removeVisibleAnnotationComponents(comp);
                comp->getEvent()->setSelected(false);
                comp->setVisible(false);
            }
        }
    }
    
    if(this->videoPlayingUpdate != nullptr)
        this->videoPlayingUpdate(pos);
    
    resized();
}
// --------------------------------------------------

// --------------------------------------------------
void IRVideoAnnotaterObject::setAnnotationEvents(std::vector<VideoAnnotationEventComponent*> events)
{
    // reset
    clearAnnotationComponent();
    clearVisibleAnnotationComponents();
    
    for(auto e : events)
    {
        createAnnotationComponent(e);
    }
}
// --------------------------------------------------

void IRVideoAnnotaterObject::clearAnnotationComponent()
{
    for(auto comp : this->annotationComponents)
    {
        delete comp;
    }
    
    this->annotationComponents.clear();
    
}


// --------------------------------------------------

void IRVideoAnnotaterObject::createAnnotationComponent(VideoAnnotationEventComponent* event)
{
    using t = VideoAnnotationEventComponent::VideoAnnotationType;
    switch(event->getType())
    {
        case t::TEXT:
            createTEXTAnnotationComponent(event);
            break;
        case t::SHAPE:
            break;
        case t::IMAGE:
            break;
        case t::AUDIO:
            break;
        default:
            break;
    }
}

// --------------------------------------------------

void IRVideoAnnotaterObject::addAnnotationComponent(IRVideoAnnotationComponent* component)
{
    this->annotationComponents.push_back(component);
}

void IRVideoAnnotaterObject::createTEXTAnnotationComponent(VideoAnnotationEventComponent* event)
{
    auto e = static_cast<AnnotationTextEventComponent* >(event);
    
    IRVideoAnnotationTextComponent* comp = new IRVideoAnnotationTextComponent(getStr(), e);
    
    comp->setIRTextEditorObject(e->getIRTextEditorObject());
    addAndMakeVisible(comp);
    comp->setVisible(false);
    addAnnotationComponent(comp);
    
    resized();
}
// --------------------------------------------------
void IRVideoAnnotaterObject::clearVisibleAnnotationComponents()
{
    this->visibleAnnotationComponents.clear();
}

void IRVideoAnnotaterObject::addVisibleAnnotationComponents(IRVideoAnnotationComponent* visibleComponent)
{
    this->visibleAnnotationComponents.push_back(visibleComponent);
}

void IRVideoAnnotaterObject::removeVisibleAnnotationComponents(IRVideoAnnotationComponent* visibleComponent)
{
    auto it = std::find(this->visibleAnnotationComponents.begin(),
                        this->visibleAnnotationComponents.end(),
                        visibleComponent);
    if (it != this->visibleAnnotationComponents.end())
    {
      this->visibleAnnotationComponents.erase(it);
    }
}

// --------------------------------------------------
// --------------------------------------------------
