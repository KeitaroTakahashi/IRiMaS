
#ifndef IRImageLoader_hpp
#define IRImageLoader_hpp

#include "JuceHeader.h"

#include "IRFileManager.hpp"





class IRImageLoader : public ChangeBroadcaster
{
public:
    
    IRImageLoader();
    IRImageLoader(String path);
    ~IRImageLoader();

    void open();
    void open(String pathToOpen);
    
    void resized(int w, int h);
    
    void sizeFix();
    
    double getAspectRatio() const;
    
    Image* getData();
    String getPath() const;
    
    
    bool isFileLoadCompleted = false;
    bool isFileOpened = false;
    
    
private:
    
    String path;
    File file;
    Image* imgData = nullptr;
    
    Image bindImage;
    
    double aspectRatio = 0;
    
    // define max size of the loaded picture to avoid erroneous size change
    int maxWidth = 3000;
    int maxHeight = 3000;
    
    
    IRFileManager& FILEMANAGER = singleton<IRFileManager>::get_instance();
    
};



#endif /* IRImageLoader_hpp */



