//
//  IRImageLoader.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 24/08/2018.
//

#ifndef IRImageLoader_hpp
#define IRImageLoader_hpp

#include "JuceHeader.h"

class IRImageLoader : public ChangeBroadcaster
{
public:
    IRImageLoader(String path)
    {
        this->path = path;
        
    }
    IRImageLoader()
    {
        
    }
    ~IRImageLoader() {}
    // ------------------------------------------------------------

    void open()
    {
        this->isFileLoadCompleted = false;
        
        FileChooser chooser("Select an image file...",
                            File::nonexistent,
                            "*.png, *.jpg, *.jpeg");
        if(chooser.browseForFileToOpen())
        {
            auto file = chooser.getResult();
            auto p = file.getFullPathName();
            this->path = p;
        
            String pathToOpen;
            pathToOpen.swapWith(p);
            
            if(pathToOpen.isNotEmpty())
            {
                this->isFileOpened = true;
                loadImage(pathToOpen);
                this->isFileLoadCompleted = true;
            }else{
                this->isFileOpened = false;
            }
            
            sendChangeMessage();
        }
    }
    
    void open(String pathToOpen)
    {
        this->isFileLoadCompleted = false;
        this->path = pathToOpen;
        pathToOpen.swapWith(this->path);
        
        if(pathToOpen.isNotEmpty())
        {
            this->isFileOpened = true;
            loadImage(pathToOpen);
            this->isFileLoadCompleted = true;
        }else{
            this->isFileOpened = false;
        }
        
        sendChangeMessage();
        
    }
    // ------------------------------------------------------------

    void loadImage(String filePath)
    {
        File file (filePath);
        this->imgData = ImageFileFormat::loadFrom(file);
        
        if(this->imgData.getWidth() == 0 || this->imgData.getHeight() == 0)
        {
            std::cout << "Error : problem occurs during loading the file " << filePath << std::endl;
        }else{
            std::cout << "file loaded!! width = " <<this->imgData.getWidth() << ", " << this->imgData.getHeight() << " : " << filePath << std::endl;

        }
        
        // fix image size to avoid erroneous result
        sizeFix();
    }
    
   
    
    void resized(int w, int h)
    {
        this->bindImage = this->imgData.rescaled(w, h);

    }
    
    void sizeFix()
    {
        // image size
        int img_w = this->imgData.getWidth();
        int img_h = this->imgData.getHeight();
                
        this->aspectRatio = (double) img_w / (double) img_h;

        if(img_w > this->maxWidth)
        {
            img_w = this->maxWidth;
            img_h = (double)this->maxWidth / this->aspectRatio;
        }else if(img_h > this->maxHeight)
        {
            img_w = (double)this->maxHeight * this->aspectRatio;
            img_h = this->maxHeight;
        }
    }
    
    double getAspectRatio() const { return this->aspectRatio; }

    // ------------------------------------------------------------
    Image& getData() { return this->imgData; }
    String getPath() const { return this->path; }
    bool isFileLoadCompleted = false;
    bool isFileOpened = false;
    
private:
    String path;
    Image imgData;
    
    Image bindImage;
    
    double aspectRatio = 0;
    
    // define max size of the loaded picture to avoid erroneous size change
    int maxWidth = 3000;
    int maxHeight = 3000;

};



#endif /* IRImageLoader_hpp */
