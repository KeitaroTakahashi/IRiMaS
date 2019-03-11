
#ifndef IRImageViewer_h
#define IRImageViewer_h

#include "JuceHeader.h"
#include "IRFoundation.h"
#include "IRImageLoader.hpp"





class IRImageViewer : public Component, private ChangeListener, public ChangeBroadcaster
{
    
public:
    
    IRImageViewer();
    
    ~IRImageViewer();

    void paint(Graphics& g) override;
    void resized() override;
    
    void openFile();
    void openFile(String filePath);
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    double getAspectRatio() const;

    String getFilePath() const;
    void setFilePath(String path);
    
private:
    
    IRImageLoader imgLoader;
    
    // this member contains a reference from FileManager and should not be modified!
    IRImage* imgRef = nullptr;
    
    // When you draw Image with or withour modification (such as rescaling the size), use this pointer
    Image img;
    
    TextButton openButton;

    
};




#endif /* IRImageViewer_h */




