
#ifndef IRImageViewer_h
#define IRImageViewer_h

#include "JuceHeader.h"
#include "IRFoundation.h"
#include "IRImageLoader.hpp"





class IRImageViwer : public Component, private ChangeListener, public ChangeBroadcaster
{
    
public:
    
    IRImageViwer()
    {
        setSize(100,100);
        addAndMakeVisible(&this->openButton);
        this->openButton.setButtonText("open image");
        
        // ===========================================================================
        // system appearance
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
        this->openButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.fundamental);
        this->setEnabled(true);
        this->openButton.onClick = [this]{ openFile(); };
        
        this->imgLoader.addChangeListener(this);
        
    }
    
    ~IRImageViwer()
    {
        
    }
    // ------------------------------------------------------------

    void paint(Graphics& g) override
    {
        
        if(!this->img.isNull())
        {
            g.drawImageTransformed (*this->imgRef,
                                    AffineTransform::scale (getWidth()  / (float) this->imgRef->getWidth(),
                                                            getHeight() / (float) this->imgRef->getHeight()), false);
           // g.drawImageAt(this->img, 0,0);
            
        }
        
    }
    // ------------------------------------------------------------
    void resized() override
    {        
        
        if(this->imgRef != nullptr)
            this->img = this->imgLoader.getData()->rescaled(getWidth(), getHeight());

        this->openButton.setBounds(0,0,getWidth(),getHeight());

    }
    
    // ------------------------------------------------------------
    void openFile()
    {
        this->imgLoader.open();
        // receive pointer of the image file from FileManager
        this->imgRef = this->imgLoader.getData();

    }
    
    void openFile(String filePath)
    {
        this->imgLoader.open(filePath);
        // receive pointer of the image file from FileManager
        this->imgRef = this->imgLoader.getData();
    }
    // ------------------------------------------------------------
    void changeListenerCallback (ChangeBroadcaster* source)override
    {
        if(source == &this->imgLoader)
        {
            if(this->imgLoader.isFileLoadCompleted)
            {
                removeChildComponent(&this->openButton);
            
                //inform parent the size change
                sendChangeMessage();
            }
        }
    }
    // ------------------------------------------------------------
    double getAspectRatio() const { return this->imgLoader.getAspectRatio(); }
    // ------------------------------------------------------------
    String getFilePath() const { return this->imgLoader.getPath(); }
    void setFilePath(String path) { this->imgLoader.open(path); }
    
private:
    
    IRImageLoader imgLoader;
    
    // this member contains a reference from FileManager and should not be modified!
    Image* imgRef = nullptr;
    
    // When you draw Image with or withour modification (such as rescaling the size), use this pointer
    Image img;
    
    TextButton openButton;

    
    
};




#endif /* IRImageViewer_h */




