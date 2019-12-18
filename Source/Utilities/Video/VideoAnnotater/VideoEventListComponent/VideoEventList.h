//
//  VideoEventList.h
//  IRiMaS
//
//  Created by Keitaro on 12/12/2019.
//

#ifndef VideoEventList_h
#define VideoEventList_h
#include "IRViewPort.hpp"
#include "VideoEventListComponent.h"

class VideoEventList : public IRViewPort
{
public:
    VideoEventList(IRStr* str) : IRViewPort(str)
    {
        this->videoListComponent.reset( new VideoEventListComponent(str));
        this->videoListComponent->newEventAddedCallback = [this]{ newEventAdded(); };
        this->viewPort.reset(new Component4ViewPort(this->videoListComponent.get()));
        
        setViewedComponent(this->viewPort.get());
    }
    
    ~VideoEventList()
    {
        
    }
    //==================================================
    
    void resized() override
    {
        int margin = 5;
        int listCompHeight = (this->videoListComponent->getEventNum() + margin) * this->listComponentHeight;
        this->viewPort->setBounds(0,0,getWidth()-10, listCompHeight);
        std::cout << "listCompHeight = " << listCompHeight << std::endl;
        this->videoListComponent->setSize(getWidth()-10, listCompHeight);
    }
    //==================================================
    
    void newEventAdded()
    {
        resized();
    }
    //==================================================
    
    void createTextEventComponent()
    {
        std::cout << "V::createTextEventComponent\n";

        this->videoListComponent->createTextEventComponent();
    }
    void createShapeEventComponent()
    {
       this->videoListComponent->createShapeEventComponent();

    }
    void createImageEventComponent()
    {
       this->videoListComponent->createImageEventComponent();

    }
    void createAudioEventComponent()
    {
       this->videoListComponent->createAudioEventComponent();

    }

    
    //==================================================
    void visibleAreaChangedAction(const Rectangle<int> &newVisibleArea) override
    {
        
    }

private:
    //==================================================
    

    
    class Component4ViewPort : public Component
    {
    public:
        Component4ViewPort(Component* main) : main(main)
        {
            addAndMakeVisible(main);
        }
        
        ~Component4ViewPort() {}
        
        void resized() override
        {
            this->main->setBounds(0, 0, getWidth(), getHeight());
        }
        
        
    private:
        Component* main;
        
    };
    
    
    //==================================================
    std::shared_ptr<Component4ViewPort> viewPort;
    
    std::shared_ptr<VideoEventListComponent> videoListComponent;
    
    int listComponentHeight = 40;

    //==================================================
    //==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoEventList)

};

#endif /* VideoEventList_h */
