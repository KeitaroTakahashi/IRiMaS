//
//  IRVideoAnnotaterObjectManagement.cpp
//  IRiMaS
//
//  Created by Keitaro on 01/03/2020.
//

#include "IRVideoAnnotater.hpp"

void IRVideoAnnotater::createTextEventComponent()
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float betinTimeInSec = this->videoTransport.getPlayPosition();
    float endTimeInSec = betinTimeInSec + 3.0;

    this->eventListComponent->createTextEventComponent(betinTimeInSec,
                                                       endTimeInSec);
    this->annotationMenu->closeAction();
    
    // create eventComponent on the video
    auto events = this->eventListComponent->getEventComponents();
    auto event = events[events.size() - 1];
    createEventOnTheLoadedVideo(event);

    
    IRVATextEditorObject* nodeObj = static_cast<IRVATextEditorObject*>(IRObjectCreater<IRVATextEditorObject>().create(this->workspace.get(),
                                                                getStr()));
    nodeObj->setObjectBounds(this->workspace->getWidth()/2,
                             this->workspace->getHeight()/2,
                             this->workspace->getWidth()/2,
                             60);
    this->workspace->createObject(nodeObj);
    nodeObj->bringThisToFront();
    nodeObj->setSelected(true);
}

void IRVideoAnnotater::createShapeEventComponent()
{
    this->eventListComponent->createShapeEventComponent();
    this->annotationMenu->closeAction();
    
    IRVAShapeObject* nodeObj = static_cast<IRVAShapeObject*>(IRObjectCreater<IRVAShapeObject>().create(this->workspace.get(),
                                                                getStr()));
    nodeObj->setObjectBounds(this->workspace->getWidth()/2,
                             this->workspace->getHeight()/2,
                             this->workspace->getWidth()/2,
                             this->workspace->getHeight()/2);
    this->workspace->createObject(nodeObj);
    nodeObj->bringThisToFront();
    nodeObj->setSelected(true);

}

void IRVideoAnnotater::createImageEventComponent()
{
    
}

void IRVideoAnnotater::createAudioEventComponent()
{
    
}
