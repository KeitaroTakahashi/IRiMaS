//
//  IRVideoAnnotaterWindow.h
//  IRiMaS
//
//  Created by Keitaro on 07/12/2019.
//

#ifndef IRVideoAnnotaterWindow_h
#define IRVideoAnnotaterWindow_h

#include "IRVideoAnnotater.hpp"

class IRVideoAnnotaterWindow : public DocumentWindow,
public IRStrComponent,
public ChangeListener,
public ChangeBroadcaster
{
public:
    IRVideoAnnotaterWindow(IRStr* str,
                           String name,
                           Rectangle<int>frameRect,
                           IRVideoAnnotaterObject2* videoPlayerObject) :
    DocumentWindow(name, Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), DocumentWindow::allButtons),
    IRStrComponent(str),
    videoPlayerObject(videoPlayerObject),
    videoAnnotater(str, videoPlayerObject)
    {
        setUsingNativeTitleBar(true);
        setResizable(true, true);
        setName(name);
        setBounds(frameRect);
        
        Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
        setResizeLimits(1000, 800, r.getWidth(), r.getHeight());
        
        this->videoAnnotater.setBounds(0, 0, frameRect.getWidth(), frameRect.getHeight());
        setContentOwned(&this->videoAnnotater, true);
        addKeyListener(&this->videoAnnotater);
        
        this->videoAnnotater.closeAnnotationWindowCallback = [this] { close(); };
        
        this->videoAnnotater.bindVideoPlayerObject();

    }
    
    ~IRVideoAnnotaterWindow()
    {
        
    }
    // ==================================================
    std::function<void()> closeButtonPressedCallback;
    void closeButtonPressed() override
    {
        if(this->closeButtonPressedCallback != nullptr)
            this->closeButtonPressedCallback();
        
        close();
    }
    
    // ==================================================
    
    void close()
    {
        //this->videoAnnotater.removeVideoPlayerObject();
        setVisible(false);
    }
    
    void open()
    {
        //this->videoAnnotater.bindVideoPlayerObject();
        setVisible(true);
        
        this->videoAnnotater.openAnnotaterWindowAction();
    }
    // ==================================================
    
    void uodateAnnotater()
    {
        this->videoAnnotater.updateVideoPlayerOfThis();
    }
    
    void updateParentVideoPlayerObject()
    {
        this->videoAnnotater.updateVideoPlayerOfWorkspace();
    }
    // ==================================================
    /*
    void setEventModifiedCallback(std::function<void()> callback)
    {
        this->videoAnnotater.setEventModifiedCallback(callback);
    }

   */
    // ==================================================

    IRVideoAnnotater* getVideoAnnotaterComponent() { return &this->videoAnnotater; }
    
    // ==================================================
    // ==================================================

private:
    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        
    }

    // ==================================================
    
    IRVideoAnnotaterObject2* videoPlayerObject = nullptr;
    // ==================================================
    // ==================================================
    
    IRVideoAnnotater videoAnnotater;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRVideoAnnotaterWindow)

};

#endif /* IRVideoAnnotaterWindow_h */
