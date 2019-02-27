//
//  IRCVVideoPlayer.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 07/02/2019.
//

#ifndef IRCVVideoPlayer_hpp
#define IRCVVideoPlayer_hpp

#include <opencv.hpp>
#include <opencv2/opencv.hpp>
#include <highgui.hpp>

#include "KPixelDataConvertor.h"
#include "IRVideoDataLoader.h"

#include "KPixelDataConvertor.h"
#include "KeAnimationComponent.h"

class IRCVVideoPlayer : public Component,
public KeAnimationComponent
{
public:
    IRCVVideoPlayer()
    {
        //createButton();
        //addAndMakeVisible(this->openButton);
        
        this->loader.OpenVideoFile("/Users/keitaro/program/JUCE/sources/06August2018-01-720.mov");
        this->cap = this->loader.getCapture();
        
        getCameraCapture();

        setFps(33);
        startAnimation();
        
        cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.

        
    }
    ~IRCVVideoPlayer()
    {
        
    }
    // --------------------------------------------------
    void resized() override
    {
        this->openButton.setBounds(getLocalBounds());

    }
    void paint(Graphics& g) override
    {
        
        //g.fillAll(Colours::blue);
        
        getCameraCapture();

        g.drawImage(*this->img, Rectangle<float>(0,0,getWidth(),getHeight()));
    }
    // --------------------------------------------------
    void updateAnimationFrame() override
    {
        repaint();
    }
    // --------------------------------------------------

    void createButton();
    // --------------------------------------------------
    void openVideoFile();
    // --------------------------------------------------
    void getCameraCapture();
    // --------------------------------------------------
    void MatToImage(Image* img, cv::Mat *m);

private:
    
    // ui
    TextButton openButton;
    float thumbWidth;
    
    Image* img = nullptr;
    
    IRVideoDataLoader loader;
    IRVideoDataLoader::videoStr videoInfo;
    cv::Mat frame;
    cv::VideoCapture* cap;
    
    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRCVVideoPlayer)
};

#endif /* IRCVVideoPlayer_hpp */
