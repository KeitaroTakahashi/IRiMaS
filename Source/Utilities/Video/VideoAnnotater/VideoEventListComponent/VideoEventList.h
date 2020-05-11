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

class VideoEventList :
public IRViewPort,
public IRVideoAnnotaterDelegate
{
public:
    VideoEventList(IRStr* str, IRVideoAnnotaterBase* base) :
    IRViewPort(str),
    IRVideoAnnotaterDelegate(base)
    {
        this->videoListComponent.reset( new VideoEventListComponent(str, base));
        this->videoListComponent->newEventAddedCallback = [this]{ newEventAdded(); };
        this->viewPort.reset(new Component4ViewPort(this->videoListComponent.get()));
        
        setViewedComponent(this->viewPort.get());
    }
    
    ~VideoEventList()
    {
        this->viewPort.reset();
        this->videoListComponent.reset();
    }
    //==================================================
    
    void resized() override
    {
        //int margin = 5;
        int listCompHeight = this->videoListComponent->getTotalComponentHeight();
        //(this->videoListComponent->getEventNum() + margin) * this->listComponentHeight;
        this->viewPort->setBounds(0,0,getWidth()-10, listCompHeight);
        this->videoListComponent->setSize(getWidth()-10, listCompHeight);
    }
    //==================================================
    
    void newEventAdded()
    {
        resized();
    }
    //==================================================
    // TRANSPORT
    void openAnnotationFile()
    {
        this->videoListComponent->openAnnotationFile();

    }
    
    void openAnnotationFile(File file)
    {
        this->videoListComponent->openAnnotationFile(file);

    }
    
    void saveAnnotationFile()
    {
        this->videoListComponent->saveAnnotationFile();
    }
    
    void saveAnnotationFile(File file)
    {
        this->videoListComponent->saveAnnotationFile(file);
    }
    
    std::string getFilePath()
    {
        return this->videoListComponent->getFilePath();
    }
    
    std::string getSrtSavePath() const
    {
        return this->videoListComponent->getSrtSavePath();
        
    }

    //==================================================
    // EVENTCOMPONENT
    AnnotationTextEventComponent* createTextEventComponent()
    {
        return this->videoListComponent->createTextEventComponent();
        
    }
    
    AnnotationTextEventComponent* createTextEventComponent(float beginTime,
                                                       float endTime)
    {
        return this->videoListComponent->createTextEventComponent(beginTime,
                                                                  endTime);
        
        
    }
    
    AnnotationShapeEventComponent* createShapeEventComponent()
    {
       return this->videoListComponent->createShapeEventComponent();

    }
    
    AnnotationShapeEventComponent* createShapeEventComponent(float beginTime,
                                                             float endTime)
    {
       return this->videoListComponent->createShapeEventComponent(beginTime,
                                                                  endTime);

    }
    AnnotationImageEventComponent* createImageEventComponent()
    {
       return this->videoListComponent->createImageEventComponent();

    }
    
    AnnotationImageEventComponent* createImageEventComponent(float beginTime,
                                                             float endTime)
    {
       return this->videoListComponent->createImageEventComponent(beginTime,
                                                                  endTime);

    }
    AnnotationWaveformEventComponent* createWaveformEventComponent()
    {
       return this->videoListComponent->createWaveformEventComponent();

    }
    
    AnnotationWaveformEventComponent* createWaveformEventComponent(float beginTime,
                                                                float endTime)
    {
       return this->videoListComponent->createWaveformEventComponent(beginTime,
                                                                  endTime);

    }
    //==================================================
    
    std::vector<VideoAnnotationEventComponent*> getSelectedEventComponents()
    {
        return this->videoListComponent->selectedEventComponents;
    }
    
    std::vector<VideoAnnotationEventComponent*> getEventComponents()
    {
        return this->videoListComponent->eventComponents;
    }
    
    
    //==================================================

    void createEventComponent(VideoAnnotationEventComponent* comp)
    {
        this->videoListComponent->createEventComponent(comp);
    }
    
    void deleteSelectedEventComponent()
    {
        this->videoListComponent->deleteSelectedEventComponent();
    }
    
    void deleteEventComponent(VideoAnnotationEventComponent* comp)
    {
        this->videoListComponent->clearEventComponent(comp);
    }
    
    void clearAllEventComponent()
    {
        this->videoListComponent->clearAllEventComponent();

    }
    
    void deSelectAllEventComponents()
    {
        this->videoListComponent->deSelectAllEventComponents();
    }
    
    void selectEventComponent(VideoAnnotationEventComponent* comp)
    {
        this->videoListComponent->selectEventComponent(comp);
    }
    
    //==================================================
    void sortEventComponentByBeginTime()
    {
        
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
