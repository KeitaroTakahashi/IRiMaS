//
//  IRVideoAnnotaterInterfaceObject.cpp
//  IRiMaS
//
//  Created by Keitaro on 19/01/2020.
//

#include "IRVideoAnnotaterInterfaceObject.hpp"

IRVideoAnnotaterInterfaceObject::IRVideoAnnotaterInterfaceObject(Component* parent, IRStr* str) :
IRVideoAnnotaterObject(parent, str)
{
   this->controller.reset( new IRVideoPlayerController(str, this) );
   this->controller->addChangeListener(this);
   setObjController(this->controller.get());

}

IRVideoAnnotaterInterfaceObject::~IRVideoAnnotaterInterfaceObject()
{
    this->controller.reset();
}

void IRVideoAnnotaterInterfaceObject::videoLoadCompletedCallback()
{
    this->controller->updateAnnotater();
}
// --------------------------------------------------

void IRVideoAnnotaterInterfaceObject::annotaterOpenedAction()
{
    
}
// --------------------------------------------------

void IRVideoAnnotaterInterfaceObject::annotaterClosedAction()
{
    //this->parent->addAndMakeVisible(this);
    //this->setBounds(getBoundsInWorkspace());
    
    std::cout << "annotaterClosedAction\n";
}

// --------------------------------------------------

void IRVideoAnnotaterInterfaceObject::changeListenerCallback (ChangeBroadcaster* source)
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
