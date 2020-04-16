//
//  IRVideoAnnotater.cpp
//  IRiMaS
//
//  Created by Keitaro on 07/12/2019.
//

#include "IRVideoAnnotater.hpp"

IRVideoAnnotater::IRVideoAnnotater(IRStr* str, IRVideoAnnotaterObject* videoPlayerObject) : IRStrComponent(str),
videoArea(10, 10, 640, 480),
videoPlayerObject(videoPlayerObject),
videoTransport(str, this)
{
   
    // setup delegate
    this->delegate = new IRVideoAnnotaterDelegate(this);
    
    // video button
    addAndMakeVisible(&this->openVideoButton);
    this->openVideoButton.setButtonText("Open Video Annotater");
    this->openVideoButton.onClick = [this] { openVideoButtonClicked(); };
    
    createVideoTransport();
    createEventListComponent();
    this->eventLogList.reset(new EventLogList(str));
    addAndMakeVisible(this->eventLogList.get());
    
    setWantsKeyboardFocus(true);
    
    createWorkspace();
    
   
}

IRVideoAnnotater::~IRVideoAnnotater()
{
    closeAnnotationMenu();
    
    this->workspace.reset();
    
    this->myVideoPlayerObject.reset();

    this->eventListComponent.reset();
    
    delete this->delegate;
    
}


// ==================================================
void IRVideoAnnotater::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);

    g.setColour(getStr()->SYSTEMCOLOUR.contents);
    
    g.fillRect(this->videoArea);
    g.setColour(getStr()->SYSTEMCOLOUR.contents);

    g.fillRect(this->workArea);
    
}

void IRVideoAnnotater::resized()
{
    int xMarge = 10;
    int yMarge = 10;
    this->videoArea = Rectangle<int>(xMarge, yMarge, getWidth() * 0.6, getHeight() * 0.5);
    int ha = this->videoArea.getY() + this->videoArea.getHeight() + 5;
    
    this->workArea  = Rectangle<int>(xMarge,
                                     ha + 45,
                                     getWidth() - 20,
                                     getHeight() - (ha + 55));
    
    videoResized();
    
    this->videoTransport.setBounds(xMarge,
                                   ha,
                                   getWidth() - 20,
                                   40);
    
    this->openVideoButton.setBounds(xMarge, ha, 100, 30);
    
    this->eventListComponent->setBounds(this->workArea);
  
   
    this->eventLogList->setBounds(xMarge + this->videoArea.getWidth(),
                                  yMarge,
                                  getWidth() - xMarge - this->videoArea.getWidth() - xMarge,
                                  getHeight() * 0.5);
    

    
    eventComponentResized();
}

void IRVideoAnnotater::videoResized()
{
    if(this->myVideoPlayerObject.get() != nullptr)
    {
        this->myVideoPlayerObject->resizeThisComponent(Rectangle<int>(this->videoArea.getX(),
                                                                      this->videoArea.getY(),
                                                                      this->videoArea.getWidth(),
                                                                      this->videoArea.getHeight()));
        

        this->workspace->setBounds(this->myVideoPlayerObject->getBounds());
        
    }
}
// ==================================================

void IRVideoAnnotater::openAnnotaterWindowAction()
{
    if(this->eventListComponent.get() == nullptr) return;
    auto events = this->eventListComponent->getEventComponents();
    if(events.size() > 0)
    {
        if(this->workspace.get() != nullptr){
            this->workspace->manageHeavyWeightComponents(true);
        }
    }
}

// ==================================================


void IRVideoAnnotater::createVideoTransport()
{
    // video transport
    addAndMakeVisible(&this->videoTransport);
    this->videoTransport.addChangeListener(this);
    setVisible(true);
}

void IRVideoAnnotater::createEventListComponent()
{
    // eventListComponent
    this->eventListComponent.reset(new VideoEventList(getStr(), this));
    addAndMakeVisible(this->eventListComponent.get());

}

void IRVideoAnnotater::createWorkspace()
{
    Rectangle<int> r(0, 0, 0, 0);
    this->workspace.reset( new VideoAnnotaterWorkspace("VideoAnnotater", r, getStr() ));
    this->workspace->setDraggableMargin(Rectangle<int>(0, 0, 0, 0));
    addAndMakeVisible(this->workspace.get());
    this->workspace->setBackgroundColour(Colours::transparentBlack);
    this->workspace->addListener(this);
    this->workspace->addKeyListener(this);
}

void IRVideoAnnotater::nothingSelected()
{
    if(this->eventLogList.get() != nullptr)
        this->eventLogList->removeLogComponent();
}

void IRVideoAnnotater::nodeObjectPasted(IRNodeObject* obj)
{
    String name = obj->name;
    std::cout << "nodeObjectPasted " << name << std::endl;
    
    if(name == "IRTextEditor")
    {
        createTextEventComponentFromIRNodeObject(obj);
    }else if(name == "IRShape")
    {
        createShapeEventComponentFromNodeObject(obj);
    }
}

void IRVideoAnnotater::nodeObjectWillDeleted(IRNodeObject* obj)
{
    auto event = static_cast<VideoAnnotationEventComponent* >(obj->getEventComponent());
    
    std::cout <<"nodeObejctWillDeleted , event delete " << event << std::endl;
    deleteEventComponent(event);
}
// ==================================================

void IRVideoAnnotater::eventComponentResized()
{

}
// ==================================================

void IRVideoAnnotater::bindVideoPlayerObject()
{
    if(this->videoPlayerObject != nullptr)
    {
        
        std::cout << "bindVideoPlayerObject\n";
        // create IRVideoPlayerObject without OpenButton
        /*
        this->myVideoPlayerObject.reset( new IRVideoAnnotaterObject(this->videoPlayerObject->getParent(),
                                                                getStr(),
                                                                 false));
         */
        this->myVideoPlayerObject.reset( new IRVideoAnnotaterObject(
                                                                    this,
                                                                    getStr(),
                                                                    false));
        
        this->myVideoPlayerObject->videoLoadCompletedCallbackFunc = [this] { myVideoLoadCompleted(); };
        this->myVideoPlayerObject->videoPlayingUpdate = [this](double pos){ myVideoPlayingUpdate(pos); };
        // disable QT controller
        this->myVideoPlayerObject->enableController(false);

        
        //this->videoPlayerObject->copyContents(this->myVideoPlayerObject.get());

        this->myVideoPlayerObject->setMovable(false, false, false);
        this->myVideoPlayerObject->setResizable(false);
        this->myVideoPlayerObject->setObjectBounds(this->videoArea);
        addAndMakeVisible(this->myVideoPlayerObject.get());
        
        // set up workspace
        this->workspace->setFixObjectSizeRatioWithOriginalSize(false, this->myVideoPlayerObject->getBounds().toFloat());
        
        
        resized();
    }
}
void IRVideoAnnotater::removeVideoPlayerObject()
{
    if(this->myVideoPlayerObject.get() != nullptr)
    {
        removeChildComponent(this->myVideoPlayerObject.get());
    }
}
// ==================================================

void IRVideoAnnotater::openVideoButtonClicked()
{
    openFile();
}

// --------------------------------------------------
void IRVideoAnnotater::openFile()
{
    if(this->myVideoPlayerObject == nullptr) return ;
    
    this->myVideoPlayerObject->openFile();
    
    resized();
}

void IRVideoAnnotater::openFile(File file)
{
    this->myVideoPlayerObject->openFile(file);
}
// --------------------------------------------------

void IRVideoAnnotater::openSRTs()
{
    if(this->eventListComponent.get() != nullptr)
        this->eventListComponent->openAnnotationFile();
}

void IRVideoAnnotater::openSRTs(File file)
{
    if(this->eventListComponent.get() != nullptr)
        this->eventListComponent->openAnnotationFile(file);
    else std::cout << "error eventListComponent null\n";
}

void IRVideoAnnotater::saveSRTs()
{
    
    if(this->srtSavePath.length() == 0)
    {
        if(this->eventListComponent.get() != nullptr){
            this->eventListComponent->saveAnnotationFile();
            
            this->srtSavePath = this->eventListComponent->getSrtSavePath();
        }
    }else{
        File file(this->srtSavePath);
        if(file.exists())
        {
            saveSRTs(File(this->srtSavePath));
        }else{
            this->srtSavePath = "";
            saveSRTs();
        }
    }
}

void IRVideoAnnotater::saveSRTs(File file)
{
    if(this->eventListComponent.get() != nullptr)
        this->eventListComponent->saveAnnotationFile(file);
}

std::string IRVideoAnnotater::getSRTFilePath()
{
    if(this->eventListComponent.get() != nullptr)
        return this->eventListComponent->getFilePath();
    else return "";
}

// --------------------------------------------------

bool IRVideoAnnotater::hsaVideo() const { return this->isVideoLoaded; }

// --------------------------------------------------

void IRVideoAnnotater::myVideoLoadCompleted()
{
    //disable controller
    this->myVideoPlayerObject->enableController(false);
    
    updateVideoSize(this->myVideoPlayerObject->getVideoSize());
    // update videoPlayerObject
    std::cout<< "myVideoLoadCompleted length = " << myVideoPlayerObject->getVideoPlayer()->getVideoLength() << std::endl;
    //update video length
    this->videoTransport.setVideoLengthInSec(myVideoPlayerObject->getVideoPlayer()->getVideoLength());
    std::cout << myVideoPlayerObject->getVideoPlayer()->getVideoLength() << std::endl;
    
    this->workspace->bringThisToFront("Workspace bringToThisFront");
    this->workspace->manageHeavyWeightComponents(true);
    
    resized();
}

void IRVideoAnnotater::updateVideoSize(juce::Point<int> newVideoSize)
{
    setVideoSize(newVideoSize);
}

void IRVideoAnnotater::myVideoPlayingUpdate(double pos)
{
    this->videoTransport.setCurrentPlayingPosition(pos);
    
    
    updateWorkspaceWithCurrentPlayingPosition(pos);
}

void IRVideoAnnotater::updateWorkspaceWithCurrentPlayingPosition(float pos)
{
    // test
    this->workspace->enableTimeCodeAnimation(true);
    this->workspace->setCurrentTimeCode(pos);
    this->workspace->updateCurrentAnimation();
}


// ==================================================

void IRVideoAnnotater::openAnnotationMenu ()
{
    Rectangle<int> pos = getBounds();
    pos.setX(getScreenPosition().getX());
    pos.setY(getScreenPosition().getY());

    this->annotationMenu.reset(new VideoAnnotationMenuWindow(getStr(),
                                                             "AnnotationMenu",
                                                             pos));
    
    this->annotationMenu->closeMenuWindow = [this]{ closeAnnotationMenu(); };
    //this->annotationMenu->setChangeEventListener(this);
    this->annotationMenu->addChangeListener(this);
}

void IRVideoAnnotater::closeAnnotationMenu ()
{
    this->annotationMenu.reset();
}

void IRVideoAnnotater::addEventButtonAction ()
{
    openAnnotationMenu();
}

void IRVideoAnnotater::deleteEventButtonAction ()
{
    deleteSelectedEvents();
}

void IRVideoAnnotater::playPositionChangedBySliderAction()
{
    std::cout << "playPositionChangedBySliderAction\n";
    stopAction();
    
    float p = this->videoTransport.getPlayPosition();
    //this->myVideoPlayerObject->getVideoPlayer()->setPlayPosition(p);
    this->myVideoPlayerObject->setPlayPosition(p);
    
    std::cout << "workspace\n";

    // first reset animated Object
    //this->workspace->resetAnimatedObjectList();
    // and then, re-animate the corresponding objects
    updateWorkspaceWithCurrentPlayingPosition(p);
    
    std::cout << "end\n";

}

void IRVideoAnnotater::playAction()
{
    this->workspace->setEditMode(false);
    this->myVideoPlayerObject->play();
}
void IRVideoAnnotater::stopAction()
{
    this->workspace->setEditMode(true);
    this->myVideoPlayerObject->stop();

}
// ==================================================

void IRVideoAnnotater::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == &this->videoTransport)
    {
        videoTransportChangeListener();
    }
    
    if(this->annotationMenu.get() != nullptr)
    {
        if(source == this->annotationMenu.get())
        {
            annotationMenuChangeListener();
        }
    }
    
}

void IRVideoAnnotater::setEventModifiedCallback(std::function<void(VideoAnnotationEventComponent*)> callback)
{
    this->eventModifiedCallback = callback;
}

// ==================================================

void IRVideoAnnotater::eventModifiedAction(Component* modifiedEvent)
{
    
    auto event = dynamic_cast<VideoAnnotationEventComponent* >(modifiedEvent);
    jassert(event != nullptr);
    
    // stop playing video first
    //this->videoPlayerObject->stop();
    this->myVideoPlayerObject->stop();
    
    //resize to sort
    auto currentPosition = this->eventListComponent->getViewPosition();
    this->eventListComponent->resized();
    this->eventListComponent->setViewPosition(currentPosition);
    
    // apply
    applyEventsOnTheLoadedVideo(event);
    /*
    if(this->eventModifiedCallback != nullptr)
        this->eventModifiedCallback(event);
     
     */
}

void IRVideoAnnotater::eventSelectedAction(Component* selectedEvent)
{
    std::cout << "selectedEvent = " << selectedEvent << std::endl;
    auto event = static_cast<VideoAnnotationEventComponent* >(selectedEvent);
    jassert(event != nullptr);
    
    // select object
    this->workspace->deselectAllObjects();
    std::cout<< "nodeobj = " << event->getNodeObject() << std::endl;
    
    if(this->workspace->isEditMode())
    {
        event->getNodeObject()->setSelected(true);
    }
    using s = VideoAnnotationEventComponent::VideoAnnotationType;
       switch (event->getType())
       {
           case s::TEXT:
               textEventComponentSelected(event);
               break;
           case s::SHAPE:
               shapeEventComponentSelected(event);
               break;
           case s::IMAGE:
               break;
           case s::AUDIO:
               break;
           default:
               break;
       }
    
    
    //this->videotransport->setCurrentPlayingPosition
}

void IRVideoAnnotater::updateAnimation()
{
    this->workspace->updateCurrentAnimation();
}

void IRVideoAnnotater::showEventPosition(Component* event)
{
    // first deselect all objects
    deselectAllObjectsOnWorkspace();
    
    auto e = static_cast<VideoAnnotationEventComponent* >(event);
    
    this->videoTransport.setCurrentPlayingPosition(e->getBeginTimeCode());
    this->workspace->setCurrentTimeCode(e->getBeginTimeCode());
    updateAnimation();
    
    //e->getNodeObject()->setSelected(true);
}
// ==================================================

void IRVideoAnnotater::textEventComponentSelected(VideoAnnotationEventComponent* event)
{
    //auto e = static_cast<AnnotationTextEventComponent* >(event);
    
    //auto c = static_cast<IRVideoAnnotationTextComponent*>(this->myVideoPlayerObject->getVideoAnnotationComponentOf(event));
    //this->eventLogList->setLogComponent(c->getIRTextEditorObject()->getObjController());
}

void IRVideoAnnotater::shapeEventComponentSelected(VideoAnnotationEventComponent* event)
{
    auto e = static_cast<AnnotationShapeEventComponent* >(event);

}

void IRVideoAnnotater::videoTransportChangeListener()
{
    auto status = this->videoTransport.getStatus();
    switch (status)
    {
        case IRVideoTransport::OpenVideoFile:
            openFile();
            break;
        case IRVideoTransport::OpenAnnotationFile:
            openSRTs();
            break;
        case IRVideoTransport::SaveAnnotationFile:
            saveSRTs();
            break;
        case IRVideoTransport::addEventButtonClicked:
            addEventButtonAction();
            break;
        case IRVideoTransport::deleteEventButtonClicked:
            deleteEventButtonAction();
            break;
        case IRVideoTransport::play:
            playAction();
            break;
        case IRVideoTransport::stop:
            stopAction();
            break;
        case IRVideoTransport::playPositionChanged:
            playPositionChangedBySliderAction();
            break;
        default:
            break;
    }
}

void IRVideoAnnotater::annotationMenuChangeListener()
{
    using s = VideoAnnotationMenuWindow::VideoAnnotationStatus;
    switch (this->annotationMenu->getStatus())
    {
        case s::CreateTextEvent:
            createTextEventComponent();
            break;
        case s::CreateShapeEvent:
            createShapeEventComponent();
            break;
        case s::CreateImageEvent:
            createImageEventComponent();
            break;
        case s::CreateAudioEvent:
            createAudioEventComponent();
            break;
        default:
            break;
    }
}

// ==================================================
void IRVideoAnnotater::clearAllEventComponent()
{
    this->eventListComponent->clearAllEventComponent();
}

void IRVideoAnnotater::clearEventComponent(VideoAnnotationEventComponent* eventComponent)
{

}

void IRVideoAnnotater::addEventComponent(VideoAnnotationEventComponent* eventComponent)
{
    this->eventListComponent->createEventComponent(eventComponent);
    
    // call event
    eventSelectedAction(eventComponent);
}

void IRVideoAnnotater::deleteSelectedEvents()
{
    auto events = this->eventListComponent->getSelectedEventComponents();
    
    for(auto e : events)
    {
        std::cout << "Selected List component = " << e << std::endl;
        if(e->getNodeObject()->isSelected())
        {
            this->eventLogList->removeLogComponent();
        }
        this->workspace->deleteObject(e->getNodeObject());
    }

    this->eventListComponent->deleteSelectedEventComponent();
    
    deleteEventOnTheLoadedVideo();
    //eventModifiedAction();
}

void IRVideoAnnotater::deleteEventComponent(VideoAnnotationEventComponent* event)
{
    this->eventListComponent->deleteEventComponent(event);
    deleteEventOnTheLoadedVideo();
    this->eventListComponent->newEventAdded();
}


// ==================================================

// ==================================================

void IRVideoAnnotater::closeAnnotationWindow()
{
    if(this->closeAnnotationWindowCallback != nullptr)
        this->closeAnnotationWindowCallback();
}

// ==================================================

void IRVideoAnnotater::updateVideoPlayerOfThis()
{
    /*
    std::cout << "updateVideoPlayerOfThis\n";
    if(this->videoPlayerObject != nullptr)
    {
        if(this->myVideoPlayerObject.get() != nullptr)
        {
            this->videoPlayerObject->shareContentsWith(this->myVideoPlayerObject.get());
        this->videoTransport.setVideoLengthInSec(this->videoPlayerObject->getVideoPlayer()->getVideoLength());
            resized();
        }
    }*/
    
}
void IRVideoAnnotater::updateVideoPlayerOfWorkspace()
{
    /*
    std::cout << "updateVideoPlayerOfWorkspace\n";
    if(this->videoPlayerObject != nullptr)
    {
        if(this->myVideoPlayerObject.get() != nullptr)
            this->myVideoPlayerObject->shareContentsWith(this->videoPlayerObject);

    }*/
}
// ==================================================

void IRVideoAnnotater::applyEventsOnTheLoadedVideo(VideoAnnotationEventComponent* event)
{
    if(this->myVideoPlayerObject.get() == nullptr) return;
    
    //this->myVideoPlayerObject->updateEventComponent(event);
    //this->videoPlayerObject->updateEventComponent(event);

}

void IRVideoAnnotater::createEventOnTheLoadedVideo(VideoAnnotationEventComponent* event)
{
    if(this->myVideoPlayerObject.get() == nullptr) return;
    
    this->myVideoPlayerObject->createAnnotationComponent(event);
    
}

void IRVideoAnnotater::deleteEventOnTheLoadedVideo()
{
    if(this->myVideoPlayerObject.get() == nullptr) return;
    // stop playing video first
    //this->videoPlayerObject->stop();
    this->myVideoPlayerObject->stop();
    
    //resize to sort
    auto currentPosition = this->eventListComponent->getViewPosition();
    this->eventListComponent->resized();
    this->eventListComponent->setViewPosition(currentPosition);
    
    auto events = this->eventListComponent->getEventComponents();
        
    //first update the videoObject on the Annotater
    this->myVideoPlayerObject->setAnnotationEvents(events);
    //this->videoPlayerObject->setAnnotationEvents(events);
    
}


// ==================================================

void IRVideoAnnotater::nodeObjectSelectionChange(IRNodeObject* obj)
{
    std::cout << "nodeObjectSelectionChange "<< obj << " : " << obj->isSelected()<< std::endl;
    
    // return when obj is DESELECTED
    if(!obj->isSelected()) return;
    
    this->eventLogList->setLogComponent(obj->getObjController());

    VideoAnnotationEventComponent* event = static_cast<VideoAnnotationEventComponent*>(obj->getEventComponent());
    
    this->eventListComponent->deSelectAllEventComponents();
    
    std:: cout << "event select  = "<< event << std::endl;
    if(event != nullptr){
        this->eventListComponent->selectEventComponent(event);
        //this->videoTransport.setCurrentPlayingPosition(obj->getBeginTimeCode());
        
        //this->myVideoPlayerObject->setPlayPosition(obj->getBeginTimeCode());
    }
    //event->setSelected(true);
    
  
    
}
void IRVideoAnnotater::nodeObjectGetFocused(IRNodeObject* obj)
{
    
}
void IRVideoAnnotater::editModeChanged(IRWorkspaceComponent* changedSpace)
{
    
}
void IRVideoAnnotater::heavyObjectCreated(IRNodeObject* obj)
{
    
}

void IRVideoAnnotater::deselectAllObjectsOnWorkspace()
{
    this->workspace->deselectAllObjects();
}

// ==================================================
