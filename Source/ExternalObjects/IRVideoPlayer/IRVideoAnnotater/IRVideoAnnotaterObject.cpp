//
//  IRVideoAnnotaterObject.cpp
//  IRiMaS
//
//  Created by Keitaro on 13/12/2019.
//

#include "IRVideoAnnotaterObject.hpp"

IRVideoAnnotaterObject::IRVideoAnnotaterObject(Component* parent, IRStr* str) :
IRVideoPlayerObject(parent, str)
{
    this->controller.reset( new IRVideoPlayerController(str, this) );
    this->controller->addChangeListener(this);
    setObjController(this->controller.get());
    
    std::cout << "IRVideoAnnotaterObject init\n";

}

IRVideoAnnotaterObject::~IRVideoAnnotaterObject()
{
    this->controller.reset();
}
// --------------------------------------------------

void IRVideoAnnotaterObject::videoLoadCompletedCallback()
{
    this->controller->updateAnnotater();
}
// --------------------------------------------------

void IRVideoAnnotaterObject::annotaterOpenedAction()
{
    
}
// --------------------------------------------------

void IRVideoAnnotaterObject::annotaterClosedAction()
{
    //this->parent->addAndMakeVisible(this);
    //this->setBounds(getBoundsInWorkspace());
    
    std::cout << "annotaterClosedAction\n";
}

// --------------------------------------------------


void IRVideoAnnotaterObject::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == this->controller.get())
    {
        using uiStatus = VideoController::VideoControllerStatus;
        
        switch(this->controller->getStatus())
        {
            case uiStatus::OpenVideoFile:
                this->videoPlayer->openFile();
                break;
            case uiStatus::OpenVideoAnnotater:
                annotaterOpenedAction();
                break;
            case uiStatus::CloseVideoAnnotater:
                annotaterClosedAction();
                break;
            default:
                break;
        }
    }
}
// --------------------------------------------------

// --------------------------------------------------
// --------------------------------------------------
