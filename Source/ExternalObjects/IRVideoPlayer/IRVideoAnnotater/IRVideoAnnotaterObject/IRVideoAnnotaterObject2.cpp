//
//  IRVideoAnnotaterObject2.cpp
//  IRiMaS
//
//  Created by Keitaro on 10/05/2020.
//

#include "IRVideoAnnotaterObject2.hpp"


IRVideoAnnotaterObject2::IRVideoAnnotaterObject2(Component* parent, IRStr* str, bool withOpenButton) :
IRNodeObject(parent, "IRVideoAnnotater2", str, NodeObjectType(ordinaryIRComponent))

{
    
    this->workspace.reset(new IRVideoAnnotaterWorkspace("Video Annotater", Rectangle<int>(0, 0, 0, 0), str));
    this->workspace->setDraggableMargin(Rectangle<int>(0, 0, 0, 0));
    addAndMakeVisible(this->workspace.get());
    //this->annotaterWorkspace->setBackgroundColour(Colours::transparentBlack);
    //this->annotaterWorkspace->addListener(this);
    //this->annotaterWorkspace->addKeyListener(this);
    
    setObjectSize(300, 200);
    
    
    //createTextObject();
}

IRVideoAnnotaterObject2::~IRVideoAnnotaterObject2()
{
    this->workspace.reset();
}

void IRVideoAnnotaterObject2::resized()
{
    std::cout << "videoANnoter2 resized\n";
    this->workspace->setBounds(getLocalBounds());
    
}


void IRVideoAnnotaterObject2::paint(Graphics& g) 
{
    //g.fillAll(Colours::yellow);
}

// --------------------------------------------------
void IRVideoAnnotaterObject2::createNodeObjectOnWorkspace(IRNodeObject* obj)
{
    obj->setObjectBounds(this->workspace->getWidth()/2,
                             this->workspace->getHeight()/2,
                             this->workspace->getWidth()/4,
                             60);
    this->workspace->createObject(obj);
    obj->bringThisToFront();
    this->workspace->deselectAllObjects();
    //obj->setEventComponent(event);
    //event->setNodeObject(obj);
    obj->setSelected(true);
    // initially the object is shown
    obj->setAnimated(true);
    //obj->setCurrentTimeCode(this->videoTransport.getPlayPosition());
}
void IRVideoAnnotaterObject2::createTextObject()
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

    
    IRVATextEditorObject* nodeObj = static_cast<IRVATextEditorObject*>(IRObjectCreater<IRVATextEditorObject>().create(this->workspace.get(),
                                                                getStr()));
    createNodeObjectOnWorkspace(nodeObj);

}
// --------------------------------------------------

void IRVideoAnnotaterObject2::videoLoadCompletedCallback()
{
    std::cout << "IRVideoAnnotaterObject2::videoLoadCompletedCallback\n";

    resized();

}


// --------------------------------------------------
// define if call videoLoadCompletedCallbackFunc();
void IRVideoAnnotaterObject2::openFile(File file, bool isCallback)
{
    
}
void IRVideoAnnotaterObject2::openFile(bool isCallbback)
{
    
}

// --------------------------------------------------
void IRVideoAnnotaterObject2::play()
{
    
}
void IRVideoAnnotaterObject2::stop()
{
    
}
void IRVideoAnnotaterObject2::setPlayPosition(double newPositionInSec)
{
    
}

juce::Point<int> IRVideoAnnotaterObject2::getVideoSize()
{
    
}
// --------------------------------------------------

// --------------------------------------------------
void IRVideoAnnotaterObject2::enableController(bool flag)
{
    
}
// --------------------------------------------------
IRVideoPlayerObject2* IRVideoAnnotaterObject2::getVideoPlayerObject()
{
    return this->workspace->getVideoPlayerObject();
}

// --------------------------------------------------

void IRVideoAnnotaterObject2::createAnnotationComponent(VideoAnnotationEventComponent* event)
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

void IRVideoAnnotaterObject2::createTEXTAnnotationComponent(VideoAnnotationEventComponent* event)
{

}
