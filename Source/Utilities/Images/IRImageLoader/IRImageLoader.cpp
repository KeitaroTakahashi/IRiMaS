
#include "IRImageLoader.hpp"

IRImageLoader::IRImageLoader(IRNodeObject* parent)
{
    this->parent = parent;
}


IRImageLoader::IRImageLoader(String path)
{
    this->path = path;
}


IRImageLoader::~IRImageLoader()
{
    // remove pointer
    if(this->imgData != nullptr)
        FILEMANAGER.discardFilePtr(IRFileType::IRIMAGE, this->imgData, this->parent, this->file);
}


void IRImageLoader::open()
{
    this->isFileLoadCompleted = false;
    
    FileChooser chooser("Select an image file...",
                        {},
                        "*.png, *.jpg, *.jpeg");
    if (chooser.browseForFileToOpen())
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
            
            this->imgData = static_cast<DataAllocationManager<IRImage>*>(FILEMANAGER.getFilePtr(IRFileType::IRIMAGE, file, this->parent));
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
        
        this->imgData = static_cast<DataAllocationManager<IRImage>*>(FILEMANAGER.getFilePtr(IRFileType::IRIMAGE, file, this->parent));
        //loadImage(pathToOpen);

        this->isFileLoadCompleted = true;
    }else{
        this->isFileOpened = false;
        this->imgData = nullptr;
    }
    
    sendChangeMessage();
}


void IRImageLoader::resized(int w, int h)
{

}

Point<int> IRImageLoader::sizeFix()
{
    // image size
    int img_w = getData()->getImageData().getWidth();
    int img_h = getData()->getImageData().getHeight();
    
    this->aspectRatio = (double) img_w / (double) img_h;
    std::cout << "aspectRatio = " <<this->aspectRatio << " : w = " << img_w << std::endl;
    if (img_w > this->maxWidth)
    {
        img_w = this->maxWidth;
        img_h = (double)this->maxWidth / this->aspectRatio;
    }
    
    if (img_h > this->maxHeight)
    {
        img_w = (double)this->maxHeight * this->aspectRatio;
        img_h = this->maxHeight;
    }
    
    return Point<int> (img_w, img_h);
}


double IRImageLoader::getAspectRatio() const
{
    return this->aspectRatio;
}


IRImage* IRImageLoader::getData()
{
    return this->imgData->getData();
}


String IRImageLoader::getPath() const
{
    return this->path;
}




