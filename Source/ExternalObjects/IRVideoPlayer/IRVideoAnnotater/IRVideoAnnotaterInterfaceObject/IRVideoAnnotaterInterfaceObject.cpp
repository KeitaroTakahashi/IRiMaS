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

// --------------------------------------------------

IRNodeObject* IRVideoAnnotaterInterfaceObject::copyThis()
{
    return new IRVideoAnnotaterInterfaceObject(this->parent, getStr());
}
// --------------------------------------------------
IRNodeObject* IRVideoAnnotaterInterfaceObject::copyContents(IRNodeObject* object)
{
    IRVideoAnnotaterInterfaceObject* obj = static_cast<IRVideoAnnotaterInterfaceObject*>(object);
    obj->setBounds(getLocalBounds());
    File movieFile = getVideoPlayer()->getMovieFile();
    if(movieFile.exists())
    {
        obj->openFile(movieFile, false);
    }
    
    return obj;
}
// --------------------------------------------------
IRNodeObject* IRVideoAnnotaterInterfaceObject::copyDragDropContents(IRNodeObject* object)
{
    IRVideoAnnotaterInterfaceObject* obj = new IRVideoAnnotaterInterfaceObject(this->parent, getStr());
    return obj;
}
// --------------------------------------------------
// --------------------------------------------------
// SAVE LOAD
t_json IRVideoAnnotaterInterfaceObject::saveThisToSaveData()
{
    using t = VideoAnnotationEventComponent::VideoAnnotationType;
    
    IRVideoAnnotater* videoAnnotator = this->controller->getVideoAnnotaterComponent();
    
    std::string srtFilePath = videoAnnotator->getSRTFilePath();
    
    std::cout << "videoFilePath = " << this->videoPlayer->getPath() << std::endl;
    std::cout << "srtFilePath = " << srtFilePath << std::endl;
    
    t_json saveData = t_json::object({
        {"filePath", this->videoPlayer->getPath()},
        {"srtPath",  srtFilePath}
    });
    
    t_json save = t_json::object({
        {"videoPlayer", saveData}
    });
    
    return save;
}
// --------------------------------------------------
void IRVideoAnnotaterInterfaceObject::loadThisFromSaveData(t_json data)
{
    t_json w = data["videoPlayer"];
    
    File file(w["filePath"].string_value());
    this->videoPlayer->openFile(file);
    std::string srtPath = w["srtPath"].string_value();
    if(srtPath.size() > 0)
    {
        IRVideoAnnotater* videoAnnotator = this->controller->getVideoAnnotaterComponent();
        videoAnnotator->openSRTs(File(srtPath));
    
        std::cout << "srtPath = " << srtPath << std::endl;
    }
}
// --------------------------------------------------

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
