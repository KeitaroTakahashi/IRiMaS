//
//  IRVideoAnnotater.hpp
//  IRiMaS
//
//  Created by Keitaro on 07/12/2019.
//

#ifndef IRVideoAnnotater_hpp
#define IRVideoAnnotater_hpp

#include "IRStrComponent.hpp"
#include "VideoTransport.h"
#include "IRVideoAnnotaterObject.hpp"
#include "VideoAnnotationMenuWindow.h"
#include "VideoEventList.h"


class IRVideoAnnotater : public Component,
public IRStrComponent,
public KeyListener,
public ChangeListener
{
public:
    IRVideoAnnotater(IRStr* str, IRVideoAnnotaterObject* videoPlayerObject);
    ~IRVideoAnnotater();
    
    // ==================================================
    void paint(Graphics& g) override;
    void resized() override;
    void videoResized();
    // ==================================================
    
    void openFile();
    void openFile(File file);
    
    // SRTs file is the original file format by Keitaro
    // this covers shape, image, and other IRNodeObjects besides text subtitles.
    void openSRTs();
    void openSRTs(File file);
    void saveSRTs();
    void saveSRTs(File file);
    
    bool hsaVideo() const;

    void myVideoLoadCompleted();
    void myVideoPlayingUpdate(double pos);
    // ==================================================
    void setVideoComponent(IRVideoComponent* videoComponent);
    
    // as videoPlayerObject is shared with IRWorkspace, you need to remove it from this component when closed.
    void bindVideoPlayerObject();
    void removeVideoPlayerObject();
    
    // update video player and share the status.
    void updateVideoPlayerOfThis();
    void updateVideoPlayerOfWorkspace();
    // ==================================================
    void openAnnotationMenu();
    void closeAnnotationMenu();
    
    void addEventButtonAction();
    void deleteEventButtonAction();
    void playPositionChangedAction();
    void playAction();
    void stopAction();
    // ==================================================
   
    void setEventModifiedCallback(std::function<void()> callback);
    // ==================================================

    std::vector<VideoAnnotationEventComponent*> getEventComponents()
    {
        return this->eventListComponent->getEventComponents();
    }
    // ==================================================
    std::string getSRTFilePath();
    // ==================================================
    std::function<void()> closeAnnotationWindowCallback;
    void closeAnnotationWindow();
    // ==================================================

private:
    // ==================================================
    // VIDEO
    File videoFile;
    String path;
    
    //SRT
    String srtSavePath;
    
    bool isVideoLoaded = false;
    Rectangle<int> videoArea;
    float aspectRatio = 1.0;

    // ==================================================

    Rectangle<int> workArea;
    // ==================================================

    TextButton openVideoButton;
    void openVideoButtonClicked();
    // ==================================================

    IRVideoAnnotaterObject* videoPlayerObject = nullptr;
    std::shared_ptr<IRVideoAnnotaterObject> myVideoPlayerObject;
    
    
    // ==================================================
    IRVideoTransport videoTransport;
    std::unique_ptr<VideoAnnotationMenuWindow> annotationMenu;
    
    // ==================================================
    
    std::shared_ptr<VideoEventList> eventListComponent;
    
    void clearAllEventComponent();
    void clearEventComponent(VideoAnnotationEventComponent* eventComponent);
    void addEventComponent(VideoAnnotationEventComponent* eventComponent);
    void deleteSelectedEvents();
    
    void createTextEventComponent();
    void createShapeEventComponent();
    void createImageEventComponent();
    void createAudioEventComponent();
    
    
    void eventComponentResized();

    // ==================================================
    // Apply
    void applyEventsOnTheLoadedVideo();
    // ==================================================

    void changeListenerCallback (ChangeBroadcaster* source) override;
    void videoTransportChangeListener();
    void annotationMenuChangeListener();
    
    void eventModifiedAction();
    std::function<void()> eventModifiedCallback;
    // ==================================================
    
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
    
    void DeleteKeyPressed();
    void AKeyPressed();
    void CommandWKeyPressed();
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRVideoAnnotater)

};

#endif /* IRVideoAnnotater_hpp */
