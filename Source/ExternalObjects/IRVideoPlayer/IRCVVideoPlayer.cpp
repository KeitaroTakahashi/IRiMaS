//
//  IRCVVideoPlayer.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 07/02/2019.
//

#include "IRCVVideoPlayer.hpp"

void IRCVVideoPlayer::createButton()
{
    this->openButton.setButtonText("open a movie file");
    this->openButton.setColour(TextButton::buttonColourId,
                               SYSTEMCOLOUR.fundamental);
    this->openButton.setColour(TextButton::textColourOffId,
                               Colour((uint8)255, (uint8)255, (uint8)255, (uint8)255));
    this->openButton.setColour(TextButton::textColourOnId,
                               Colour((uint8)255, (uint8)255, (uint8)255, (uint8)255));
    this->openButton.onClick =[this] { openVideoFile(); };
}
// --------------------------------------------------

void IRCVVideoPlayer::openVideoFile()
{
    FileChooser chooser("Select a video file to open...",
                        {},
                        "*.mov, *.mp4, *.m4v");
    
    FilenameComponent co {"movie", {}, true, false, false, "*", {}, "(choose a video file to play)"};
    
    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto path = file.getFullPathName();
        auto url = URL (file);
        
        std::cout << url.getFileName() << std::endl;
        
        this->loader.OpenVideoFile(path.toStdString());
        this->cap = this->loader.getCapture();
        this->videoInfo = this->loader.getVideoInfo();
        
        removeChildComponent(&this->openButton);

    }
    repaint();
}
// --------------------------------------------------
void IRCVVideoPlayer::getCameraCapture()
{
    
    if(getWidth() > 0 && getHeight() > 0){
        *this->cap >> this->frame;
        cv::Mat resizedFrame;
        cv::resize(this->frame, resizedFrame, cv::Size(getWidth(), getHeight()));
        
        cv::imshow("Display window", resizedFrame);

        this->img = KPixelDataConvertor().Mat2Image_RGB(&resizedFrame);
        
        if(this->frame.empty()){
           
            stopAnimation();
         printf("Error : Could not get frame\n");
        }
    }
}
// --------------------------------------------------

void IRCVVideoPlayer::MatToImage(Image* img, cv::Mat *m)
{
    
}


// --------------------------------------------------
