//
//  IRVideoAnnotaterObjectManagement.cpp
//  IRiMaS
//
//  Created by Keitaro on 01/03/2020.
//

#include "IRVideoAnnotater.hpp"

void IRVideoAnnotater::createNodeObjectOnEvent(IRNodeObject* obj,
                                               VideoAnnotationEventComponent* event)
{
    obj->setObjectBounds(this->workspace->getWidth()/2,
                             this->workspace->getHeight()/2,
                             this->workspace->getWidth()/4,
                             60);
    this->workspace->createObject(obj);
    obj->bringThisToFront();
    this->workspace->deselectAllObjects();
    obj->setEventComponent(event);
    event->setNodeObject(obj);
    obj->setSelected(true);
    // initially the object is shown
    obj->setAnimated(true);
    obj->setCurrentTimeCode(this->videoTransport.getPlayPosition());
}


void IRVideoAnnotater::createTextEventComponent()
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float betinTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = betinTimeInSec + 3.0;

    auto event = this->eventListComponent->createTextEventComponent(betinTimeInSec,
                                                                    endTimeInSec);
    this->annotationMenu->closeAction();
    createEventOnTheLoadedVideo(event);

    
    IRVATextEditorObject* nodeObj = static_cast<IRVATextEditorObject*>(IRObjectCreater<IRVATextEditorObject>().create(this->workspace.get(),
                                                                getStr()));
    createNodeObjectOnEvent(nodeObj, event);

}

void IRVideoAnnotater::createTextEventComponentFromIRNodeObject(IRNodeObject* obj)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float betinTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = betinTimeInSec + 3.0;

    auto event = this->eventListComponent->createTextEventComponent(betinTimeInSec,
                                                                    endTimeInSec);
   
    createEventOnTheLoadedVideo(event);
    obj->setEventComponent(event);
    event->setNodeObject(obj);
    obj->setSelected(true);
    obj->setAnimated(true);
    obj->setCurrentTimeCode(this->videoTransport.getPlayPosition());

}


void IRVideoAnnotater::createShapeEventComponent()
{
    
    float betinTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = betinTimeInSec + 3.0;
    
    auto event = this->eventListComponent->createShapeEventComponent(betinTimeInSec,
                                                            endTimeInSec);
    this->annotationMenu->closeAction();
    createEventOnTheLoadedVideo(event);

    IRVAShapeObject* nodeObj = static_cast<IRVAShapeObject*>(IRObjectCreater<IRVAShapeObject>().create(this->workspace.get(),
                                                                getStr()));
    createNodeObjectOnEvent(nodeObj, event);
}

void IRVideoAnnotater::createShapeEventComponentFromNodeObject(IRNodeObject* obj)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float betinTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = betinTimeInSec + 3.0;

    auto event = this->eventListComponent->createShapeEventComponent(betinTimeInSec,
                                                                     endTimeInSec);
    
    createEventOnTheLoadedVideo(event);
    obj->setEventComponent(event);
    event->setNodeObject(obj);
    this->workspace->deselectAllObjects();
    obj->setSelected(true);
    obj->setAnimated(true);
    obj->setCurrentTimeCode(this->videoTransport.getPlayPosition());

}

void IRVideoAnnotater::createImageEventComponent()
{
    
}

void IRVideoAnnotater::createAudioEventComponent()
{
    
}
