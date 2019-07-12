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


class IRVideoPlayer : public IRUIFoundation,
                      public DragAndDropContainer
{
public:
    IRVideoPlayer(IRNodeObject* parent) :
    IRUIFoundation(parent),
    player(false),
    player_with_controller(true)
    {
        this->openButton.setButtonText("open a movie file");
        this->openButton.setColour(TextButton::buttonColourId,
                                   SYSTEMCOLOUR.fundamental);
        this->openButton.setColour(TextButton::textColourOffId,
                                   Colour((uint8)255, (uint8)255, (uint8)255, (uint8)255));
        this->openButton.setColour(TextButton::textColourOnId,
                                   Colour((uint8)255, (uint8)255, (uint8)255, (uint8)255));
        this->openButton.onClick =[this] { openFile(); };
        addAndMakeVisible(this->openButton);

    }
    
    ~IRVideoPlayer()
    {
        
    }
    // --------------------------------------------------

    // --------------------------------------------------
    
    void resized() override
    {
        this->player.setBounds(getLocalBounds());
        this->player_with_controller.setBounds(getLocalBounds());
        this->openButton.setBounds(getLocalBounds());
        
    }
    void paint(Graphics &g) override
    {
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
            
            // register file
            
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
        this->player.load(url);
        this->player_with_controller.loadAsync(url, [this] (const URL& u, Result r) { videoLoadingFinished (u, r); });
    }
    
    // just for test
    void registerFileToManager();
    
    // --------------------------------------------------
    // call back function for video loader
    void videoLoadingFinished (const URL& url, Result result)
    {
        ignoreUnused (url);
        
        if(result.wasOk()){
            this->videoSize = this->player_with_controller.getVideoNativeSize();
            KLib().showRectangle<int>(this->videoSize);
            this->aspectRatio = (float)this->videoSize.getWidth() / (float)this->videoSize.getHeight();
            setNeedController(true);
            
            int w,h;
            if(this->videoSize.getWidth() > 320)
            {
                w = 320;
                h = (int)((float)w / this->aspectRatio);
            }else{
                w = this->videoSize.getWidth();
                h = this->videoSize.getHeight();

            }
            this->player.setBounds(0,0,w,h);
            this->player_with_controller.setBounds(0,0,w,h);
            addAndMakeVisible(this->player_with_controller);
            removeChildComponent(&this->openButton);
            
            setSize(w,h);
            
            this->isVideoOpenedFlag = true;
            
            if(videoLoadCompleted != nullptr)
            {
                videoLoadCompleted();
            }
        }else{
            this->isVideoOpenedFlag = false;
            removeChildComponent(&this->player);
            removeChildComponent(&this->player_with_controller);
            KLib().showConnectionErrorMessage("Could not load the video file of "+url.getSubPath());
        }
    }
    // --------------------------------------------------
    // switch navi on off
    void setNeedController(bool flag)
    {
        this->isController = flag;
        if(flag){
            removeChildComponent(&this->player);
            addAndMakeVisible(this->player_with_controller);
        }else{
            removeChildComponent(&this->player_with_controller);
            addAndMakeVisible(this->player);
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
    
private:
    VideoComponent player;
    
    // no idea how to switch Navi Controlelr on off, so that we prepare two objects, one is with and another is without.
    VideoComponent player_with_controller;
        
    Rectangle<int> videoSize;
    float aspectRatio = 0.0;
    bool isVideoLoaded = false;
    String path;
    File movieFile;
    
    bool isController = false;
    bool isVideoOpenedFlag = false;
    
    TextButton openButton;
    
    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoPlayer)
    JUCE_LEAK_DETECTOR (IRVideoPlayer)
    
};
#endif /* IRVideoPlayer_hpp */
