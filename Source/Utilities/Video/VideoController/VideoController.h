//
//  VideoController.h
//  IRiMaS
//
//  Created by Keitaro on 24/10/2019.
//

#ifndef VideoController_h
#define VideoController_h

#include "JuceHeader.h"
#include "IRVideoAnnotaterWindow.h"
#include "IRVideoPlayerObject.hpp"

class VideoController : public Component,
public IRStrComponent,
public ChangeBroadcaster,
public ChangeListener
{
public:
    // ==================================================

    enum VideoControllerStatus
    {
        OpenVideoFile,
        OpenVideoAnnotater,
        CloseVideoAnnotater,
        WidthChange,
        HeightChange,
        W_H_Sync,
        NONE
    };
    // ==================================================

    VideoController(IRStr * str, IRVideoAnnotaterObject* videoPlayerObject) : IRStrComponent(str),
    videoPlayerObject(videoPlayerObject)
    {
        addAndMakeVisible(&this->openVideoButton);
        this->openVideoButton.setButtonText("Open Video File");
        this->openVideoButton.onClick = [this]{ OpenVideoFileAction(); };
        
        addAndMakeVisible(&this->openVideoAnnotatorButton);
        this->openVideoAnnotatorButton.setButtonText("Open Video Annotater");
        this->openVideoAnnotatorButton.onClick = [this]{ OpenVideoAnnotaterAction(); };

    }
    
    ~VideoController()
    {
        this->videoAnnotater.reset();
    }
    // ==================================================

    void resized() override
    {
        int y = 0;
       // int yIncrement = 30;
        int yBigIncrement = 30;
        
        int menuWidth = getWidth() - 20;
        
        this->openVideoButton.setBounds(10, y, menuWidth, 30);
        
        y += yBigIncrement + 10;
        this->openVideoAnnotatorButton.setBounds(10, y, menuWidth, 30);
    }
    
    void paint(Graphics& g) override
    {
        
    }
    // ==================================================

    void OpenVideoFileAction()
    {
        this->status = OpenVideoFile;
        sendChangeMessage();
    }
    // ==================================================

    void OpenVideoAnnotaterAction()
    {
        // store the original bounds of the videoPlayerObject

        if(this->videoAnnotater.get() == nullptr)
        {
            this->videoAnnotater.reset(new IRVideoAnnotaterWindow(getStr(),
                                                                  "Video Annotater",
                                                                  Rectangle<int> (10, 10, 1000, 800),
                                                                  this->videoPlayerObject));
            

        }
        this->videoAnnotater->closeButtonPressedCallback = [this]{ CloseVideoAnnotaterAction(); };

        this->videoAnnotater->open();
        this->videoAnnotater->toFront(true);
        
        this->status = OpenVideoAnnotater;
        sendChangeMessage();
    }
    
    void CloseVideoAnnotaterAction()
    {
        this->status = CloseVideoAnnotater;
        sendChangeMessage();
    }
    
   // ==================================================
    void updateAnnotater()
    {
        if(this->videoAnnotater.get() != nullptr)
        {
            this->videoAnnotater->uodateAnnotater();
        }
    }
    
    void updateParentVideoPlayerObject()
    {
        if(this->videoAnnotater.get() != nullptr)
        {
           this->videoAnnotater->updateParentVideoPlayerObject();
        }
    }
    
    // ==================================================
  
    // ==================================================

    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }
    // ==================================================

    VideoControllerStatus getStatus() const { return this->status; }
    // ==================================================
    // ==================================================
    // ==================================================
    // ==================================================


private:
    
    TextButton openVideoButton;
    TextButton openVideoAnnotatorButton;
    // ==================================================

    IRVideoAnnotaterObject* videoPlayerObject = nullptr;
    
    std::shared_ptr<IRVideoAnnotaterWindow> videoAnnotater;

    // ==================================================
    

    VideoControllerStatus status = NONE;
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoController)

};
#endif /* VideoController_h */
