//
//  IRImageLoader.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 24/08/2018.
//

#include "IRImageLoader.hpp"

IRImageLoader::IRImageLoader(String path)
{
    this->path = path;
}

IRImageLoader::IRImageLoader()
{
}

IRImageLoader::~IRImageLoader()
{
    if(this->file.getFullPathName().length() > 0)
        FILEMANAGER.discardFilePtr(this, this->file);
}

void IRImageLoader::open()
{
    this->isFileLoadCompleted = false;
    
    FileChooser chooser("Select an image file...",
                        File::nonexistent,
                        "*.png, *.jpg, *.jpeg");
    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        this->file = file;
        auto p = file.getFullPathName();
        this->path = p;
        
        String pathToOpen;
        pathToOpen.swapWith(p);
        
        if(pathToOpen.isNotEmpty())
        {
            this->isFileOpened = true;
            //loadImage(pathToOpen);
            
            
            this->imgData = static_cast<Image*>(FILEMANAGER.getFilePtr(IRFileType::IRIMAGE, file));
            //test
            //std::cout << "IRFileManager create data pointer = " << FILEMANAGER.getFilePtr(IRFileType::IRIMAGE, file) << std::endl;
            
            
            this->isFileLoadCompleted = true;
        }else{
            
            if(!this->isFileOpened)
                this->imgData = nullptr;
            this->isFileOpened = false;
        }
        
        sendChangeMessage();
    }
}

void IRImageLoader::open(String pathToOpen)
{
    this->isFileLoadCompleted = false;
    this->path = pathToOpen;
    pathToOpen.swapWith(this->path);
    
    if(pathToOpen.isNotEmpty())
    {
        this->isFileOpened = true;
        
        File file(this->path);
        
        //loadImage(pathToOpen);
        this->imgData = static_cast<Image*>(FILEMANAGER.getFilePtr(IRFileType::IRIMAGE, file));

        this->isFileLoadCompleted = true;
    }else{
        this->isFileOpened = false;
        this->imgData = nullptr;
    }
    
    sendChangeMessage();
    
    }
