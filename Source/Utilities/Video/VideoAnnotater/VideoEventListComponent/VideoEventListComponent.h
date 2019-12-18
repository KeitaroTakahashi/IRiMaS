//
//  VideoEventListComponent.h
//  IRiMaS
//
//  Created by Keitaro on 12/12/2019.
//

#ifndef VideoEventListComponent_h
#define VideoEventListComponent_h

#include "VideoAnnotationEventComponent.hpp"
#include "AnnotationTextEventComponent.h"
#include "AnnotationShapeEventComponent.h"

class VideoEventListComponent : public Component,
public IRStrComponent
{
public:
    
    VideoEventListComponent(IRStr* str) : IRStrComponent(str)
    {
        
    }
    
    ~VideoEventListComponent()
    {
        clearAllEventComponent();
    }
    // ==================================================

    void paint(Graphics& g) override
    {
        g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    }
    
    void resized() override
    {
        eventComponentResized();
    }
    
    // ==================================================
    
    int getEventNum() const { return (int)this->eventComponents.size(); }
    // ==================================================
    
    void createTextEventComponent()
    {
       std::cout << "createTextEventComponent\n";
       AnnotationTextEventComponent* comp = new AnnotationTextEventComponent(getStr());
       addEventComponent(comp);
       addAndMakeVisible(comp);
    }
    void createShapeEventComponent()
    {
       AnnotationShapeEventComponent* comp = new AnnotationShapeEventComponent(getStr());
       addEventComponent(comp);
       addAndMakeVisible(comp);
    }
    void createImageEventComponent()
    {
       
    }
    void createAudioEventComponent()
    {
       
    }
    
    void deleteSelectedEventComponent()
    {
        for(auto event : this->selectedEventComponents)
        {
            clearEventComponent(event);
        }
        this->selectedEventComponents.clear();
    }
    
    void deSelectAllEventComponents()
    {
        for(auto event : this->selectedEventComponents)
        {
            event->setSelected(false);
        }
    }
    // ==================================================
    
    // ==================================================

    std::function<void()> newEventAddedCallback;
    
private:
    // ==================================================
    
    std::vector<VideoAnnotationEventComponent*> eventComponents;
    std::vector<VideoAnnotationEventComponent*> selectedEventComponents;

   
    // ==================================================

    void clearAllEventComponent()
    {
        for(auto event : this->eventComponents)
        {
            delete event;
        }
        this->eventComponents.clear();
        this->selectedEventComponents.clear();
    }
    
    void clearEventComponent(VideoAnnotationEventComponent* eventComponent)
    {
        auto it = std::find(this->eventComponents.begin(), this->eventComponents.end(), eventComponent);
        if(it != this->eventComponents.end())
        {
            this->eventComponents.erase(it);
            delete eventComponent;
        }
    }
    
    void addEventComponent(VideoAnnotationEventComponent* eventComponent)
    {
        this->eventComponents.push_back(eventComponent);
        
        if(this->newEventAddedCallback != nullptr)
            this->newEventAddedCallback();
    }
    
    void eventComponentResized()
    {
        int margin = 5;
        int x = margin;
        int y = margin;
        int w = getWidth();
        int h = 40;
        
        for(auto event : this->eventComponents)
        {
            event->setBounds(x, y, w, h);
            y += h + margin;
        }
    }

    // ==================================================
    
    // ==================================================
    // ==================================================

};

#endif /* VideoEventListComponent_h */
