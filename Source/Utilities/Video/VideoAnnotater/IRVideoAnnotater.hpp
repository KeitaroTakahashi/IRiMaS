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
#include "IRVideoPlayerObject.hpp"
#include "VideoAnnotationMenuWindow.h"
#include "VideoEventList.h"


class IRVideoAnnotater : public Component,
public IRStrComponent,
public KeyListener,
public ChangeListener
{
public:
    IRVideoAnnotater(IRStr* str, IRVideoPlayerObject* videoPlayerObject);
    ~IRVideoAnnotater();
    
    // ==================================================
    void paint(Graphics& g) override;
    void resized() override;
    // ==================================================
    
    void openFile();
    void openFile(File file);
    bool hsaVideo() const;

    void myVideoLoadCompleted();
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
    // ==================================================
   
    

private:
    // ==================================================
    // VIDEO
    File videoFile;
    String path;
    
    bool isVideoLoaded = false;
    Rectangle<int> videoArea;
    float aspectRatio = 1.0;

    // ==================================================

    Rectangle<int> workArea;
    // ==================================================

    TextButton openVideoButton;
    void openVideoButtonClicked();
    // ==================================================

    IRVideoPlayerObject* videoPlayerObject = nullptr;
    std::shared_ptr<IRVideoPlayerObject> myVideoPlayerObject;
    
    
    // ==================================================
    IRVideoTransport videoTransport;
    std::unique_ptr<VideoAnnotationMenuWindow> annotationMenu;
    
    // ==================================================
    
    std::shared_ptr<VideoEventList> eventListComponent;
    
    void clearAllEventComponent();
    void clearEventComponent(VideoAnnotationEventComponent* eventComponent);
    void addEventComponent(VideoAnnotationEventComponent* eventComponent);
    
    void createTextEventComponent();
    void createShapeEventComponent();
    void createImageEventComponent();
    void createAudioEventComponent();
    
    void eventComponentResized();

    // ==================================================
    // ==================================================

    void changeListenerCallback (ChangeBroadcaster* source) override;
    void videoTransportChangeListener();
    void annotationMenuChangeListener();
    // ==================================================
    
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
    
    void DeleteKeyPressed();
    void AKeyPressed();
    // ==================================================

    
};

#endif /* IRVideoAnnotater_hpp */
