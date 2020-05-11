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
    
    
    createTextObject();
}

IRVideoAnnotaterObject2::~IRVideoAnnotaterObject2()
{
    this->workspace.reset();
}

void IRVideoAnnotaterObject2::resized()
{
    std::cout << "videoANnoter2 resized\n";
    this->workspace->setBounds(getLocalBounds());
   //this->annotaterWorkspace->setBounds(0, 0, 200, 200);
    
    

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
