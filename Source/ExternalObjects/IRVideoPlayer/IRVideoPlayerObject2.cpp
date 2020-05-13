//
//  IRVideoPlayerObject2.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/01/2019.
//

#include "IRVideoPlayerObject2.hpp"

IRVideoPlayerObject2::IRVideoPlayerObject2(Component* parent, IRStr* str, bool withOpenButton) :
IRNodeObject(parent, "IRVideoPlayer", str, NodeObjectType(ordinaryIRComponent))
{
    setOpaque(false);
    // original function to give this ChangeListener to controller->UI
    
    
   
    
    this->videoPlayer = std::make_shared<IRVideoPlayer>(this, str, withOpenButton);
    this->videoPlayer->videoLoadCompleted = [this]{ videoLoadCompletedAction(); };
    addAndMakeVisible(this->videoPlayer.get());
    this->videoPlayer->updateAnimationFrameCallback = [this](double pos) { videoPlayingUpdateAction(pos); };
    
    
}

IRVideoPlayerObject2::~IRVideoPlayerObject2()
{
    this->videoPlayer.reset();

}

IRNodeObject* IRVideoPlayerObject2::copyThis()
{
    return new IRVideoPlayerObject2(this->parent, getStr());
}
// --------------------------------------------------
IRNodeObject* IRVideoPlayerObject2::copyContents(IRNodeObject* object)
{
    IRVideoPlayerObject2* obj = static_cast<IRVideoPlayerObject2*>(object);
    obj->setObjectBounds(getLocalBounds());
    File movieFile = getVideoPlayer()->getMovieFile();
    if(movieFile.exists())
    {
        obj->openFile(movieFile, false);
    }
    return obj;
}

// --------------------------------------------------

IRNodeObject* IRVideoPlayerObject2::copyDragDropContents(IRNodeObject* object)
{
    IRVideoPlayerObject2* obj = new IRVideoPlayerObject2(this->parent, getStr());
    return obj;
}

void IRVideoPlayerObject2::shareContentsWith(IRVideoPlayerObject2* withObject)
{
    File movieFile = getVideoPlayer()->getMovieFile();
    if(movieFile.exists())
    {
        withObject->openFile(movieFile, false);
    }
}

// --------------------------------------------------

// --------------------------------------------------
t_json IRVideoPlayerObject2::saveThisToSaveData()
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
void IRVideoPlayerObject2::loadThisFromSaveData(t_json data)
{
    t_json w = data["videoPlayer"];
    
    File file(w["filePath"].string_value());
    this->videoPlayer->openFile(file);
}
// --------------------------------------------------
void IRVideoPlayerObject2::resized()
{
   this->videoPlayer->setBounds(getLocalBounds().reduced(0));
   
}
// --------------------------------------------------
void IRVideoPlayerObject2::resizeThisComponentEvent(const MouseEvent& e)
{
    // turn off controller otherwise mouse event will be stolen by the controller,
    // and resize event can not be acomplished properly.
    if(this->enableControllerFlag)
    {
        if(this->videoPlayer->isNeedController() && this->videoPlayer->hsaVideo())
            this->videoPlayer->setNeedController(false);
    }
    
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
        setObjectSize(newWidth, newHeight);

    }else{
        IRNodeComponent::resizeThisComponentEvent(e);
    }
    
    this->resizing = true;
}

void IRVideoPlayerObject2::resizeThisComponent(Rectangle<int> rect)
{
    double ratio = this->videoPlayer->getAspectRatio();
    if(ratio >= 0 && ratio != 1.0)
    {
        float curr_w = (float)getWidth();
        float curr_h = (float)getHeight();
        float new_w  = (float)rect.getWidth();
        float new_h  = (float)rect.getHeight();
        
        float ratio_w = curr_w / new_w;
        float ratio_h = curr_h / new_h;
                
        // if w is larger, then follow w
        if(ratio_w >= ratio_h)
        {
            
            float fixed_h = new_w / ratio;
            float y = (rect.getHeight() - fixed_h) / 2.0;
            setObjectBounds(rect.getX(), rect.getY() + y, new_w, fixed_h);
        }else{
            float fixed_w = new_h * ratio;
            float x = (rect.getWidth() - fixed_w) / 2.0;

            setObjectBounds(rect.getX() + x, rect.getY(), fixed_w, new_h);
        }
        
    }else{
        setObjectBounds(rect);
    }
    
}

juce::Point<int> IRVideoPlayerObject2::getVideoSize()
{
    int w = this->videoPlayer->getVideoSize().getWidth();
    int h = this->videoPlayer->getVideoSize().getHeight();
    return juce::Point<int>( w, h );
}

// --------------------------------------------------

void IRVideoPlayerObject2::mouseUpEvent(const MouseEvent& e)
{
    //recover event
    if(this->enableControllerFlag)
    {
        if(!this->videoPlayer->isNeedController() && this->videoPlayer->hsaVideo())
        {
            this->videoPlayer->setNeedController(true);
        
            refreshZIndex();
        }
    }
    
    if(this->resizing)
    {
        this->resizing = false;
    }
    

}
// --------------------------------------------------
void IRVideoPlayerObject2::paint(Graphics& g)
{
    IRNodeObject::paint(g);
    
    auto area = getLocalBounds();
    g.fillAll(getStr()->SYSTEMCOLOUR.background);
    
    if(isEditMode())
        g.drawRoundedRectangle(area.toFloat(), 0, 2.0);
}
// --------------------------------------------------
void IRVideoPlayerObject2::videoLoadCompletedAction()
{
    
    
    bringThisToFront();
    // call reset Heavy-weight components
    refreshZIndex();
    // called only when isCallback is true. isCallback is defined in this class.
    if(this->videoLoadCompletedCallbackFunc != nullptr)
    {
        if(this->isCallback)
            this->videoLoadCompletedCallbackFunc();
    }

    std::cout << "IRVideoPlayerObject2::videoLoadCompletedAction\n";

    // virtual
    videoLoadCompletedCallback();
    
    
    
}
// --------------------------------------------------

void IRVideoPlayerObject2::videoPlayingUpdateAction(double pos)
{
    videoPlayingUpdateCallback(pos);
}

// --------------------------------------------------

void IRVideoPlayerObject2::moveToFrontAction()
{
    std::cout << "IRVideoPlayerObject2::moveToFrontAction\n";
    if(this->videoPlayer->hsaVideo())
        this->videoPlayer->bringViewToFront();
    

    
}

// --------------------------------------------------

void IRVideoPlayerObject2::heavyComponentRefreshed()
{
    moveToFrontAction();
}


// --------------------------------------------------
void IRVideoPlayerObject2::openFile(File file, bool isCallback)
{
    this->isCallback = nullptr;
    if(isCallback) this->isCallback = isCallback;
    
    if(this->videoPlayer.get() != nullptr)
    {
        this->videoPlayer->openFile(file, isCallback);
    }
}

void IRVideoPlayerObject2::openFile(bool isCallback)
{
    
    this->isCallback = isCallback;
    if(this->videoPlayer.get() != nullptr)
    {
        this->videoPlayer->openFile();
    }
}
// --------------------------------------------------

void IRVideoPlayerObject2::play()
{
    this->videoPlayer->play();
}

void IRVideoPlayerObject2::stop()
{
    this->videoPlayer->stop();
}

void IRVideoPlayerObject2::setPlayPosition(double newPositionInSec)
{
    this->videoPlayer->setPlayPosition(newPositionInSec);
    
    // inform the new play position to its child classes.
    //videoPlayingUpdateAction(newPositionInSec);
}
// --------------------------------------------------

void IRVideoPlayerObject2::statusChangedCallback(IRNodeComponentStatus status)
{
    switch (status)
    {
        case EditModeStatus:
            break;
        case SelectableStatus:
            break;
        case HasResizedStatus:
            break;
        default:
            break;
    }
}

// ---------------------------------------------------

void IRVideoPlayerObject2::enableController(bool flag)
{
    this->enableControllerFlag = flag;
    this->videoPlayer->setNeedController(flag);
}

// ---------------------------------------------------

// ---------------------------------------------------


void IRVideoPlayerObject2::refreshZIndex()
{

 
    callHeavyComponentCreated(this);

}
// ---------------------------------------------------


void IRVideoPlayerObject2::resizingSquareClickedAction(IRResizeSquare2::MovableDirection direction)
{
    
}
void IRVideoPlayerObject2::resizingSquareReleasedAction(IRResizeSquare2::MovableDirection direction)
{
    if(this->videoPlayer->hsaVideo())
        refreshZIndex();
}
void IRVideoPlayerObject2::resizingSquareDraggedAction(MouseEvent e)
{
    
}