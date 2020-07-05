//
//  IRVideoAnnotaterObjectManagement.cpp
//  IRiMaS
//
//  Created by Keitaro on 01/03/2020.
//

#include "IRVideoAnnotater.hpp"

void IRVideoAnnotater::duplicateNodeObject(IRNodeObject* obj, bool addEventList)
{
    String name = obj->name;
    
    if (name == "IRTextEditor")
    {
        createTextEventComponentFromIRNodeObject(obj, addEventList);
    }else if(name == "IRShape")
    {
        createShapeEventComponentFromNodeObject(obj, addEventList);
    }else if(name == "IRImageViewer")
    {
        createImageEventComponentFromNodeObject(obj, addEventList);
    }
}

void IRVideoAnnotater::setupEventComponent(IRNodeObject* obj, VideoAnnotationEventComponent* event)
{
    // close menu

    if(this->annotationMenu.get() != nullptr)
        this->annotationMenu->closeAction();
    event->setNodeObject(obj);

    // add event to eventList
    this->eventListComponent->addEventComponent(event);
    
}


void IRVideoAnnotater::createEventComponent(IRNodeObject* obj, bool addEventList)
{
    auto n = obj->name;
    
    if(n == "IRTextEditor")
    {
        createTextEventComponentFromIRNodeObject(obj, addEventList);
    }else if(n == "IRShape")
    {
        createShapeEventComponentFromNodeObject(obj, addEventList);
    }else if(n == "IRImageViewer")
    {
        createImageEventComponentFromNodeObject(obj, addEventList);
    }else{
        KLib().showErrorMessage("Error : IRVideoAnnotater::createEventComponent : unknown object name " + n);
    }
}

void IRVideoAnnotater::createTextEventComponent()
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float beginTimeInSec = this->videoTransport->getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;
    
    auto event = new AnnotationTextEventComponent(this->ir_str.get(),
                                                  this,
                                                  beginTimeInSec,
                                                  endTimeInSec);
    
    
    // ask myVideoPlayerObject to create TextObject
    auto obj = this->myVideoPlayerObject->createTextObject(event);
    
    obj->setStartTimeSec(beginTimeInSec);
    obj->setEndTimeSec(endTimeInSec);

    setupEventComponent(obj, event);

}

void IRVideoAnnotater::createTextEventComponentFromSRT(SubtitleItem* item)
{
    std::string startTime = item->getStartTimeString();
    std::string endTime = item->getEndTimeString();
    std::string text = item->getText();
    std::string err;
    json11::Json saveData = json11::Json::parse(text, err);
    std::string contents = saveData["text"].string_value();
    
    std::cout << "AnnotationTextEventComponent : " << startTime << " , " << endTime << " : " << contents << std::endl;

    auto event = new AnnotationTextEventComponent(this->ir_str.get(),
                                                  this,
                                                  startTime,
                                                  endTime,
                                                  contents
                                                  );
    
    IRVATextEditorObject* obj = static_cast<IRVATextEditorObject*>(IRObjectCreater<IRVATextEditorObject>().create(this, this->ir_str.get()));
    
    obj->setStartTimeSec(event->getBeginTimeCode());
    obj->setEndTimeSec(event->getEndTimeCode());
 
    setupEventComponent(obj, event);

}


void IRVideoAnnotater::createTextEventComponentFromIRNodeObject(IRNodeObject* obj, bool addEventList)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    //float beginTimeInSec = this->videoTransport->getPlayPosition();
    //float endTimeInSec = beginTimeInSec + 3.0;

    float beginTimeInSec = obj->getStartTimeSec();
    float endTimeInSec = obj->getEndTimeSec();

    auto event = new AnnotationTextEventComponent(this->ir_str.get(),
                                                  this,
                                                  beginTimeInSec,
                                                  endTimeInSec);
   
    setupEventComponent(obj, event);
    
    obj->setEventComponent(event);

}


void IRVideoAnnotater::createShapeEventComponent()
{
    
    float beginTimeInSec = this->videoTransport->getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;
    std::cout << "createShapeEventComponent begin = " << beginTimeInSec << " , " << endTimeInSec << std::endl;

    auto event = new AnnotationShapeEventComponent(this->ir_str.get(),
                                                   this,
                                                   beginTimeInSec,
                                                   endTimeInSec);

    auto obj = this->myVideoPlayerObject->createShapeObject(event);
    obj->setStartTimeSec(beginTimeInSec);
    obj->setEndTimeSec(endTimeInSec);
    
    setupEventComponent(obj, event);


}

void IRVideoAnnotater::createShapeEventComponentFromNodeObject(IRNodeObject* obj, bool addEventList)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float beginTimeInSec = obj->getStartTimeSec();
    float endTimeInSec = obj->getEndTimeSec();
    

    auto event = new AnnotationShapeEventComponent(this->ir_str.get(),
                                                   this,
                                                   beginTimeInSec,
                                                   endTimeInSec);
    
    setupEventComponent(obj, event);
    
    obj->setEventComponent(event);


}

void IRVideoAnnotater::createImageEventComponent()
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float beginTimeInSec = this->videoTransport->getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;

    auto event = new AnnotationImageEventComponent(this->ir_str.get(),
                                                   this,
                                                   beginTimeInSec,
                                                   endTimeInSec);
    

    auto obj = this->myVideoPlayerObject->createImageObject(event);
    obj->setStartTimeSec(beginTimeInSec);
    obj->setEndTimeSec(endTimeInSec);
    setupEventComponent(obj, event);


}

void IRVideoAnnotater::createImageEventComponentFromNodeObject(IRNodeObject* obj, bool addEventList)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    float beginTimeInSec = obj->getStartTimeSec();
    float endTimeInSec = obj->getEndTimeSec();
    
    auto event = new AnnotationImageEventComponent(this->ir_str.get(),
                                                   this,
                                                   beginTimeInSec,
                                                   endTimeInSec);
   
    setupEventComponent(obj, event);
    
    obj->setEventComponent(event);


}

void IRVideoAnnotater::createAudioEventComponent()
{
    float beginTimeInSec = this->videoTransport->getPlayPosition();
    float endTimeInSec = beginTimeInSec + 3.0;
   
    this->annotationMenu->closeAction();

}

void IRVideoAnnotater::createAudioEventComponentFromNodeObject(IRNodeObject* obj, bool addEventList)
{
    float beginTimeInSec = obj->getStartTimeSec();
    float endTimeInSec = obj->getEndTimeSec();

    this->annotationMenu->closeAction();

}



// ================================================================================

// LOAD SRTs






// ================================================================================
