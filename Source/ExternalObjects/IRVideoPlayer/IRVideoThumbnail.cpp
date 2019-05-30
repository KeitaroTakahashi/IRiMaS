//
//  IRVideoThumbnail.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 04/02/2019.
//

#include "IRVideoThumbnail.hpp"
/*
void IRVideoThumbnail::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.background);
    if(thumbImages.size() > 0)
    {
        int index = 0;
        for(auto img : this->thumbImages)
        {
            float x = ((float)index * this->thumbWidth);
            float h = (float)img.getHeight();
            g.drawImage(img, Rectangle<float>(x,0,this->thumbWidth,h));
            
            index++;
        }
    }
}

void IRVideoThumbnail::openVideoFile()
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
        createThumbnail(this->thumbCount);
        
        loadThumbnail("/Users/keitaro/Desktop/frames");
        
        if(this->thumbImages.size()>0){
            this->thumbSize = Point<int>(this->thumbImages[0].getWidth(),
                                        this->thumbImages[0].getHeight());
            
            
            if(this->thumbLoadCompleted != nullptr) thumbLoadCompleted();

        }else{
            this->thumbSize = Point<int>(0,0);
        }
        removeChildComponent(&this->openButton);
        

        
        repaint();
    }
}


void IRVideoThumbnail::getCameraCapture()
{
    *this->cap >> this->frame;
    if (this->frame.empty())
    {
        std::cout << "Error : Could not get frame" << std::endl;
    }
    
}


void IRVideoThumbnail::createThumbnail(int num, cv::VideoCapture* input)
{
    
    // use input if given, otherwise use member cap
    cv::VideoCapture* video;
    if (input != nullptr)
    {
        video = input;
    }
    else
    {
        video = this->cap;
    }
    
    // check if videoInfo is loaded.
    int fc = this->videoInfo.frameCount;
    if (fc == 0)
    {
        std::cout << "Error : createThumbnail() : frameCount is 0" << std::endl;
        return;
    }
    
    // frame count interval to capture thumbnail
    int interval = fc / num;
    
    std::vector<cv::Mat> thumb;
    int w = this->videoInfo.width;
    int h = this->videoInfo.height;
    
    int i;
    
    int rw = 100;
    int rh = h * rw / w;
    cv::Mat buf = cv::Mat (rh, rw, CV_8U, new cv::Scalar(4));
    
    // for each frame
    for (i = 0; i < num; i++)
    {
        std::cout << "writing thumbnail of " << i << " : size " << rw << std::endl;
        int index = i*interval; // calc interval frame index
        video->set(cv::CAP_PROP_POS_FRAMES,index); // set index to read
        *video >> buf; // read a frame from vide
        cv::resize(buf,buf, cv::Size(rw,rh)); // resize
        thumb.push_back(buf); // store
        cv::imwrite("/Users/keitaro/Desktop/frames/im_" + std::to_string(rw) + "_" + std::to_string(i) + ".png", buf);
    }
}


void IRVideoThumbnail::loadThumbnail(std::string directoryPath)
{
    File file(directoryPath);
    Array<File> files = file.findChildFiles(File::TypesOfFileToFind::findFiles, false);
    
    for (auto f : files)
    {
        if (f.getFileExtension() == ".png")
        {
            this->thumbImages.push_back(loadImage(f.getFullPathName()));
        }
    }
    
}


Image IRVideoThumbnail::loadImage(String filePath)
{
    File file (filePath);
    Image imgData = ImageFileFormat::loadFrom(file);
    
    if (imgData.getWidth() == 0 || imgData.getHeight() == 0)
    {
        std::cout << "Error : problem occurs during loading the file " << filePath << std::endl;
    }
    else
    {
        std::cout << "file loaded!! width = " << imgData.getWidth() << ", " << imgData.getHeight() << " : " << filePath << std::endl;
    }
    return imgData;
}

*/


