//
//  IRVideoPlayer.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/01/2019.
//

#ifndef IRVideoPlayer_hpp
#define IRVideoPlayer_hpp

// opencv for getting a video frame
#include "IRUIFoundation.hpp"
#include "IRFoundation.h"
#include "IRNSViewManager.hpp"
#include "IRVideoComponent.h"
#include "KeAnimationComponent.h"

class IRVideoPlayer : public IRUIFoundation,
                      public DragAndDropContainer,
public ChangeListener,
public KeAnimationComponent
{
public:
    IRVideoPlayer(IRNodeObject* parent, IRStr* str, bool withOpenButton = true);
    ~IRVideoPlayer();

    // --------------------------------------------------
    
    void resized() override;
    void paint(Graphics &g) override;

    
    // --------------------------------------------------
    void openFile();
    void openFile(File file, bool isCallback = true);
    void videoLoadCompleteAction();

    // --------------------------------------------------
    
    bool hsaVideo() const { return this->player->hasVideo(); }
  
    // --------------------------------------------------
    // switch navi on off
    void setNeedController(bool flag)
    {
        this->player->setNeedController(flag);
    }
    
    bool isNeedController() const {return this->player->isController(); }
    // --------------------------------------------------
    File getMovieFile() const { return this->movieFile; }
    // --------------------------------------------------
    Rectangle<int> getVideoSize() const { return this->player->getVideoSize(); }
    float getAspectRatio() const { return this->player->getAspectRatio(); }
    // --------------------------------------------------

    // --------------------------------------------------
    std::string getPath() const { return this->videoFilePath; } 
    // --------------------------------------------------
    
    std::function<void()> videoLoadCompleted;
    std::function<void(double)> updateAnimationFrameCallback;
    // --------------------------------------------------

    void bringViewToFront() { this->player->bringViewToFront(); }
    
    // --------------------------------------------------
    void play() { this->player->play(); }
    void stop() { this->player->stop(); }
    
    bool isPlaying() const { return this->player->isPlaying(); }
    void setPlayPosition(double newPlayPositionInSec)
    {
        this->player->setPlayPosition(newPlayPositionInSec);
        
        // NOT necessary anymore
        /*
        // call playback
        if(this->updateAnimationFrameCallback != nullptr)
            this->updateAnimationFrameCallback(newPlayPositionInSec);
         */
    }
    double getPlayPosition(){ return this->player->getPlayPosition(); }
    void setPlaySpeed(double newSpeed) { this->player->setPlaySpeed(newSpeed); }
    double getPlaySpeed() { return this->player->getPlaySpeed(); }
    void setAudioVolume(float newVolume) { this->player->setAudioVolume(newVolume); }
    float getAudioVolume() { return this->player->getAudioVolume(); }
    double getVideoLength() { return this->player->getVideoLength(); }
    
private:
    std::shared_ptr<IRVideoComponent> player;

    // --------------------------------------------------
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }

    // --------------------------------------------------
    
    void updateAnimationFrame() override
    {
        if(isPlaying())
        {
            if(this->updateAnimationFrameCallback != nullptr)
                this->updateAnimationFrameCallback(this->player->getPlayPosition());
        }
    }
    // --------------------------------------------------
    // --------------------------------------------------

    Rectangle<int> videoSize;
    float aspectRatio = 0.0;
    bool isVideoLoaded = false;
    std::string videoFilePath;
    File movieFile;
    
    bool isController = false;
    bool isVideoOpenedFlag = false;
    // --------------------------------------------------

    TextButton openButton;
    
     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoPlayer)
    
};
#endif /* IRVideoPlayer_hpp */
