//
//  IRVideoDataLoader.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 04/02/2019.
//

#ifndef IRVideoDataLoader_h
#define IRVideoDataLoader_h
//#include <opencv.hpp>
//#include <opencv2/opencv.hpp>
//#include <highgui.hpp>

/*
#include "IRFoundation.h"

class IRVideoDataLoader
{
public:
    
    struct videoStr
    {
        int width;      // with of the frames in the video stream
        int height;      // height of the frames in the video stream
        
        int framePos;    // frame index
        float relativeFramePos; // 0= start, 1 = end
        
        float fps;      // frame rate per second
        int frameCount; // number of frames in the video file
        
        float length; // length of the video file in second.
        
        videoStr(int width, int height,
                 int framePos, float relativeFramePos,
                 float fps, int frameCount,
                 float length)
        {
            this->width = width;
            this->height = height;
            this->framePos = framePos;
            this->relativeFramePos = relativeFramePos;
            this->fps = fps;
            this->frameCount = frameCount;
            this->length = length;
        }
        // initialize
        videoStr()
        {
            this->width = 0;
            this->height = 0;
            this->framePos = 0;
            this->relativeFramePos = 0;
            this->fps = 0;
            this->frameCount = 0;
            this->length = 0;
        }
        
    };
    
    IRVideoDataLoader()
    {
        
    }
    
    ~IRVideoDataLoader()
    {
        if(this->cap != nullptr)
        {
            delete this->cap;
        }
    }
    
    // --------------------------------------------------

    videoStr getVideoInfo()
    {
        std::cout << "getVideoInfo\n";
        
        if(this->cap == nullptr){
            std::cout << "cap is null\n";
            return videoStr();
        }
        // if video file is already opened.
        if(this->cap->isOpened()){

            int width = this->cap->get(cv::CAP_PROP_FRAME_WIDTH);
            int height = this->cap->get(cv::CAP_PROP_FRAME_HEIGHT);
            int frameCount = this->cap->get(cv::CAP_PROP_FRAME_COUNT);
            float fps = this->cap->get(cv::CAP_PROP_FPS);
            float length = (float)frameCount / fps;
            
            std::cout << "frame size = (" << width << ", " << height << ") : fps = " << fps << " : frameCount = " << frameCount << " : length = "<< length << std::endl;

            return videoStr(
                               width,
                               height,
                               this->cap->get(cv::CAP_PROP_POS_FRAMES),
                               this->cap->get(cv::CAP_PROP_POS_AVI_RATIO),
                               fps,
                               frameCount,
                               length
                             );
            
            
            
        }else{
            std::cout << "Error : could not find cap. cap is empty" << std::endl;
            // if no video file opened, then return empty videoInfo
            return videoStr();
        }
    }
    
    // --------------------------------------------------
    void OpenVideoFile(std::string filePath)
    {
        this->cap = new cv::VideoCapture(filePath);
        if(this->cap->isOpened())
        {
            this->videoInfo = getVideoInfo();
            std::cout << "video file opened successfuly! " << filePath << std::endl;
        }else{
            std::cout << "Error : Could not open video file of " << filePath << std::endl;
            this->cap = nullptr;
        }
    }
    // --------------------------------------------------
    cv::VideoCapture* getCapture() { return this->cap; }
    

private:
    
    videoStr videoInfo;
    
    cv::Mat frame;
    cv::VideoCapture* cap = nullptr;
    
};*/

#endif /* IRVideoDataLoader_h */
