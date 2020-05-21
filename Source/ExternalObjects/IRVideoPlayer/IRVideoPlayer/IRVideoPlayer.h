//
//  IRVideoPlayer.h
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
    IRVideoPlayer(IRNodeObject* parent, IRStr* str, bool withOpenButton = true) :
    IRUIFoundation(parent, str)
    {
        if(withOpenButton)
        {
            this->openButton.setButtonText("open a movie file");
            this->openButton.onClick =[this] { openFile(); };
            addAndMakeVisible(this->openButton);
        }

        this->player.reset(new IRVideoComponent());
        this->player->videoLoadCompleted = [this] { videoLoadCompleteAction(); };
        
        //update rate for getting the video playing status
        setFps(30);
        
    }

    ~IRVideoPlayer()
    {
        this->player.reset();
    }

    // --------------------------------------------------
    
    void resized() override
    {
        if(this->player != nullptr)
            this->player->setBounds(getLocalBounds());
      
        this->openButton.setBounds(getLocalBounds());
    }
    
    void paint(Graphics &g) override
    {
        g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    }

    
    // --------------------------------------------------
    void openFile()
    {
        FileChooser chooser("Select an video file to play...",
                            {},
                            "*.mov, *.mp4, *.m4v");
        
        FilenameComponent co {"movie", {}, true, false, false, "*", {}, "(choose a video file to play)"};
        
        if(chooser.browseForFileToOpen())
        {
            auto file = chooser.getResult();
            this->movieFile = file;
            auto path = file.getFullPathName();
            auto url = URL (file);
            
            std::cout << url.getFileName() << std::endl;
            this->videoFilePath = path.toStdString();
            this->player->loadVideo(url);
        }
    }
    
    void openFile(File file, bool isCallback = true)
    {
        auto url = URL (file);
        this->videoFilePath = file.getFullPathName().toStdString();
        this->movieFile = file;
        std::cout << url.getFileName() << std::endl;
        this->player->loadVideo(url, isCallback);
    }
    
    void videoLoadCompleteAction()
    {
        removeChildComponent(&this->openButton);
        addAndMakeVisible(this->player.get());
                
        if(this->videoLoadCompleted != nullptr)
            this->videoLoadCompleted();
        
        startAnimation();

    }

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
      
    }
    double getPlayPosition(){ return this->player->getPlayPosition(); }
    void setPlaySpeed(double newSpeed) { this->player->setPlaySpeed(newSpeed); }
    double getPlaySpeed() { return this->player->getPlaySpeed(); }
    void setAudioVolume(float newVolume) { this->player->setAudioVolume(newVolume); }
    float getAudioVolume() { return this->player->getAudioVolume(); }
    double getVideoLength() { return this->player->getVideoLength(); }
    
    Rectangle<int> getCurrentVideoBounds() { return this->player->getCurrentVideoBounds(); }

    
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
