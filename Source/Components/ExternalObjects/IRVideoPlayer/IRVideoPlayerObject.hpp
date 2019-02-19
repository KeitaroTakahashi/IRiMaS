//
//  IRVideoPlayerObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/01/2019.
//

#ifndef IRVideoPlayerObject_hpp
#define IRVideoPlayerObject_hpp

#include "IRNodeObject.hpp"
#include "IRVideoPlayer.hpp"


class IRVideoPlayerObject : public IRNodeObject
{
public:
    IRVideoPlayerObject(Component* parent) : IRNodeObject(parent, "IRVideoPlayer", NodeObjectType(heavyWeightComponent))
    {
        this->videoPlayer = new IRVideoPlayer();
        this->videoPlayer->setBounds(getLocalBounds().reduced(5));
        addAndMakeVisible(this->videoPlayer);
    }
    ~IRVideoPlayerObject()
    {
        delete this->videoPlayer;
    }
    
    // --------------------------------------------------
    
    IRNodeObject* copyThis() override
    {
        return new IRVideoPlayerObject(this->parent);
    }
    
    IRNodeObject* copyThisWithContents() override
    {
        IRVideoPlayerObject* obj = new IRVideoPlayerObject(this->parent);
        obj->setBounds(getLocalBounds());
        File movieFile = obj->getVideoPlayer()->getMovieFile();
        obj->getVideoPlayer()->openFile(movieFile);
        
        return obj;
    }
    
    IRNodeObject* copySelectedContents() override
    {
        IRVideoPlayerObject* obj = new IRVideoPlayerObject(this->parent);

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
            IRNodeComponent::resizeThisComponentEvent(e);
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
        IRNodeObject::paint(g);
    }
    // --------------------------------------------------
    
    IRVideoPlayer* getVideoPlayer() { return this->videoPlayer; }

private:
    IRVideoPlayer *videoPlayer;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoPlayerObject)
    
};

#endif /* IRVideoPlayerObject_hpp */
