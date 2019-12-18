//
//  VideoAnnotationMenuWindow.h
//  IRiMaS
//
//  Created by Keitaro on 10/12/2019.
//

#ifndef VideoAnnotationMenuWindow_h
#define VideoAnnotationMenuWindow_h


#include "VideoAnnotationMenuComponent.h"

class VideoAnnotationMenuWindow : public DocumentWindow,
public IRStrComponent,
public ChangeListener,
public ChangeBroadcaster
{
public:
    
    enum VideoAnnotationStatus
    {
        CreateTextEvent,
        CreateImageEvent,
        CreateShapeEvent,
        CreateAudioEvent,
        NONE
    };
    
    
    VideoAnnotationMenuWindow(IRStr* str, String name, Rectangle<int>frameRect) :
    DocumentWindow(name, Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), DocumentWindow::allButtons),
    IRStrComponent(str)

    {
        setOpaque(false);
        
        setUsingNativeTitleBar(false);
        setResizable(false, false);
        setTitleBarHeight(0);
        setBounds(frameRect);

        
        setVisible(true);
        
        this->component.reset( new VideoAnnotationMenuComponent(str) );
        this->component->addChangeListener(this);
        this->component->setBounds(frameRect);
        setContentOwned(this->component.get(), true);
      
    }
    
    ~VideoAnnotationMenuWindow()
    {
        this->component.reset();
    }
    // ==================================================
   
    void paint(Graphics& g) override
    {
        //g.fillAll(Colour::fromFloatRGBA(0.4,0.4,0.4,0.4));
        g.fillAll(Colours::transparentBlack);
        
    }

    
    // ==================================================
    std::function<void()> closeMenuWindow;
    void closeAction()
    {
        if(this->closeMenuWindow != nullptr)
            this->closeMenuWindow();
    }
    // ==================================================
    
    void setChangeEventListener(ChangeListener* l)
    {
        this->component->addChangeListener(l);
    }
    VideoAnnotationMenuComponent* getComponent() const { return this->component.get(); }
    // ==================================================
    
    VideoAnnotationStatus getStatus() const { return this->status; }
    // ==================================================

private:
    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        if(source == this->component.get())
        {
            using s = VideoAnnotationMenuComponent::VideoAnnotationMenuComponentStatus;
            
            std::cout << "MenuWindow change\n";
            switch ( this->component->getStatus() )
            {
                case s::CreateTextEvent:
                    
                    std::cout << "VideoAnnotationMenuWindow change\n";
                    this->status = CreateTextEvent;
                    sendChangeMessage();
                    break;
                case s::CreateImageEvent:
                    this->status = CreateImageEvent;
                    sendChangeMessage();

                    break;
                case s::CreateShapeEvent:
                    this->status = CreateShapeEvent;
                    sendChangeMessage();

                    break;
                case s::CreateAudioEvent:
                    this->status = CreateAudioEvent;
                    sendChangeMessage();

                    break;
                default:
                    break;
            }
            
        }
    }

    // ==================================================
    
    IRImageButton TextEventButton;
    IRImageButton ShapeEventButton;
    IRImageButton AudioEventButton;
    // ==================================================
    
    VideoAnnotationStatus status = NONE;
    // ==================================================
    std::shared_ptr<VideoAnnotationMenuComponent> component;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoAnnotationMenuWindow)

};
#endif /* VideoAnnotationMenuWindow_h */
