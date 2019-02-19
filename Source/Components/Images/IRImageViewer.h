//
//  IRImageViewer.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 24/08/2018.
//

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
        g.drawImageAt(this->img, 0,0);
        
    }
    // ------------------------------------------------------------
    void resized() override
    {        
        if(!this->img.isNull())
            this->img = this->imgLoader.getData().rescaled(getWidth(), getHeight());
        this->openButton.setBounds(0,0,getWidth(),getHeight());

    }
    
    // ------------------------------------------------------------
    void openFile()
    {
        std::cout << "open\n";
        this->imgLoader.open();
        this->img = this->imgLoader.getData();
    }
    
    void openFile(String filePath)
    {
        std::cout << "open\n";
        this->imgLoader.open(filePath);
        this->img = this->imgLoader.getData();
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
    Image img;
    
    TextButton openButton;

    
    
};

#endif /* IRImageViewer_h */
