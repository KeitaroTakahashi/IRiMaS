
#ifndef IRImageLoader_hpp
#define IRImageLoader_hpp

#include "JuceHeader.h"

#include "IRFileManager.hpp"
#include "IRNodeObject.hpp"


class IRImageLoader : public ChangeBroadcaster
{
public:
    
    IRImageLoader(IRNodeObject* parent);
    IRImageLoader(String path);
    ~IRImageLoader();

    void open();
    void open(String pathToOpen);
    
    void resized(int w, int h);
    Point<int> sizeFix();

    double getAspectRatio() const;
    
    IRImage* getData();
    String getPath() const;
    
    bool isFileLoadCompleted = false;
    bool isFileOpened = false;
    
private:
    
    IRNodeObject* parent;
    
    String path;
    File file;
    
    DataAllocationManager<IRImage>* imgData = nullptr;
    //IRImage* imgData = nullptr;
        
    double aspectRatio = 0;
    
    // define max size of the loaded picture to avoid erroneous size change
    int maxWidth = 300;
    int maxHeight = 300;
    
    IRFileManager& FILEMANAGER = singleton<IRFileManager>::get_instance();
    
};



#endif /* IRImageLoader_hpp */



