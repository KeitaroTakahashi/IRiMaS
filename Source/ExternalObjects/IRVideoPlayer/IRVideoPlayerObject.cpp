//
//  IRVideoPlayerObject.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/01/2019.
//

#include "IRVideoPlayerObject.hpp"

IRVideoPlayerObject::IRVideoPlayerObject(Component* parent, IRStr* str, bool withOpenButton) :
IRNodeObject(parent, "IRVideoPlayer", str, NodeObjectType(orginaryIRComponent))
{
    
    setOpaque(false);
       
    // original function to give this ChangeListener to controller->UI
    
    this->videoPlayer = std::make_shared<IRVideoPlayer>(this, str, withOpenButton);
    this->videoPlayer->videoLoadCompleted = [this]{ videoLoadCompletedAction(); };
    addAndMakeVisible(this->videoPlayer.get());
    this->videoPlayer->updateAnimationFrameCallback = [this](double pos) { videoPlayingUpdateAction(pos); };
    
    
    setSize(300, 200);
}

IRVideoPlayerObject::~IRVideoPlayerObject()
{
    this->videoPlayer.reset();

}

IRNodeObject* IRVideoPlayerObject::copyThis()
{
    return new IRVideoPlayerObject(this->parent, getStr());
}
// --------------------------------------------------
IRNodeObject* IRVideoPlayerObject::copyContents(IRNodeObject* object)
{
    IRVideoPlayerObject* obj = static_cast<IRVideoPlayerObject*>(object);
    obj->setBounds(getLocalBounds());
    File movieFile = getVideoPlayer()->getMovieFile();
    if(movieFile.exists())
    {
        obj->openFile(movieFile, false);
    }
    
    return obj;
}
// --------------------------------------------------
IRNodeObject* IRVideoPlayerObject::copyDragDropContents(IRNodeObject* object)
{
    IRVideoPlayerObject* obj = new IRVideoPlayerObject(this->parent, getStr());
    return obj;
}

void IRVideoPlayerObject::shareContentsWith(IRVideoPlayerObject* withObject)
{
    File movieFile = getVideoPlayer()->getMovieFile();
    if(movieFile.exists())
    {
        withObject->openFile(movieFile, false);
    }
}
// --------------------------------------------------

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
    if(this->videoPlayer->isNeedController() && this->videoPlayer->hsaVideo())
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
    if(!this->videoPlayer->isNeedController() && this->videoPlayer->hsaVideo())
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
    // Keitaro : I decided not to fix the object size according to the loaded video size because it causes a lot of unnecessary difficulties.
    /*
    int video_w = this->videoPlayer->getVideoSize().getWidth();
    int video_h = this->videoPlayer->getVideoSize().getHeight();
    
    int w,h;
   if(video_w > getWidth())
   {
       w = getWidth();
       h = (int)((float)w / this->videoPlayer->getAspectRatio());
       
       // then
       if(h > getHeight())
       {
           float newRatio = (float)getHeight() / (float)h;
           h = getHeight();
           w = (int)((float)w * newRatio);
       }
   }else{
       w = video_w;
       h = video_h;
   }
    
    */
    //setSize(w + 10, h + 10);
    //setSize(w, h);

    // callback
    videoLoadCompletedCallback();
    
    // called only when isCallback is true. isCallback is defined in this class.
    if(this->videoLoadCompletedCallbackFunc != nullptr)
    {
        if(this->isCallback)
            this->videoLoadCompletedCallbackFunc();
    }

    bringThisToFront();
    // call reset Heavy-weight components
    callHeavyComponentCreated(this);
    
    // and bring this obejct to the top of objectZOrder on the workspace
    //callAddHeavyComponentToTopZOrder(this);
    
}
// --------------------------------------------------

void IRVideoPlayerObject::videoPlayingUpdateAction(double pos)
{
    videoPlayingUpdateCallback(pos);
}

// --------------------------------------------------

void IRVideoPlayerObject::moveToFrontAction()
{
    std::cout << "IRVideoPlayerObject::moveToFrontAction\n";
    if(this->videoPlayer->hsaVideo())
        this->videoPlayer->bringViewToFront();
    
}

// --------------------------------------------------

void IRVideoPlayerObject::heavyComponentRefreshed()
{
    moveToFrontAction();
}


// --------------------------------------------------
void IRVideoPlayerObject::openFile(File file, bool isCallback)
{
    this->isCallback = isCallback;
    if(this->videoPlayer.get() != nullptr)
    {
        this->videoPlayer->openFile(file);
    }
}

void IRVideoPlayerObject::openFile(bool isCallback)
{
    
    this->isCallback = isCallback;
    if(this->videoPlayer.get() != nullptr)
    {
        this->videoPlayer->openFile();
    }
}
// --------------------------------------------------

void IRVideoPlayerObject::play()
{
    this->videoPlayer->play();
}
void IRVideoPlayerObject::stop()
{
    this->videoPlayer->stop();
}

// --------------------------------------------------
// --------------------------------------------------
