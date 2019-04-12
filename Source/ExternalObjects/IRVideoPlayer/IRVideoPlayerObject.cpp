//
//  IRVideoPlayerObject.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/01/2019.
//

#include "IRVideoPlayerObject.hpp"

IRNodeObject* IRVideoPlayerObject::copyThis()
{
    return new IRVideoPlayerObject(this->parent);
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
    IRVideoPlayerObject* obj = new IRVideoPlayerObject(this->parent);
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
}
// --------------------------------------------------
void IRVideoPlayerObject::mouseUpEvent(const MouseEvent& e)
{
    //recover event
    if(!this->videoPlayer->isNeedController() && this->videoPlayer->isVideoOpened())
        this->videoPlayer->setNeedController(true);
}
// --------------------------------------------------
void IRVideoPlayerObject::paint(Graphics& g)
{
    IRNodeObject::paint(g);
    auto area = getLocalBounds().reduced (10);
    
    g.setColour (Colours::black);
    g.fillRoundedRectangle (area.toFloat(), 5.0f);
}
// --------------------------------------------------
void IRVideoPlayerObject::videoLoadCompletedAction()
{
    setSize(this->videoPlayer->getWidth(), this->videoPlayer->getHeight());
}
// --------------------------------------------------
