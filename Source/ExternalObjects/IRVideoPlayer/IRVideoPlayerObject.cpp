//
//  IRVideoPlayerObject.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/01/2019.
//

#include "IRVideoPlayerObject.hpp"

IRVideoPlayerObject::IRVideoPlayerObject(Component* parent, IRStr* str) :
IRNodeObject(parent, "IRVideoPlayer", str, NodeObjectType(orginaryIRComponent))
{
    
    setOpaque(false);
    
    this->controller.reset( new IRVideoPlayerController(str) );
    this->controller->addChangeListener(this);
    setObjController(this->controller.get());
       
    // original function to give this ChangeListener to controller->UI
    this->controller->addChangeListener(this);
    
    this->videoPlayer = std::make_shared<IRVideoPlayer>(this, str);
    this->videoPlayer->videoLoadCompleted = [this]{ videoLoadCompletedAction(); };
    addAndMakeVisible(this->videoPlayer.get());
    
    setSize(300, 200);
}

IRVideoPlayerObject::~IRVideoPlayerObject()
{
    this->controller.reset();
    this->videoPlayer.reset();

}

IRNodeObject* IRVideoPlayerObject::copyThis()
{
    return new IRVideoPlayerObject(this->parent, getStr());
}
// --------------------------------------------------
IRNodeObject* IRVideoPlayerObject::copyContents(IRNodeObject* object)
{
    IRVideoPlayerObject* obj = static_cast<IRVideoPlayerObject*>(this->parent);
    obj->setBounds(getLocalBounds());
    File movieFile = obj->getVideoPlayer()->getMovieFile();
    obj->getVideoPlayer()->openFile(movieFile);
    
    return obj;
}
// --------------------------------------------------
IRNodeObject* IRVideoPlayerObject::copyDragDropContents(IRNodeObject* object)
{
    IRVideoPlayerObject* obj = new IRVideoPlayerObject(this->parent, getStr());
    return obj;
}
// --------------------------------------------------
t_json IRVideoPlayerObject::saveThisToSaveData()
{
    t_json saveData = t_json::object({
        {"filePath", this->videoPlayer->getPath()}
    });
    
    t_json save = t_json::object({
        {"videoPlayer", saveData}
    });
    
    return save;
}
// --------------------------------------------------
void IRVideoPlayerObject::loadThisFromSaveData(t_json data)
{
    t_json w = data["videoPlayer"];
    
    File file(w["filePath"].string_value());
    this->videoPlayer->openFile(file);
}
// --------------------------------------------------
void IRVideoPlayerObject::resized()
{
   this->videoPlayer->setBounds(getLocalBounds().reduced(5));
}
// --------------------------------------------------
void IRVideoPlayerObject::resizeThisComponentEvent(const MouseEvent& e)
{
    // turn off controller otherwise mouse event will be stolen by the controller,
    // and resize event can not be acomplished properly.
    if(this->videoPlayer->isNeedController() && this->videoPlayer->isVideoOpened())
        this->videoPlayer->setNeedController(false);
    
    double ratio = this->videoPlayer->getAspectRatio();
    if(ratio != 0){
        
        float deltaX = e.getScreenX() - e.getMouseDownScreenX();
        float deltaY = e.getScreenY() - e.getMouseDownScreenY();
        
        float newWidth = getPreviousWidth();
        float newHeight = getPreviousHeight();
        
        if(deltaX > deltaY)
        {
            newWidth += deltaX;
            newHeight = (double) newWidth / this->videoPlayer->getAspectRatio();
        }else{
            newHeight += deltaY;
            newWidth = (double) newHeight * this->videoPlayer->getAspectRatio();
        }
        setSize(newWidth, newHeight);

    }else{
        IRNodeComponent::resizeThisComponentEvent(e);
    }
    
    this->resizing = true;
}
// --------------------------------------------------
void IRVideoPlayerObject::mouseUpEvent(const MouseEvent& e)
{
    //recover event
    if(!this->videoPlayer->isNeedController() && this->videoPlayer->isVideoOpened())
        this->videoPlayer->setNeedController(true);
    
    if(this->resizing)
    {
        callHeavyComponentCreated(this);
        this->resizing = false;
    }
    
}
// --------------------------------------------------
void IRVideoPlayerObject::paint(Graphics& g)
{
    IRNodeObject::paint(g);
    
    auto area = getLocalBounds();
    g.fillAll(getStr()->SYSTEMCOLOUR.background);
    
    if(isEditMode())
        g.drawRoundedRectangle(area.toFloat(), 0, 2.0);
}
// --------------------------------------------------
void IRVideoPlayerObject::videoLoadCompletedAction()
{
    int video_w = this->videoPlayer->getVideoSize().getWidth();
    int video_h = this->videoPlayer->getVideoSize().getHeight();
    
    int w,h;
   if(video_w > getWidth())
   {
       w = getWidth();
       h = (int)((float)w / this->videoPlayer->getAspectRatio());
   }else{
       w = video_w;
       h = video_h;

   }
    
    setSize(w + 10, h + 10);    
    // call reset Heavy-weight components
    callHeavyComponentCreated(this);
    // and bring this obejct to the top of objectZOrder on the workspace
    callAddHeavyComponentToTopZOrder(this);
}
// --------------------------------------------------

void IRVideoPlayerObject::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == this->controller.get())
    {
    
        using uiStatus = VideoController::VideoControllerStatus;
        
        switch(this->controller->getStatus())
        {
            case uiStatus::OpenMovieFile:
                this->videoPlayer->openFile();
                break;
            default:
                break;
        }
    }
}
// --------------------------------------------------

void IRVideoPlayerObject::moveToFrontAction()
{
    if(this->videoPlayer->hsaVideo())
        this->videoPlayer->bringViewToFront();
    
}

// --------------------------------------------------

void IRVideoPlayerObject::heavyComponentRefreshed()
{
    moveToFrontAction();
}
