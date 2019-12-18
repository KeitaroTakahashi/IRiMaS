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
//#include "IRVideoPlayerController.hpp"

class IRVideoPlayer : public IRUIFoundation,
                      public DragAndDropContainer,
public ChangeListener
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
        
    }
    
    ~IRVideoPlayer()
    {
        this->player.reset();
    }
    // --------------------------------------------------

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
            this->path.swapWith(path);
            
            this->player->loadVideo(url);
                        
        }
    }
    
    void openFile(File file, bool isCallback = true)
    {
        auto url = URL (file);
        this->movieFile = file;
        std::cout << url.getFileName() << std::endl;
        this->player->loadVideo(url);
    }
    
    void videoLoadCompleteAction()
    {
        
        removeChildComponent(&this->openButton);
        addAndMakeVisible(this->player.get());
                
        if(this->videoLoadCompleted != nullptr)
            this->videoLoadCompleted();
        
        
        
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
    std::string getPath() const { return this->path.toStdString(); } 
    // --------------------------------------------------
    
    std::function<void()> videoLoadCompleted;
    // --------------------------------------------------

    void bringViewToFront()
    {
        this->player->bringViewToFront();
        
    }
    
    // --------------------------------------------------

    
private:
    std::shared_ptr<IRVideoComponent> player;

    // --------------------------------------------------
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }

    // --------------------------------------------------
    // --------------------------------------------------
    // --------------------------------------------------

    Rectangle<int> videoSize;
    float aspectRatio = 0.0;
    bool isVideoLoaded = false;
    String path;
    File movieFile;
    
    bool isController = false;
    bool isVideoOpenedFlag = false;
    // --------------------------------------------------

    TextButton openButton;
    
     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoPlayer)
    //JUCE_LEAK_DETECTOR (IRVideoPlayer)
    
};
#endif /* IRVideoPlayer_hpp */
