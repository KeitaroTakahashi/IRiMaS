//
//  IRVideoAnnotaterWorkspace.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/05/2020.
//

#include "IRVideoAnnotaterWorkspace.hpp"

IRVideoAnnotaterWorkspace::IRVideoAnnotaterWorkspace(String title, Rectangle<int> draggableMargin, IRStr* str, bool withOpenButton) :
IRNodeObjectWorkspace(title, draggableMargin, str)
{
    enableDrawGrids(true);
    // initially create videoplayer and add
    this->videoPlayerObj.reset( new IRVideoPlayerObject2(this, str, withOpenButton) );
    //createParentObject(this->videoPlayerObj.get(), false);
    addAndMakeVisible(this->videoPlayerObj.get());
    setParentNodeObject(this->videoPlayerObj.get());
    this->videoPlayerObj->addMouseListener(this, true);
    this->videoPlayerObj->addListener(this);
    this->videoPlayerObj->addChangeListener(this);
    this->videoPlayerObj->videoLoadCompletedCallbackFunc = [this] { videoLoadCompletedAction(); };
    this->videoPlayerObj->videoPlayingUpdateCallbackFunc = [this] (double pos) { videoPlayingUpdateAction(pos); };

}

IRVideoAnnotaterWorkspace::~IRVideoAnnotaterWorkspace()
{
    this->videoPlayerObj.reset();
}
// ------------------------------------------------------------------------------------------

void IRVideoAnnotaterWorkspace::onResized()
{
    
    std::cout << "IRVideoAnnotaterWorkspace onResized\n";
    this->videoPlayerObj->setObjectBounds(getLocalBounds());
    
    auto vs = this->videoPlayerObj->getVideoSize();
    std::cout << "getVideoSize = " << vs.getX() << ", " << vs.getY() << std::endl;
    std::cout << "getVideoObjectSize = " << this->videoPlayerObj->getWidth() << " , " << this->videoPlayerObj->getHeight() << std::endl;

}

void IRVideoAnnotaterWorkspace::onPaint(Graphics& g)
{
    g.drawText("IRVideoAnnotaterWorkspace", 0, 0, getWidth(), getHeight()/2, Justification::centred);
    g.fillAll(Colours::blue);
}

void IRVideoAnnotaterWorkspace::setFixObjectSizeRatioWithOriginalSize(bool flag, Rectangle<float> originalSize)
{
    setFixObjectSizeRatio(flag, originalSize);

}
// ------------------------------------------------------------------------------------------


void IRVideoAnnotaterWorkspace::addAnnotationObject(IRNodeObject* obj, Rectangle<int> bounds)
{
    
}
// ------------------------------------------------------------------------------------------

void IRVideoAnnotaterWorkspace::copyAllDataToWorkspace(IRVideoAnnotaterWorkspace* newWorkspace)
{
    auto w = newWorkspace;
        
    auto p = static_cast<IRVideoPlayerObject2*>(this->videoPlayerObj->copyThis());
    std::cout << "IRVideoAnnotaterWorkspace copy VideoPlayerObj url = " << p->getVideoPlayer()->getMovieFile().getFullPathName() << std::endl;
    
    // replace videoPlayerObject
    w->replaceVideoPlayerObject(p);
    
    /*
    for(auto obj : getObjects())
    {
        auto o = obj->copyThis();
        w->createObject(o);
    }*/
}

void IRVideoAnnotaterWorkspace::replaceVideoPlayerObject(IRVideoPlayerObject2* newVideoPlayer)
{
    if(this->videoPlayerObj.get() != nullptr)
    {
        //removeChildComponent(this->videoPlayerObj.get());
    }
    std::cout << "newVideoPlayer = " << newVideoPlayer << std::endl;
    this->videoPlayerObj.reset( newVideoPlayer );
    addAndMakeVisible(this->videoPlayerObj.get());
    setParentNodeObject(this->videoPlayerObj.get());
    this->videoPlayerObj->addMouseListener(this, true);
    this->videoPlayerObj->addListener(this);
    this->videoPlayerObj->addChangeListener(this);
    this->videoPlayerObj->videoLoadCompletedCallbackFunc = [this] { videoLoadCompletedAction(); };
    this->videoPlayerObj->videoPlayingUpdateCallbackFunc = [this] (double pos) { videoPlayingUpdateAction(pos); };
 
    //this->videoPlayerObj->setObjectBounds(0, 0, newVideoPlayer->getWidth(), newVideoPlayer->getHeight());
    //setBounds(newVideoPlayer->getLocalBounds());
    
    manageHeavyWeightComponents(true);
    
    std::cout << "IRVideoAnnotaterWorkspace::replaceVideoPlayerObject bounds = " << getWidth() << ", " << getHeight() << " : " << this->videoPlayerObj->getWidth() << ", " << this->videoPlayerObj->getHeight()<< std::endl;
    
}



// ------------------------------------------------------------------------------------------
void IRVideoAnnotaterWorkspace::videoLoadCompletedAction()
{
    manageHeavyWeightComponents(true);
    if(this->videoLoadCompletedCallback != nullptr)
        this->videoLoadCompletedCallback();
}
// ------------------------------------------------------------------------------------------


IRVideoPlayerObject2* IRVideoAnnotaterWorkspace::getVideoPlayerObject()
{
    if(this->videoPlayerObj.get() != nullptr)
        return this->videoPlayerObj.get();
    else return nullptr;
}

// ------------------------------------------------------------------------------------------

void IRVideoAnnotaterWorkspace::createNodeObjectOnWorkspace(IRNodeObject* obj)
{
    obj->setObjectBounds(getWidth()/2,
                             getHeight()/2,
                             getWidth()/4,
                             60);
    createObject(obj);
    obj->bringToFront();
    deselectAllObjects();
    //obj->setEventComponent(event);
    //event->setNodeObject(obj);
    obj->setSelected(true);
    // initially the object is shown
    obj->setAnimated(true);
    //obj->setCurrentTimeCode(this->videoTransport.getPlayPosition());
}
// ------------------------------------------------------------------------------------------

void IRVideoAnnotaterWorkspace::createTextObject(Component* event)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    //float beginTimeInSec = this->videoTransport.getPlayPosition();
    //float endTimeInSec = beginTimeInSec + 3.0;
/*
    auto event = this->eventListComponent->createTextEventComponent(beginTimeInSec,
                                                                    endTimeInSec);*/
    //this->annotationMenu->closeAction();
    //createEventOnTheLoadedVideo(event);

    
    IRVATextEditorObject* nodeObj = static_cast<IRVATextEditorObject*>(IRObjectCreater<IRVATextEditorObject>().create(this,
                                                                getStr()));
    
    nodeObj->setEventComponent(event);
    createNodeObjectOnWorkspace(nodeObj);

}

void IRVideoAnnotaterWorkspace::createShapeObject(Component* event)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    //float beginTimeInSec = this->videoTransport.getPlayPosition();
    //float endTimeInSec = beginTimeInSec + 3.0;
/*
    auto event = this->eventListComponent->createTextEventComponent(beginTimeInSec,
                                                                    endTimeInSec);*/
    //this->annotationMenu->closeAction();
    //createEventOnTheLoadedVideo(event);

    
    IRVAShapeObject* nodeObj = static_cast<IRVAShapeObject*>(IRObjectCreater<IRVAShapeObject>().create(this,
                                                                getStr()));
    
    nodeObj->setEventComponent(event);
    createNodeObjectOnWorkspace(nodeObj);

}

void IRVideoAnnotaterWorkspace::createImageObject(Component* event)
{
    // automatically fill begin and end time Code
    // end time code in default is begin + 3.0 sec
    //float beginTimeInSec = this->videoTransport.getPlayPosition();
    //float endTimeInSec = beginTimeInSec + 3.0;
/*
    auto event = this->eventListComponent->createTextEventComponent(beginTimeInSec,
                                                                    endTimeInSec);*/
    //this->annotationMenu->closeAction();
    //createEventOnTheLoadedVideo(event);

    
    IRVAImageViewerObject* nodeObj = static_cast<IRVAImageViewerObject*>(IRObjectCreater<IRVAImageViewerObject>().create(this,
                                                                getStr()));
    
    nodeObj->setEventComponent(event);
    createNodeObjectOnWorkspace(nodeObj);

}
// --------------------------------------------------
void IRVideoAnnotaterWorkspace::videoPlayingUpdateAction(double pos)
{
    //  std::cout << "pos = " << pos << std::endl;
    // first reset the list of the visible annotation components

    bool shouldUpdateZOrder = false;
    
    using t = VideoAnnotationEventComponent::VideoAnnotationType;
    for(auto obj : getObjects())
    {
        auto event = static_cast<VideoAnnotationEventComponent*>(obj->getEventComponent());
        
        //std::cout << "event component begin = " << event->getBeginTimeCode() << " : " << event->getEndTimeCode() << std::endl;
        
        if(pos >= event->getBeginTimeCode() && event->getEndTimeCode() > pos)
        {
            //std::cout << "in event activate? " << obj << " : " << obj->isActive() << std::endl;
            if(event->isActive())
            {
                if(!obj->isActive())
                {
                    event->setSelected(true);
                    event->selectedAction();
                    // show object
                    obj->setActive(true);
                    //obj->callReorderZIndex();
                    
                    shouldUpdateZOrder = true;
                    
                }
            }
        }else{
            //std::cout << "out event activate? " << obj << " : " << obj->isActive() << std::endl;
            if(event->isActive())
            {
                if(obj->isActive())
                {
                    event->setSelected(false);
                    // hide object
                    obj->setActive(false);
                    
                    //obj->callReorderZIndex();
                    
                    shouldUpdateZOrder = true;
                }
            }
            
        }

    }

    if(shouldUpdateZOrder)
    {
        manageHeavyWeightComponents(true);
    }
            
    //resized();
      
    if(this->videoPlayingUpdateCallback != nullptr)
        this->videoPlayingUpdateCallback(pos);
}
