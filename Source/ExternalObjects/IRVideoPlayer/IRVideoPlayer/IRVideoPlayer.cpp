//
//  IRVideoPlayer.cpp
//  IRiMaS
//
//  Created by Keitaro on 15/01/2020.
//
#include "IRVideoPlayer.hpp"


IRVideoPlayer::IRVideoPlayer(IRNodeObject* parent, IRStr* str, bool withOpenButton) :
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

IRVideoPlayer::~IRVideoPlayer()
{
    this->player.reset();
}

// --------------------------------------------------

void IRVideoPlayer::resized()
{
    if(this->player != nullptr)
        this->player->setBounds(getLocalBounds());
  
    this->openButton.setBounds(getLocalBounds());
}

void IRVideoPlayer::paint(Graphics &g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
}

// --------------------------------------------------

void IRVideoPlayer::openFile()
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

void IRVideoPlayer::openFile(File file, bool isCallback)
{
    auto url = URL (file);
    this->videoFilePath = file.getFullPathName().toStdString();
    this->movieFile = file;
    std::cout << url.getFileName() << std::endl;
    this->player->loadVideo(url);
}
// --------------------------------------------------

void IRVideoPlayer::videoLoadCompleteAction()
{
    
    removeChildComponent(&this->openButton);
    addAndMakeVisible(this->player.get());
            
    if(this->videoLoadCompleted != nullptr)
        this->videoLoadCompleted();
    
    startAnimation();

}
// --------------------------------------------------
