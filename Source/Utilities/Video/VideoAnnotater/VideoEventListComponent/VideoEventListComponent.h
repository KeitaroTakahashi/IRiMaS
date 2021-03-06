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
            this->filePath = file.getFullPathName().toStdString();
            
            srtOpen();
            
        }
    }
    
    void openAnnotationFile(File file)
    {
        this->filePath = file.getFullPathName().toStdString();
        srtOpen();
    }
    
    void saveAnnotationFile()
    {
        FileChooser chooser("Save srt file...",
                            {},
                            "");
        
        
        if(chooser.browseForFileToSave(true))
        {
            auto file = chooser.getResult();
            this->savePath = file.getFullPathName().toStdString() + ".srt";
            
            srtSave();
        }
    }
    
    void saveAnnotationFile(File file)
    {
        this->savePath = file.getFullPathName().toStdString();
        srtSave();
    }
    
    // ==================================================

    std::string getSrtSavePath() const { return this->savePath; }
    int getEventNum() const { return (int)this->eventComponents.size(); }
    
    // ==================================================

    std::string getFilePath() const { return this->filePath; }
    // ==================================================
    
    void createTextEventComponent()
    {
       std::cout << "createTextEventComponent\n";
       AnnotationTextEventComponent* comp = new AnnotationTextEventComponent(getStr());
       createEventComponent(comp);
    }
    
    void createTextEventComponent(float beginTime,
                                  float endTime)
    {
        AnnotationTextEventComponent* comp = new AnnotationTextEventComponent(getStr(),
                                                                              beginTime,
                                                                              endTime);
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
    }
    
    void eventModified(VideoAnnotationEventComponent* comp) override
    {
        sortEventComponentByTimeCode();
        

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
    //std::string srtPath = "/Users/keitaro/Desktop/out.srt";
    
    std::string filePath;
    std::string savePath;
    
    void srtOpen()
    {
        this->srtL.openFile(this->filePath);
        auto eventList = this->srtL.getSubtitleItems();
        
        if(eventList.size() == 0)
        {
            KLib().showErrorMessage("SRT file has no contents to read.");
            return;
        }
        
        deleteAllEventComponents();
        
        for(auto item : eventList)
        {
                createTextEventComponent(item->getStartTimeString(),
                                         item->getEndTimeString(),
                                         item->getText());
        }
        
        // update events
        eventModified(nullptr);
    }
    
    void srtSave()
    {
        this->srt.open(this->savePath);

        using t = VideoAnnotationEventComponent::VideoAnnotationType;
        
        for(auto item : this->eventComponents)
        {
            this->srt.addItem(item->getSRT());
        }

        this->srt.close();
        
    }

    // ==================================================
    
    // ==================================================
    // ==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoEventListComponent)

};

#endif /* VideoEventListComponent_h */
