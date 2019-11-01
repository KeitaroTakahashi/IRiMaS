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

class IRVideoPlayer : public IRUIFoundation,
                      public DragAndDropContainer
{
public:
    IRVideoPlayer(IRNodeObject* parent, IRStr* str) :
    IRUIFoundation(parent, str)
    {
        this->openButton.setButtonText("open a movie file");
        this->openButton.onClick =[this] { openFile(); };
        addAndMakeVisible(this->openButton);

    }
    
    ~IRVideoPlayer()
    {
        //this->player->closeVideo();
        //this->player_with_controller->closeVideo();
        
        this->player.reset();
        this->player_with_controller.reset();

    }
    // --------------------------------------------------

    // --------------------------------------------------
    
    void resized() override
    {
        if(this->player.get() != nullptr)
            this->player->setBounds(getLocalBounds());
        if(this->player_with_controller.get() != nullptr)
            this->player_with_controller->setBounds(getLocalBounds());
        
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
            this->path.swapWith(path);
            
            loadVideo(url);
                        
        }
    }
    
    void openFile(File file)
    {
        auto url = URL (file);
        std::cout << url.getFileName() << std::endl;
        loadVideo(url);
    }
    // --------------------------------------------------

    void loadVideo(URL url)
    {
        
        this->player.reset(new VideoComponent(false));
        this->player->load(url);
        this->player_with_controller.reset(new VideoComponent(true));

        this->player_with_controller->loadAsync(url, [this] (const URL& u, Result r) { videoLoadingFinished (u, r); });
    }

    
    // --------------------------------------------------
    // call back function for video loader
    void videoLoadingFinished (const URL& url, Result result)
    {
        
        ignoreUnused (url);
        
        if(result.wasOk()){
            this->videoSize = this->player_with_controller->getVideoNativeSize();
            KLib().showRectangle<int>(this->videoSize);
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
            this->player->setBounds(0,0,w,h);
            this->player_with_controller->setBounds(0,0,w,h);
            addAndMakeVisible(this->player_with_controller.get());
            removeChildComponent(&this->openButton);
            
            this->isVideoOpenedFlag = true;
            
            if(videoLoadCompleted != nullptr)
            {
                videoLoadCompleted();
            }
        }else{
            this->isVideoOpenedFlag = false;
            removeChildComponent(this->player.get());
            removeChildComponent(this->player_with_controller.get());
            KLib().showConnectionErrorMessage("Could not load the video file of "+url.getSubPath());
        }
    }
    // --------------------------------------------------
    // switch navi on off
    void setNeedController(bool flag)
    {
        this->isController = flag;
        if(flag){
            removeChildComponent(this->player.get());
            addAndMakeVisible(this->player_with_controller.get());
        }else{
            removeChildComponent(this->player_with_controller.get());
            addAndMakeVisible(this->player.get());
        }
    }
    
    bool isNeedController() const {return this->isController;}
    // --------------------------------------------------
    File getMovieFile() const { return this->movieFile; }
    // --------------------------------------------------
    Rectangle<int> getVideoSize() const { return this->videoSize; }
    float getAspectRatio() const { return this->aspectRatio; }
    // --------------------------------------------------
    bool isVideoOpened() const { return this->isVideoOpenedFlag; }
    // --------------------------------------------------
    std::string getPath() const { return this->path.toStdString(); } 
    // --------------------------------------------------
    
    std::function<void()> videoLoadCompleted;
    // --------------------------------------------------

    void bringViewToFront()
    {
        
#if JUCE_MAC
        NSViewComponent* view = static_cast<NSViewComponent*>(this->player_with_controller->getPimpl());
        if(view == nullptr) return;
        
        IRNSViewManager manager;
        manager.bringOpenGLContextFront(this, view);
        
#endif
        
    }
    
    
    
private:
    std::shared_ptr<VideoComponent> player;
    
    // no idea how to switch Navi Controlelr on off, so that we prepare two objects, one is with and another is without.
    std::shared_ptr<VideoComponent> player_with_controller;
        
    Rectangle<int> videoSize;
    float aspectRatio = 0.0;
    bool isVideoLoaded = false;
    String path;
    File movieFile;
    
    bool isController = false;
    bool isVideoOpenedFlag = false;
    
    TextButton openButton;
    
     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoPlayer)
    //JUCE_LEAK_DETECTOR (IRVideoPlayer)
    
};
#endif /* IRVideoPlayer_hpp */
