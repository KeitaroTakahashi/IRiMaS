//
//  VideoEventListComponent.h
//  IRiMaS
//
//  Created by Keitaro on 12/12/2019.
//

#ifndef VideoEventListComponent_h
#define VideoEventListComponent_h

#include "AnnotationTextEventComponent.h"
#include "AnnotationShapeEventComponent.h"
#include "KLib.h"

class VideoEventListComponent : public Component,
public IRStrComponent,
public VideoAnnotationEventComponent::Listener
{
public:
    
    VideoEventListComponent(IRStr* str) :
    IRStrComponent(str)
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
    
    void mouseDown(const MouseEvent& e) override
    {
        deSelectAllEventComponents();
    }
    
    // ==================================================
    
    void openAnnotationFile()
    {
        FileChooser chooser("Select a SRT file to load...",
                            {},
                            "*.srt", "*.srts");
        
        
        if(chooser.browseForFileToOpen())
        {
            auto file = chooser.getResult();
            auto path = file.getFullPathName();
            
            this->srtL.openFile(path.toStdString());
            auto eventList = this->srtL.getSubtitleItems();
            
            if(eventList.size() == 0)
            {
                KLib().showErrorMessage("SRT file has no contents to read.");
                return;
            }
            
            deleteAllEventComponents();
            
            for(auto item : eventList)
            {
                std::cout << item->getStartTimeString() << " --> " << item->getEndTimeString() << " : " << item->getText() << std::endl;
                
              
                    createTextEventComponent(item->getStartTimeString(),
                                             item->getEndTimeString(),
                                             item->getText());
            }
            
            // update events
            eventModified(nullptr);
            
        }
    }
    
    void openAnnotationFile(File file)
    {
        
    }
    
    // ==================================================

    int getEventNum() const { return (int)this->eventComponents.size(); }
    
    // ==================================================
    
    void createTextEventComponent()
    {
       std::cout << "createTextEventComponent\n";
       AnnotationTextEventComponent* comp = new AnnotationTextEventComponent(getStr());
       createEventComponent(comp);
    }
    
    void createTextEventComponent(std::string beginTime,
                                  std::string endTime,
                                  std::string contents)
    {
        AnnotationTextEventComponent* comp = new AnnotationTextEventComponent(getStr(),
                                                                              beginTime,
                                                                              endTime,
                                                                              contents);
        createEventComponent(comp);
        

    }
    
    void createShapeEventComponent()
    {
        AnnotationShapeEventComponent* comp = new AnnotationShapeEventComponent(getStr());
        createEventComponent(comp);
    }
    
    void createImageEventComponent()
    {
       
    }
    void createAudioEventComponent()
    {
       
    }
    
    void createEventComponent(VideoAnnotationEventComponent* comp)
    {
        addEventComponent(comp);
        comp->addListener(this);
        addAndMakeVisible(comp);
        
        sortEventComponentByTimeCode();
    }
    
    void deleteAllEventComponents()
    {
        for(auto event : this->eventComponents)
        {
            selectEventComponent(event);
        }
        deleteSelectedEventComponent();
    }
    
    void deleteSelectedEventComponent()
    {
        for(auto event : this->selectedEventComponents)
        {
            clearEventComponent(event);
        }
        this->selectedEventComponents.clear();
        
        eventComponentResized();
    }
    
    void deSelectAllEventComponents()
    {
        for(auto event : this->selectedEventComponents)
        {
            event->setSelected(false);
        }
        
        this->selectedEventComponents.clear();
    }
    
    void selectEventComponent(VideoAnnotationEventComponent* comp)
    {
        comp->setSelected(true);
        this->selectedEventComponents.push_back(comp);
        comp->repaint();
    }
    
    void deSelectEventComponent(VideoAnnotationEventComponent* comp)
    {
        auto it = std::find(this->selectedEventComponents.begin(), this->selectedEventComponents.end(), comp);
        if(it != this->selectedEventComponents.end())
        {
            this->selectedEventComponents.erase(it);
        }
        

        comp->setSelected(false);
    }
    // ==================================================
    // VideoAnnotationEventComponent Listener
    void eventComponentSelected(VideoAnnotationEventComponent* comp) override
    {
        deSelectAllEventComponents();
        selectEventComponent(comp);
        std::cout << "eventComponentSelected called\n";
    }
    
    void eventModified(VideoAnnotationEventComponent* comp) override
    {
        sortEventComponentByTimeCode();
        
        this->srt.open(this->srtPath);
        
        using t = VideoAnnotationEventComponent::VideoAnnotationType;
        for(auto item : this->eventComponents)
        {
            this->srt.addItem(item->getSRT());

        }

        this->srt.close();
        
        if(this->eventModifiedCallback != nullptr)
            this->eventModifiedCallback();

    }
    
    void eventActiveChanged(VideoAnnotationEventComponent* comp) override
    {
        
    }
    
    // ==================================================
    void sortEventComponentByTimeCode()
    {
        std::sort(this->eventComponents.begin(), this->eventComponents.end(), [](const VideoAnnotationEventComponent* a, const VideoAnnotationEventComponent* b){
            return a->sortVal1 < b->sortVal1;
        });
        
        eventComponentResized();
    }
    
    // SORT
    bool cmp(const VideoAnnotationEventComponent& a, const VideoAnnotationEventComponent& b)
    {
        return a.sortVal1 < b.sortVal1;
    }
    // ==================================================

    std::function<void()> newEventAddedCallback;
    std::function<void()> eventModifiedCallback;
    // ==================================================

    std::vector<VideoAnnotationEventComponent*> eventComponents;
    std::vector<VideoAnnotationEventComponent*> selectedEventComponents;
    
    // ==================================================
    void sortEventComponentsByAscending()
    {
        
    }
   
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
    
    private:
    // ==================================================
    srtWriter srt;
    srtLoader srtL;
    std::string srtPath = "/Users/keitaro/Desktop/out.srt";

    // ==================================================
    
    // ==================================================
    // ==================================================

};

#endif /* VideoEventListComponent_h */
