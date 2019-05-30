//
//  IRVideoThumnail.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 04/02/2019.
//

#ifndef IRVideoThumnail_hpp
#define IRVideoThumnail_hpp

/*
#include <opencv.hpp>
#include <opencv2/opencv.hpp>
#include <highgui.hpp>

#include "IRVideoDataLoader.h"

class IRVideoThumbnail : public Component
{
public:
    IRVideoThumbnail()
    {
        this->openButton.setButtonText("open a movie file");
        this->openButton.setColour(TextButton::buttonColourId,
                                   SYSTEMCOLOUR.fundamental);
        this->openButton.setColour(TextButton::textColourOffId,
                                   Colour((uint8)255, (uint8)255, (uint8)255, (uint8)255));
        this->openButton.setColour(TextButton::textColourOnId,
                                   Colour((uint8)255, (uint8)255, (uint8)255, (uint8)255));
        this->openButton.onClick =[this] { openVideoFile(); };
        addAndMakeVisible(this->openButton);
    }
    ~IRVideoThumbnail()
    {
        
    }
    // --------------------------------------------------
    void resized() override
    {
        this->openButton.setBounds(getLocalBounds());
        this->thumbWidth = (float)getWidth() / (float)this->thumbCount;
    }
    void paint(Graphics &g) override;
    // --------------------------------------------------
    void openVideoFile();
    // --------------------------------------------------
    void getCameraCapture();
    // --------------------------------------------------
    void createThumbnail(int num, cv::VideoCapture* input = nullptr);
    // --------------------------------------------------
    void loadThumbnail(std::string directoryPath);
    // --------------------------------------------------
    Image loadImage(String filePath);
    // --------------------------------------------------
    std::vector<Image> getThumbImages() {return this->thumbImages; }
    // --------------------------------------------------
    Point<int> getThumbSize() const { return this->thumbSize; }
    
    
    // --------------------------------------------------
    // callback
    std::function<void()> thumbLoadCompleted;
    
    
private:
    
    // ui
    TextButton openButton;
    float thumbWidth;
    
    
    // --------------------------------------------------
    
    IRVideoDataLoader loader;
    IRVideoDataLoader::videoStr videoInfo;
    cv::Mat frame;
    cv::VideoCapture* cap;
        
    int thumbCount = 10;
    Point<int> thumbSize;
    
    std::vector<Image> thumbImages;
    
    
   

    
    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoThumbnail)
    
};
*/
#endif /* IRVideoThumnail_hpp */
