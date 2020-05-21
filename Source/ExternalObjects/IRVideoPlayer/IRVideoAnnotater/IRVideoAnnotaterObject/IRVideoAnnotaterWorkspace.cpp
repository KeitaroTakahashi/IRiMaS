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
    std::cout << "print\n";
    
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
void IRVideoAnnotaterWorkspace::videoLoadCompletedAction()
{
    
    manageHeavyWeightComponents(isEditMode());
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
    obj->bringThisToFront();
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
    createNodeObjectOnWorkspace(nodeObj);

}
// --------------------------------------------------
void IRVideoAnnotaterWorkspace::videoPlayingUpdateAction(double pos)
{
    //std::cout << "pos = " << pos << std::endl;
    // first reset the list of the visible annotation components
/*
    using t = VideoAnnotationEventComponent::VideoAnnotationType;
    for(auto obj : getObjects())
    {
        auto event = obj->getEventComponent();
        
        
        if(pos >= obj->getBeginTimeInSec() && obj->getEndTimeInSec() > pos)
        {
            // check if the event is active
            if(obj->getEvent()->isActive())
            {
                // add to the list
                addVisibleAnnotationComponents(obj);
                //highlight
                obj->getEvent()->setSelected(true);
                obj->getEvent()->selectedAction(); // to opearate any following actions by selecting
                // set visible true
                obj->setVisible(true);
            }
        }else{
            
            if(obj->isVisible())
            {
                // if the component is visible, then remove from the visible list and set it invisible
                removeVisibleAnnotationComponents(obj);
                obj->getEvent()->setSelected(false);
                obj->setVisible(false);
            }
        }
    }
            
            if(this->videoPlayingUpdate != nullptr)
                this->videoPlayingUpdate(pos);
            
            resized();
    
    */
    
    
    if(this->videoPlayingUpdateCallback != nullptr)
        this->videoPlayingUpdateCallback(pos);
}
