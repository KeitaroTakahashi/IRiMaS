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
#include "AnnotationImageEventComponent.h"
#include "AnnotationWaveformEventComponent.h"
#include "KLib.h"

class VideoEventListComponent : public Component,
public IRStrComponent,
public IRVideoAnnotaterDelegate,
public VideoAnnotationEventComponent::Listener
{
public:
    
    VideoEventListComponent(IRStr* str, IRVideoAnnotaterBase* base) :
    IRStrComponent(str),
    IRVideoAnnotaterDelegate(base)
    {
        
    }
    
    ~VideoEventListComponent()
    {
        //clearAllEventComponent();
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
    /*
    AnnotationTextEventComponent* createTextEventComponent()
    {
        std::cout << "createTextEventComponent\n";
        AnnotationTextEventComponent* comp = new AnnotationTextEventComponent(getStr(),
                                                                              getBase());
        addEventComponent(comp);
        return comp;
    }
    
    AnnotationTextEventComponent* createTextEventComponent(float beginTime,
                                  float endTime)
    {
        AnnotationTextEventComponent* comp = new AnnotationTextEventComponent(getStr(),
                                                                              getBase(),
                                                                              beginTime,
                                                                              endTime);
        addEventComponent(comp);
        return comp;

    }
    
    AnnotationTextEventComponent* createTextEventComponent(std::string beginTime,
                                                           std::string endTime,
                                                           std::string contents)
    {
        AnnotationTextEventComponent* comp = new AnnotationTextEventComponent(getStr(),
                                                                              getBase(),
                                                                              beginTime,
                                                                              endTime,
                                                                              contents);
        addEventComponent(comp);
        return comp;
    }
    
    AnnotationShapeEventComponent* createShapeEventComponent()
    {
        AnnotationShapeEventComponent* comp = new AnnotationShapeEventComponent(getStr(),
                                                                                this->getBase());
        addEventComponent(comp);
        return comp;
    }
    
    AnnotationShapeEventComponent* createShapeEventComponent(float beginTime,
                                                             float endTime)
    {
        
        AnnotationShapeEventComponent* comp = new AnnotationShapeEventComponent(getStr(),
                                                                                getBase(),
                                                                                beginTime,
                                                                                endTime);
        
        addEventComponent(comp);
        return comp;
    }
    
    AnnotationImageEventComponent* createImageEventComponent()
    {
       AnnotationImageEventComponent* comp = new AnnotationImageEventComponent(getStr(),
                                                                               getBase());
       addEventComponent(comp);
       return comp;
    }
    
    AnnotationImageEventComponent* createImageEventComponent(float beginTime,
                                                             float endTime)
    {
        AnnotationImageEventComponent* comp = new AnnotationImageEventComponent(getStr(),
                                                                                getBase(),
                                                                                beginTime,
                                                                                endTime);
        addEventComponent(comp);
        return comp;
    }
    

    AnnotationWaveformEventComponent* createWaveformEventComponent()
    {
       AnnotationWaveformEventComponent* comp = new AnnotationWaveformEventComponent(getStr(),
                                                                                     getBase());
       addEventComponent(comp);
       return comp;
    }
    
    AnnotationWaveformEventComponent* createWaveformEventComponent(float beginTime,
                                                                   float endTime)
    {
        AnnotationWaveformEventComponent* comp = new AnnotationWaveformEventComponent(getStr(),
                                                                                      getBase(),
                                                                                      beginTime,
                                                                                      endTime);
        addEventComponent(comp);
        return comp;
    }
    
    */
    
    void addEventComponent(VideoAnnotationEventComponent* comp)
    {
  
        registerEventComponent(comp);
        comp->addListener(this);
        addAndMakeVisible(comp);
        
        deSelectAllEventComponents();
        selectEventComponent(comp);
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
        
        /*
        if(this->eventSelectedCallback != nullptr)
            this->eventSelectedCallback(comp);
         */
        
        eventSelectedAction(comp);
    }
    
    void eventModified(VideoAnnotationEventComponent* comp) override
    {
        sortEventComponentByTimeCode();

        /*
        if(this->eventModifiedCallback != nullptr)
            this->eventModifiedCallback(comp);
         */
        
        eventModifiedAction(comp);

    }
    
    void eventActiveChanged(VideoAnnotationEventComponent* comp) override
    {
        // call updateAnimation method in IRVideoAnnotater
        getBase()->eventActivationChanged(comp);
    }
    
    void eventShownOnVideoPlayer(VideoAnnotationEventComponent* comp) override
    {
        
    }
    
    void eventHiddenOnVideoPlayer(VideoAnnotationEventComponent* comp) override
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
   // std::function<void(VideoAnnotationEventComponent*)> eventModifiedCallback;
    //std::function<void(VideoAnnotationEventComponent*)> eventSelectedCallback;

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
        /*
        for(auto event : this->eventComponents)
        {
            delete event;
        }*/
        this->eventComponents.clear();
        this->selectedEventComponents.clear();
    }
    
    void clearEventComponent(VideoAnnotationEventComponent* eventComponent)
    {
        
        removeChildComponent(eventComponent);
        
        auto it = std::find(this->eventComponents.begin(), this->eventComponents.end(), eventComponent);
        if(it != this->eventComponents.end())
        {
            this->eventComponents.erase(it);
            delete eventComponent;
        }
        
         deSelectAllEventComponents();
        
        //sort
        sortEventComponentByTimeCode();

    }
    
    void registerEventComponent(VideoAnnotationEventComponent* eventComponent)
    {

        this->eventComponents.push_back(eventComponent);

        if(this->newEventAddedCallback != nullptr)
            this->newEventAddedCallback();
        
        newEventCreatedAction(this);
    }
    
    void eventComponentResized()
    {
        int margin = 5;
        int x = margin;
        int y = margin;
        int w = getWidth();
        
        for(auto event : this->eventComponents)
        {
            int h = event->getInitHeight();
            event->setBounds(x, y, w, h);
            y += h + margin;
        }
    }
    // return sum of the height of all added eventcomponent
    int getTotalComponentHeight()
    {
        int height = 0;
        int margin = 5;

        for(auto event : this->eventComponents)
        {
            height += event->getInitHeight() + 5;
        }
        
        return height;
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
                //createTextEventComponent(item->getStartTimeString(),
                  //                       item->getEndTimeString(),
                   //                      item->getText());
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
