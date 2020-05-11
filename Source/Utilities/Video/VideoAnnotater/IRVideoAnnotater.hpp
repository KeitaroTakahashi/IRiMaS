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
#include "IRVideoAnnotaterObject.h"
#include "VideoAnnotationMenuWindow.h"
#include "VideoEventList.h"
#include "EventLogList.h"
#include "IRVideoAnnotaterBase.h"
#include "IRVideoAnnotaterDelegate.h"
#include "VideoAnnotaterWorkspace.hpp"

// VideoAnontater object
#include "IRVATextEditorObject.h"
#include "IRVAShapeObject.h"
#include "IRVAImageViewerObject.h"
#include "IRVAWaveformObject.h"

#include "IRObjectCreater.hpp"

class IRVideoAnnotater : public Component,
public IRStrComponent,
public IRVideoAnnotaterBase,
public KeyListener,
public ChangeListener,
private IRWorkspaceComponent::Listener
{
public:
    IRVideoAnnotater(IRStr* str, IRVideoAnnotaterObject* videoPlayerObject);
    ~IRVideoAnnotater();
    
    // ==================================================
    void paint(Graphics& g) override;
    void resized() override;
    void videoResized();
    
    // do whatever necessary task when its window is opened.
    void openAnnotaterWindowAction();
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
    
    void updateVideoSize(juce::Point<int> newVideoSize);
    
    // ==================================================
    void setVideoComponent(IRVideoComponent* videoComponent);
    
    
    // as videoPlayerObject is shared with IRWorkspace, you need to remove it from this component when closed.
    void bindVideoPlayerObject();
    void removeVideoPlayerObject();
    
    // update video player and share the status.
    void updateVideoPlayerOfThis();
    void updateVideoPlayerOfWorkspace();
    
    void updateWorkspaceWithCurrentPlayingPosition(float pos);
    // ==================================================
    void openAnnotationMenu();
    void closeAnnotationMenu();
    
    void addEventButtonAction();
    void deleteEventButtonAction();
    // by slider
    void playPositionChangedBySliderAction();
    void playAction();
    void stopAction();
    // ==================================================
   
    void setEventModifiedCallback(std::function<void(VideoAnnotationEventComponent*)> callback);
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
    // SYSTEM DELEGATE
    IRVideoAnnotaterDelegate* delegate = nullptr;
    
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
    
    void createVideoTransport();
    // ==================================================
    
    std::shared_ptr<VideoEventList> eventListComponent;
    void createEventListComponent();
    
    void clearAllEventComponent();
    void clearEventComponent(VideoAnnotationEventComponent* eventComponent);
    void addEventComponent(VideoAnnotationEventComponent* eventComponent);
    void deleteSelectedEvents();
    void deleteEventComponent(VideoAnnotationEventComponent* event);
    
    void createTextEventComponent();
    void createTextEventComponentFromIRNodeObject(IRNodeObject* obj);
    void createShapeEventComponent();
    void createShapeEventComponentFromNodeObject(IRNodeObject* obj);
    void createImageEventComponent();
    void createImageEventComponentFromNodeObject(IRNodeObject* obj);
    void createAudioEventComponent();
    void createAudioEventComponentFromNodeObject(IRNodeObject* obj);

    void eventComponentResized();
    
    void createNodeObjectOnEvent(IRNodeObject* obj, VideoAnnotationEventComponent* event);

    // ==================================================
    //IRWorkspace
    // getting to know when the selection status of a NodeObject has been changed.
    // This function is called when an object is either selected or unselected.
    // this function is, for instance, used to update the Object Controller in the IRRightBar
    void createWorkspace();
    void nothingSelected() override;
    void nodeObjectPasted(IRNodeObject* obj) override;
    void nodeObjectWillDeleted(IRNodeObject* obj) override;
    // workspaceComponent::Listener
    void nodeObjectSelectionChange(IRNodeObject* obj) override;
    void nodeObjectGetFocused(IRNodeObject* obj)override;
    void editModeChanged(IRWorkspaceComponent* changedSpace) override;
    void heavyObjectCreated(IRNodeObject* obj) override;
    
    
    void deselectAllObjectsOnWorkspace();
    // ==================================================
    std::shared_ptr<EventLogList> eventLogList;
    
    

    // ==================================================
    // Apply
    void applyEventsOnTheLoadedVideo(VideoAnnotationEventComponent* event);
    void updateEventsOnTheLoadedVideo();
    // create
    void createEventOnTheLoadedVideo(VideoAnnotationEventComponent* event);
    void deleteEventOnTheLoadedVideo();
    // ==================================================

    void changeListenerCallback (ChangeBroadcaster* source) override;
    void videoTransportChangeListener();
    void annotationMenuChangeListener();
    
    // ==================================================

    // IRVideoAnnotaterBase
    void eventModifiedAction(Component* modifiedEvent) override;
    void eventSelectedAction(Component* selectedEvent) override;
    
    void updateAnimation() override;
    void showEventPosition(Component* event) override;
    // ==================================================

    std::function<void(VideoAnnotationEventComponent*)> eventModifiedCallback;
    // ==================================================
    
    void textEventComponentSelected(VideoAnnotationEventComponent* event);
    void shapeEventComponentSelected(VideoAnnotationEventComponent* event);
    
    // ==================================================

    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
    
    void DeleteKeyPressed();
    void AKeyPressed();
    void CommandWKeyPressed();
    void CommandEPressed();
    void CommandSPressed();
    // ==================================================
    
    std::shared_ptr<VideoAnnotaterWorkspace> workspace;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRVideoAnnotater)

};

#endif /* IRVideoAnnotater_hpp */
