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
    
    
    auto b = this->myVideoPlayerObject->getBounds();
    obj->setObjectBounds(b.getWidth() / 2,
                         b.getHeight() / 2,
                         b.getWidth() / 4,
                         60);
    
    obj->bringThisToFront();
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
    float beginTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;

    auto event = this->eventListComponent->createTextEventComponent(beginTimeInSec,
                                                                    endTimeInSec);
    this->annotationMenu->closeAction();
    createEventOnTheLoadedVideo(event);

    /*
    IRVATextEditorObject* nodeObj = static_cast<IRVATextEditorObject*>(IRObjectCreater<IRVATextEditorObject>().create(this->workspace.get(),
                                                                getStr()));
    createNodeObjectOnEvent(nodeObj, event);
    */
    this->myVideoPlayerObject->createTextObject(event);
    


}

void IRVideoAnnotater::createTextEventComponentFromIRNodeObject(IRNodeObject* obj)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float beginTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;

    auto event = this->eventListComponent->createTextEventComponent(beginTimeInSec,
                                                                    endTimeInSec);
   
    createEventOnTheLoadedVideo(event);
    obj->setEventComponent(event);
    event->setNodeObject(obj);
    obj->setSelected(true);
    obj->setAnimated(true);
    obj->setCurrentTimeCode(this->videoTransport.getPlayPosition());

    
    this->myVideoPlayerObject->createTextObject(event);

}


void IRVideoAnnotater::createShapeEventComponent()
{
    
    float beginTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;
    
    auto event = this->eventListComponent->createShapeEventComponent(beginTimeInSec,
                                                            endTimeInSec);
    this->annotationMenu->closeAction();
    createEventOnTheLoadedVideo(event);

    IRVAShapeObject* nodeObj = static_cast<IRVAShapeObject*>(IRObjectCreater<IRVAShapeObject>().create(this->workspace.get(),
                                                                getStr()));
    createNodeObjectOnEvent(nodeObj, event);
    
    this->myVideoPlayerObject->createShapeObject(event);

}

void IRVideoAnnotater::createShapeEventComponentFromNodeObject(IRNodeObject* obj)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float beginTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;

    auto event = this->eventListComponent->createShapeEventComponent(beginTimeInSec,
                                                                     endTimeInSec);
    
    createEventOnTheLoadedVideo(event);
    obj->setEventComponent(event);
    event->setNodeObject(obj);
    this->workspace->deselectAllObjects();
    obj->setSelected(true);
    obj->setAnimated(true);
    obj->setCurrentTimeCode(this->videoTransport.getPlayPosition());
    this->myVideoPlayerObject->createShapeObject(event);

}

void IRVideoAnnotater::createImageEventComponent()
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float beginTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;

    auto event = this->eventListComponent->createImageEventComponent(beginTimeInSec,
                                                                     endTimeInSec);
    this->annotationMenu->closeAction();
    createEventOnTheLoadedVideo(event);

    
    IRVAImageViewerObject* nodeObj = static_cast<IRVAImageViewerObject*>(IRObjectCreater<IRVAImageViewerObject>().create(this->workspace.get(),
                                                                getStr()));
    createNodeObjectOnEvent(nodeObj, event);
    
    this->myVideoPlayerObject->createImageObject(event);

}

void IRVideoAnnotater::createImageEventComponentFromNodeObject(IRNodeObject* obj)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float beginTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;

    auto event = this->eventListComponent->createImageEventComponent(beginTimeInSec,
                                                                     endTimeInSec);
   
    createEventOnTheLoadedVideo(event);
    obj->setEventComponent(event);
    event->setNodeObject(obj);
    obj->setSelected(true);
    obj->setAnimated(true);
    obj->setCurrentTimeCode(this->videoTransport.getPlayPosition());
    this->myVideoPlayerObject->createImageObject(event);

}

void IRVideoAnnotater::createAudioEventComponent()
{
    float beginTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;

    auto event = this->eventListComponent->createWaveformEventComponent(beginTimeInSec,
                                                                     endTimeInSec);
    this->annotationMenu->closeAction();
    createEventOnTheLoadedVideo(event);

    
    IRVAWaveformObject* nodeObj = static_cast<IRVAWaveformObject*>(IRObjectCreater<IRVAWaveformObject>().create(this->workspace.get(),
                                                                getStr()));
    createNodeObjectOnEvent(nodeObj, event);
}

void IRVideoAnnotater::createAudioEventComponentFromNodeObject(IRNodeObject* obj)
{
    float beginTimeInSec = this->videoTransport.getPlayPosition();
     float endTimeInSec = beginTimeInSec + 3.0;

     auto event = this->eventListComponent->createWaveformEventComponent(beginTimeInSec,
                                                                         endTimeInSec);
    
     createEventOnTheLoadedVideo(event);
     obj->setEventComponent(event);
     event->setNodeObject(obj);
     obj->setSelected(true);
     obj->setAnimated(true);
     obj->setCurrentTimeCode(this->videoTransport.getPlayPosition());
}
