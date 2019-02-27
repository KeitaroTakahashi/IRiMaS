//
//  NSVideoPlayerObject.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 06/02/2019.
//

#ifndef NSVideoPlayerObject_h
#define NSVideoPlayerObject_h

#include "IRNodeObject.hpp"
#include "IRVideoPlayer.hpp"

class NSVideoPlayerObject : public NSNodeObject
{
public:
    NSVideoPlayerObject(Component* parent) : NSNodeObject(parent, "NSVideoPlayer")
    {
        this->videoPlayer = new IRVideoPlayer();
        this->videoPlayer->setBounds(getLocalBounds().reduced(5));
        addAndMakeVisible(this->videoPlayer);
    }
    ~NSVideoPlayerObject()
    {
        delete this->videoPlayer;
    }
    
    // --------------------------------------------------
    
    NSNodeObject* copyThis() override
    {
        return new NSVideoPlayerObject(this->parent);
    }
    
    NSNodeObject* copyThisWithContents() override
    {
        NSVideoPlayerObject* obj = new NSVideoPlayerObject(this->parent);
        obj->setBounds(getLocalBounds());
        File movieFile = obj->getVideoPlayer()->getMovieFile();
        obj->getVideoPlayer()->openFile(movieFile);
        
        return obj;
    }
    
    NSNodeObject* copySelectedContents() override
    {
        NSVideoPlayerObject* obj = new NSVideoPlayerObject(this->parent);
        
        return obj;
    }
    // --------------------------------------------------
    t_json saveThisToSaveData() override
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
    void loadThisFromSaveData(t_json data) override
    {
        t_json w = data["videoPlayer"];
        
        File file(w["filePath"].string_value());
        this->videoPlayer->openFile(file);
    }
    
    // --------------------------------------------------
    
    void resized() override
    {
        this->videoPlayer->setBounds(getLocalBounds().reduced(5));
    }
    
    void resizeThisComponentEvent(const MouseEvent& e) override
    {
        
        // off controller, otherwise mouse event will be stolen by the controller,
        // and this event can not be acomplished properly.
        if(this->videoPlayer->isNeedController() && this->videoPlayer->isVideoOpened())
            this->videoPlayer->setNeedController(false);
        
        
        double ratio = this->videoPlayer->getAspectRatio();
        std::cout <<"ratio = "<< ratio << std::endl;
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
            NSNodeComponent::resizeThisComponentEvent(e);
        }
    }
    // --------------------------------------------------
    void mouseUpEvent(const MouseEvent& e) override
    {
        //recover event
        if(!this->videoPlayer->isNeedController() && this->videoPlayer->isVideoOpened())
            this->videoPlayer->setNeedController(true);
    }
    
    void paint(Graphics& g) override
    {
        NSNodeObject::paint(g);
    }
    // --------------------------------------------------
    
    IRVideoPlayer* getVideoPlayer() { return this->videoPlayer; }
    
private:
    IRVideoPlayer *videoPlayer;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NSVideoPlayerObject)
    
};


#endif /* NSVideoPlayerObject_h */
