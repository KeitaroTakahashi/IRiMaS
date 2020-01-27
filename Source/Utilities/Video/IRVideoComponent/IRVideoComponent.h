//
//  IRVideoComponent.h
//  IRiMaS
//
//  Created by Keitaro on 10/12/2019.
//

#ifndef IRVideoComponent_h
#define IRVideoComponent_h

#include "JuceHeader.h"
#include "KLib.h"
#include "IRNSViewManager.hpp"

class IRVideoComponent : public Component
{
public:
    IRVideoComponent()
    {
        this->player_with_controller.reset(new VideoComponent(true));
        this->player_without_controller.reset(new VideoComponent(false));
        addAndMakeVisible(this->player_with_controller.get());
        this->currentPlayer = this->player_with_controller.get();

    }
    
    ~IRVideoComponent()
    {
        
    }
    // ==================================================

    void paint(Graphics& g) override
    {
        
    }

    void resized() override
    {
        this->player_with_controller->setBounds(getLocalBounds());
        this->player_without_controller->setBounds(getLocalBounds());
    }
    
    // ==================================================
    void play()
    {
        if(this->currentPlayer != nullptr)
            this->currentPlayer->play();
    }
    
    void stop()
    {
        if(this->currentPlayer != nullptr)
            this->currentPlayer->stop();
    }
    
    bool isPlaying() const
    {
        if(this->currentPlayer == nullptr) return false;
        else return this->currentPlayer->isPlaying();
    }
    void setPlayPosition(double newPlayPositionInSec)
    {
        if(this->currentPlayer != nullptr)
            this->currentPlayer->setPlayPosition(newPlayPositionInSec);
    }
    double getPlayPosition()
    {
        if(this->currentPlayer == nullptr) return 0;
        else return this->currentPlayer->getPlayPosition();
    }
    void setPlaySpeed(double newSpeed)
    {
        if(this->currentPlayer != nullptr)
            this->currentPlayer->setPlaySpeed(newSpeed);
        
    }
    double getPlaySpeed()
    {
        if(this->currentPlayer == nullptr) return 1.0;
        else return this->currentPlayer->getPlaySpeed();
    }
    void setAudioVolume(float newVolume)
    {
        if(this->currentPlayer != nullptr)
            this->currentPlayer->setAudioVolume(newVolume);
    }
    
    float getAudioVolume()
    {
        if(this->currentPlayer == nullptr) return 0;
        else return this->currentPlayer->getAudioVolume();
    }
    double getVideoLength()
    {
        if(this->currentPlayer == nullptr) return 0;
        else return this->currentPlayer->getVideoDuration();
    }
    
    // ==================================================

    
    void setNeedController(bool flag)
    {
        this->isShowControllerFlag = flag;
        
        if(flag)
        {
            removeChildComponent(this->player_without_controller.get());
            addAndMakeVisible(this->player_with_controller.get());
            this->currentPlayer = this->player_with_controller.get();

        }else
        {
            removeChildComponent(this->player_with_controller.get());
            addAndMakeVisible(this->player_without_controller.get());
            this->currentPlayer = this->player_without_controller.get();

        }
    }
    
    bool isController() const { return this->isShowControllerFlag; }
    
    void loadVideo(URL url, bool callback = true)
    {
        this->url = url;
        this->isCallback = callback;
        
        this->player_without_controller->load(url);
        this->player_with_controller->loadAsync(url, [this] (const URL& u, Result r) {
            videoLoadingFinished (u, r);
        });
    }
    
    void videoLoadingFinished (const URL& url, Result result)
    {
        ignoreUnused (url);

        if(result.wasOk()){
            
            this->isVideoLoaded = true;
            
            this->videoSize = this->player_with_controller->getVideoNativeSize();
            this->aspectRatio = (float)this->videoSize.getWidth() / (float)this->videoSize.getHeight();
           
            setNeedController(false);
            
            int w,h;
            if(this->videoSize.getWidth() > getWidth())
            {
                w = getWidth();
                h = (int)((float)w / this->aspectRatio);
            }else{
                w = this->videoSize.getWidth();
                h = this->videoSize.getHeight();
            }
            
            addAndMakeVisible(this->player_with_controller.get());
            this->currentPlayer = this->player_with_controller.get();
            
            if(this->isCallback)
            {
                if(videoLoadCompleted != nullptr)
                {
                    videoLoadCompleted();
                }
            }
                
        }else{
            this->isVideoLoaded = false;
            removeChildComponent(this->player_without_controller.get());
            removeChildComponent(this->player_with_controller.get());
            this->currentPlayer = nullptr;
            KLib().showConnectionErrorMessage("Could not load the video file of "+url.getSubPath());
        }
    }
    // ==================================================
    
    bool hasVideo() const { return this->isVideoLoaded; }

    Rectangle<int> getVideoSize() const { return this->videoSize; }
    float getAspectRatio() const { return this->aspectRatio; }
    
    URL getURL() const { return this->url; }
        
    // ==================================================
    void bringViewToFront()
    {
        
    #if JUCE_MAC
        NSViewComponent* view = static_cast<NSViewComponent*>(this->player_with_controller->getPimpl());
        if(view == nullptr) return;
        
        IRNSViewManager manager;
        manager.bringOpenGLContextFront(this, view);
        
    #endif
        
    }
    // ==================================================

    //callback
    std::function<void()> videoLoadCompleted = nullptr;
    
private:
    
    std::shared_ptr<VideoComponent> player_with_controller;
    std::shared_ptr<VideoComponent> player_without_controller;
    
    VideoComponent* currentPlayer = nullptr;

    bool isShowControllerFlag = true;
    bool isVideoLoaded = false;
    
    bool isCallback = true;
    
    URL url;
    
    Rectangle<int> videoSize;
    
    // width / height
    float aspectRatio = 1.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRVideoComponent)

};
#endif /* IRVideoComponent_h */
