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
                           IRVideoAnnotaterObject* videoPlayerObject) :
    DocumentWindow(name, Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), DocumentWindow::allButtons),
    IRStrComponent(str),
    videoPlayerObject(videoPlayerObject)

    {
        setUsingNativeTitleBar(true);
        setResizable(true, true);
        setName(name);
        setBounds(frameRect);
        
        Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
        setResizeLimits(1000, 800, r.getWidth(), r.getHeight());
        
        this->videoAnnotater.reset(new IRVideoAnnotater(str, videoPlayerObject));
        this->videoAnnotater->setBounds(0, 0, frameRect.getWidth(), frameRect.getHeight());
        setContentOwned(this->videoAnnotater.get(), true);

        addKeyListener(this->videoAnnotater.get());
    }
    
    ~IRVideoAnnotaterWindow()
    {
        this->videoAnnotater.reset();
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
        this->videoAnnotater->removeVideoPlayerObject();
        setVisible(false);
    }
    
    void open()
    {
        this->videoAnnotater->bindVideoPlayerObject();
        setVisible(true);
    }
    // ==================================================
    
    void uodateAnnotater()
    {
        this->videoAnnotater->updateVideoPlayerOfThis();
    }
    
    void updateParentVideoPlayerObject()
    {
        this->videoAnnotater->updateVideoPlayerOfWorkspace();
    }
    // ==================================================
    void setEventModifiedCallback(std::function<void()> callback)
    {
        this->videoAnnotater->setEventModifiedCallback(callback);
    }

   
    // ==================================================

private:
    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        
    }

    // ==================================================
    
    IRVideoPlayerObject* videoPlayerObject = nullptr;
    // ==================================================
    // ==================================================
    
    std::shared_ptr<IRVideoAnnotater> videoAnnotater;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRVideoAnnotaterWindow)

};

#endif /* IRVideoAnnotaterWindow_h */
