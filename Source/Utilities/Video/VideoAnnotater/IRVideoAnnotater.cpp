//
//  IRVideoAnnotater.cpp
//  IRiMaS
//
//  Created by Keitaro on 07/12/2019.
//

#include "IRVideoAnnotater.hpp"

IRVideoAnnotater::IRVideoAnnotater(IRStr* str, IRVideoAnnotaterObject2* videoPlayerObject) :
videoArea(10, 10, 640, 480),
videoPlayerObject(videoPlayerObject),
ir_parentStr(str)
{
   setWantsKeyboardFocus(true);

    initialize();
    
    // setup delegate
    this->delegate = new IRVideoAnnotaterDelegate(this);

    createVideoTransport();
    createEventListComponent();
    this->eventLogList.reset(new EventLogList(str));
    addAndMakeVisible(this->eventLogList.get());
    
   
}

IRVideoAnnotater::~IRVideoAnnotater()
{
    closeAnnotationMenu();
        
    this->myVideoPlayerObject.reset();

    this->eventListComponent.reset();
    
    delete this->delegate;
    
}


// ==================================================
void IRVideoAnnotater::paint(Graphics& g)
{
    g.fillAll(this->ir_str->SYSTEMCOLOUR.fundamental);
    g.setColour(this->ir_str->SYSTEMCOLOUR.contents);
    g.fillRect(this->videoArea);
    g.setColour(this->ir_str->SYSTEMCOLOUR.contents);
    g.fillRect(this->workArea);
    
}

void IRVideoAnnotater::resized()
{
    int xMarge = 10;
    int yMarge = 10;
    
    int eventLogListWidth = 350;
    this->videoArea = Rectangle<int>(xMarge, yMarge, getWidth() - eventLogListWidth, getHeight() * 0.5);
    int ha = this->videoArea.getY() + this->videoArea.getHeight() + 5;
    
    this->workArea  = Rectangle<int>(xMarge,
                                     ha + 45,
                                     getWidth() - 20,
                                     getHeight() - (ha + 55));
    
    videoResized();
    
    this->videoTransport->setBounds(xMarge,
                                   ha,
                                   getWidth() - 20,
                                   40);
    
    //this->openVideoButton.setBounds(this->videoArea);
    
    this->eventListComponent->setBounds(this->workArea);
  
   
    this->eventLogList->setBounds(xMarge + xMarge+ this->videoArea.getWidth(),
                                  yMarge,
                                  eventLogListWidth - xMarge - xMarge,
                                  getHeight() * 0.5);
    

    
    eventComponentResized();
}

void IRVideoAnnotater::videoResized()
{
    if(this->myVideoPlayerObject.get() != nullptr)
    {
        
        std::cout << "IRVideoAnnotater::videoResized\n";
        this->myVideoPlayerObject->resizeAndCentredThisComponent(this->videoArea);
        
       
        /*
        auto vp = this->myVideoPlayerObject->getVideoPlayerObject();
        
        vp->resizeThisComponent(Rectangle<int>(0, 0,
                                               this->videoArea.getWidth(),
                                               this->videoArea.getHeight()  ));
        */

        //this->workspace->setBounds(this->myVideoPlayerObject->getBounds());
        
    }
}
// ==================================================



void IRVideoAnnotater::initialize()
{
    
    //keyListener setup
    //setWantsKeyboardFocus(true);
    //addKeyListener(this);
    
    // create IRStr
    this->ir_str.reset(new IRStr());
    this->ir_str->projectOwner = this;
    this->ir_str->setKeyListener(this);
    this->ir_str->setMouseListener(this);
    this->ir_str->projectName = "VideoAnnotator";
    this->ir_str->SYSTEMCOLOUR = this->ir_parentStr->SYSTEMCOLOUR;
    this->ir_str->ICONBANK = this->ir_parentStr->ICONBANK;
    
    Font f;
    this->ir_str->fontFamilyList = f.findAllTypefaceNames();

}
// ==================================================

void IRVideoAnnotater::openAnnotaterWindowAction()
{
    if(this->myVideoPlayerObject.get() != nullptr)
    {
        this->myVideoPlayerObject->moveToFrontAction();
    }
}

// ==================================================

void IRVideoAnnotater::createVideoTransport()
{
    // video transport
    this->videoTransport.reset( new IRVideoTransport(this->ir_str.get(), this));
    addAndMakeVisible(this->videoTransport.get());
    this->videoTransport->addChangeListener(this);
    setVisible(true);
}

void IRVideoAnnotater::createEventListComponent()
{
    // eventListComponent
    this->eventListComponent.reset(new VideoEventList(this->ir_str.get(), this));
    addAndMakeVisible(this->eventListComponent.get());

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
    
    copyNodeObject(obj);
}

void IRVideoAnnotater::nodeObjectWillDeleted(IRNodeObject* obj)
{
    auto event = static_cast<VideoAnnotationEventComponent* >(obj->getEventComponent());
    
    std::cout << obj << " nodeObejctWillDeleted , event delete " << event << std::endl;
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
        std::cout << "IRVideoAnnotater create myVideoPlayerObject\n";
        // create IRVideoPlayerObject without OpenButton

        this->myVideoPlayerObject.reset( new IRVideoAnnotaterObject2(
                                                                    this,
                                                                     this->ir_str.get(),
                                                                    false));
        this->myVideoPlayerObject->getWorkspace()->enableDrawGrids(true);
        this->myVideoPlayerObject->getWorkspace()->addListener(this);
        //this->myVideoPlayerObject->getWorkspace()->registerKeyListener(this);
        addKeyListener(this->myVideoPlayerObject->getWorkspace());
        //this->myVideoPlayerObject->addKeyListener(this);

        this->myVideoPlayerObject->videoLoadCompletedCallbackFunc = [this] { myVideoLoadCompleted(); };
        this->myVideoPlayerObject->videoPlayingUpdateCallbackFunc = [this](double pos){ myVideoPlayingUpdate(pos); };
        // disable QT controller
        this->myVideoPlayerObject->enableController(false);

        
        //this->videoPlayerObject->copyContents(this->myVideoPlayerObject.get());
        // anotater reference video is not selectable.
        this->myVideoPlayerObject->setEnableResizingSquare(false);
        this->myVideoPlayerObject->setMovable(false, false, false);
        this->myVideoPlayerObject->setResizable(false);
        this->myVideoPlayerObject->setObjectBounds(this->videoArea);
        addAndMakeVisible(this->myVideoPlayerObject.get());
        
        // if parent object on the workspace already has a video file, then load it
        
        if(this->videoPlayerObject != nullptr)
        {

            auto vp = this->videoPlayerObject->getVideoPlayerObject()->getVideoPlayer();
            
            std::cout << "videoPlayerObject : " << vp << " : hasVideo = " << vp->hasVideo() << std::endl;

            if(this->videoPlayerObject->hasVideo())
            {
                File f = vp->getMovieFile();
                
                std::cout << "bindVideo file = " << f.getFullPathName() << std::endl;
                this->myVideoPlayerObject->openFile(f);
            }
        }
        
        //add to workspace
        //this->workspace->createObject(this->myVideoPlayerObject.get());
        
        // set up workspace
        //this->myVideoPlayerObject->setFixObjectSizeRatioWithOriginalSize(false,                 this->myVideoPlayerObject->getBounds().toFloat());
        // set video size not videoPlayer Object size
        
        auto v = this->myVideoPlayerObject->getVideoPlayerObject()->getVideoPlayer();
        
        this->myVideoPlayerObject->setFixObjectSizeRatioWithOriginalSize(false, v->getCurrentVideoBounds().toFloat());
        
        
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

// --------------------------------------------------
void IRVideoAnnotater::openFile()
{
    if(this->myVideoPlayerObject == nullptr) return ;
    
    // update video on the workspace
    this->wantUpdateVideoWorkspace = true;
    
    this->myVideoPlayerObject->getVideoPlayerObject()->openFile();
    
    resized();
}

void IRVideoAnnotater::openFile(File file)
{
    if(this->myVideoPlayerObject == nullptr) return ;
    
    // update video on the workspace
    this->wantUpdateVideoWorkspace = true;
    
    this->myVideoPlayerObject->getVideoPlayerObject()->openFile(file);
    resized();
}
// --------------------------------------------------

void IRVideoAnnotater::openSRTs()
{
    //if(this->eventListComponent.get() != nullptr)
       // this->eventListComponent->openAnnotationFile();
    
  
    FileChooser chooser("Select a SRT file to load...",
                        {},
                        "*.srt", "*.srts");
    
    
    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        //this->SrtPath = file.getFullPathName();
        this->jsonManager.readSaveData(file.getFullPathName().toStdString());
        loadAndApplySRTs();
        
    }
    
    //this->srtFileLoader.openFile();
    //loadAndApplySRTs(this->srtFileLoader.getSrtData());
}

void IRVideoAnnotater::openSRTs(File file)
{
    //if(this->eventListComponent.get() != nullptr)
    //    this->eventListComponent->openAnnotationFile(file);
    //else std::cout << "error eventListComponent null\n";
    
    //this->SrtPath = file.getFullPathName();
    //loadAndApplySRTs(this->SrtPath);
    //this->srtFileLoader.openFile(file.getFullPathName());
    //loadAndApplySRTs(this->srtFileLoader.getSrtData());
    this->jsonManager.readSaveData(file.getFullPathName().toStdString());

    loadAndApplySRTs();
}

void IRVideoAnnotater::saveSRTs()
{
    if(this->srtSavePath.length() == 0)
    {
        /*
        if(this->eventListComponent.get() != nullptr){
            this->eventListComponent->saveAnnotationFile();
            
            this->srtSavePath = this->eventListComponent->getSrtSavePath();
        }*/
        
        openDialogtoSaveSRTs();
        
    }else{
        File file(this->srtSavePath);
        if(file.exists())
        {
            saveSRTs(file);
        }else{
            this->srtSavePath = "";
            saveSRTs();
        }
    }
}

void IRVideoAnnotater::saveSRTs(File file)
{
    //if(this->eventListComponent.get() != nullptr)
        //this->eventListComponent->saveAnnotationFile(file);
    //if(!file.exists()) return;
    
    //this->srt.open(file.getFullPathName().toStdString());

    
    //json11::json saveData = this->myVideoPlayerObject->getWorkspace()->makeSaveDataOfThis();
    
    
    
    
    
    json11::Json wsData;
    auto workspaces = this->myVideoPlayerObject->getWorkspace();
    wsData = workspaces->makeSaveDataOfThis();
    
    json11::Json saveData = json11::Json::object({
        {"IRVideoAnnotaterSaveData", wsData}
    });
    this->jsonManager.setData(saveData);
    this->jsonManager.writeSaveData(file.getFullPathName().toStdString());
    
    /*
    using t = VideoAnnotationEventComponent::VideoAnnotationType;
    
    for(auto obj : this->myVideoPlayerObject->getWorkspace()->getObjectList())
    {
        
        if(obj->getEventComponent() == nullptr)
        {
            std::cout << "Error : saveSTRs() eventComponent NULL!\n";
            KLib().showErrorMessage("Error : saveSTRs() : eventComponent is NULL!");
            break;
        }
        auto event = static_cast<VideoAnnotationEventComponent*>(obj->getEventComponent());
        auto id = event->getSRT();
        
        auto s = obj->getStatusStr();
          
        std::string contents = "{\"" + obj->name.toStdString() + "\": ";

        contents += "{\"ArrangeController\": ";
        contents += "{\"bounds\": [" + std::to_string(s->bounds.getX()) +
        ", " + std::to_string(s->bounds.getY()) +
        ", " + std::to_string(s->bounds.getWidth()) +
        ", " + std::to_string(s->bounds.getHeight()) + "], ";

        contents += "\"relativeBounds\": [" + std::to_string(s->relativeBounds.getX()) +
        ", " + std::to_string(s->relativeBounds.getY()) +
        ", " + std::to_string(s->relativeBounds.getWidth()) +
        ", " + std::to_string(s->relativeBounds.getHeight()) + "], ";

        contents += "\"wrap\": " + std::to_string(s->wrap) + ", ";

        contents += "\"wrapColour\": [" + std::to_string(s->wrapColour.getRed()) +
        ", " + std::to_string(s->wrapColour.getGreen()) +
        ", " + std::to_string(s->wrapColour.getBlue()) +
        ", " + std::to_string(s->wrapColour.getAlpha()) + "]}"; // ,

        //auto objSaveData = obj->saveThisToSaveData().dump();
        //contents += "{\"contents\": " + objSaveData + "}";

        contents += "}"; // close ArrangeController
        contents += "}"; // close Object

        srtWriter::SRT_STRUCT saveItem(id.beginTime,
                                       id.endTime,
                                       contents);



        this->srt.addItem(saveItem);
    }
    
    this->srt.close();*/
    
    
}

void IRVideoAnnotater::openDialogtoSaveSRTs()
{
    FileChooser chooser("Save irSrt file...",
                        {},
                        "");
    if(chooser.browseForFileToSave(true))
    {
        auto file = chooser.getResult();
        this->savePath = file.getFullPathName() + ".srt";
        saveSRTs(File(this->savePath));
    }
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
    
    videoResized();
    //disable controller
    //this->myVideoPlayerObject->enableController(false);
    
    //updateVideoSize(this->myVideoPlayerObject->getVideoSize());
    //update video length
    
    double len = this->myVideoPlayerObject->getVideoPlayerObject()->getVideoPlayer()->getVideoLength();
    
    std::cout << "loaded video length = " << len << std::endl;
    this->videoTransport->setVideoLengthInSec(len);
    
    videoResized();
    
    // update video on the workspace
    if(this->wantUpdateVideoWorkspace)
        updateVideoFileWorkspace();
    
    //reset
    this->wantUpdateVideoWorkspace = false;
}

void IRVideoAnnotater::updateVideoSize(juce::Point<int> newVideoSize)
{
    setVideoSize(newVideoSize);
}

void IRVideoAnnotater::myVideoPlayingUpdate(double pos)
{
    this->videoTransport->setCurrentPlayingPosition(pos);
    
    
    //updateWorkspaceWithCurrentPlayingPosition(pos);
}

void IRVideoAnnotater::updateWorkspaceWithCurrentPlayingPosition(float pos)
{
    // test
    std::cout << "updateWorkspaceWithCurrentPlayingPosition " << pos << std::endl;
    this->myVideoPlayerObject->getWorkspace()->enableTimeCodeAnimation(true);
    this->myVideoPlayerObject->getWorkspace()->setCurrentTimeCode(pos);
    //this->myVideoPlayerObject->getWorkspace()->updateCurrentAnimation();
    
    //this->workspace->enableTimeCodeAnimation(true);
   // this->workspace->setCurrentTimeCode(pos);
   // this->workspace->updateCurrentAnimation();
}


// ==================================================

void IRVideoAnnotater::openAnnotationMenu ()
{
    Rectangle<int> pos = getBounds();
    pos.setX(getScreenPosition().getX());
    pos.setY(getScreenPosition().getY());

    this->annotationMenu.reset(new VideoAnnotationMenuWindow(this->ir_str.get(),
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
    
    float p = this->videoTransport->getPlayPosition();
    //this->myVideoPlayerObject->getVideoPlayer()->setPlayPosition(p);
    this->myVideoPlayerObject->setPlayPosition(p);
   // this->myVideoPlayerObject->getWorkspace()->resetAnimatedObjectList();
    
    std::cout << "workspace\n";

    // first reset animated Object
    //this->workspace->resetAnimatedObjectList();
    // and then, re-animate the corresponding objects
    updateWorkspaceWithCurrentPlayingPosition(p);
    
    std::cout << "end\n";

}

void IRVideoAnnotater::playAction()
{
   // this->workspace->setEditMode(false);
    this->myVideoPlayerObject->play();
}
void IRVideoAnnotater::stopAction()
{
    //this->workspace->setEditMode(true);
    this->myVideoPlayerObject->stop();

}
// ==================================================

void IRVideoAnnotater::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == this->videoTransport.get())
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
    std::cout << "IRVideoAnnotater::eventModifiedAction\n";
    auto event = dynamic_cast<VideoAnnotationEventComponent* >(modifiedEvent);
    jassert(event != nullptr);
    
    // stop playing video first
    //this->videoPlayerObject->stop();
    //this->myVideoPlayerObject->stop();
    
    //resize to sort
    auto currentPosition = this->eventListComponent->getViewPosition();
    this->eventListComponent->resized();
    this->eventListComponent->setViewPosition(currentPosition);
    
    // apply
    //applyEventsOnTheLoadedVideo(event);
    
    updateAnnotationWorkspace();
    
}

void IRVideoAnnotater::eventSelectedAction(Component* selectedEvent)
{
    std::cout << "selectedEvent = " << selectedEvent << std::endl;
    auto event = static_cast<VideoAnnotationEventComponent* >(selectedEvent);
    jassert(event != nullptr);
    
    // select object
    //this->workspace->deselectAllObjects();
    std::cout<< "nodeobj = " << event->getNodeObject() << std::endl;
    /*
    if(this->workspace->isEditMode())
    {
        event->getNodeObject()->setSelected(true);
    }*/
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

void IRVideoAnnotater::updateAnnotation()
{
        //updateAnnotationWorkspace();
    
}

void IRVideoAnnotater::showEventPosition(Component* event)
{
    // first deselect all objects
    deselectAllObjectsOnWorkspace();
    
    auto e = static_cast<VideoAnnotationEventComponent* >(event);
    
    this->videoTransport->setCurrentPlayingPosition(e->getBeginTimeCode());
    
    this->myVideoPlayerObject->getWorkspace()->setCurrentTimeCode(e->getBeginTimeCode());
    this->myVideoPlayerObject->setPlayPosition(e->getBeginTimeCode());
    
    e->getNodeObject()->setSelected(true);

}

void IRVideoAnnotater::eventActivationChanged(Component* changedEvent)
{
    
    std::cout << "eventActivationChanged\n";
    //auto e = static_cast<VideoAnnotationEventComponent* >(changedEvent);
    float p = this->videoTransport->getPlayPosition();

    this->myVideoPlayerObject->getWorkspace()->updateVideoPlayingPos(p);
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
    auto status = this->videoTransport->getStatus();
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
        
        deleteEventComponent(e);
        //this->eventListComponent->deleteEventComponent(e);
        
        
        //this->workspace->deleteObject(e->getNodeObject());
    }
    
    

    //this->eventListComponent->deleteSelectedEventComponent();
    
    //deleteEventOnTheLoadedVideo();
    //eventModifiedAction();
}

void IRVideoAnnotater::deleteEventComponent(VideoAnnotationEventComponent* event)
{
    
    this->eventListComponent->deleteEventComponent(event);
    
    //delete event;
    
    //deleteEventOnTheLoadedVideo();
    //this->eventListComponent->newEventAdded();
}


// ==================================================

// ==================================================

void IRVideoAnnotater::closeAnnotationWindow()
{
    if(this->closeAnnotationWindowCallback != nullptr)
        this->closeAnnotationWindowCallback();
}

// ==================================================

void IRVideoAnnotater::updateThisAnnotationWorkspace()
{
    if(this->videoPlayerObject == nullptr || this->myVideoPlayerObject.get() == nullptr) return;
}
void IRVideoAnnotater::updateThisVideoFile()
{
    if(this->videoPlayerObject == nullptr || this->myVideoPlayerObject.get() == nullptr) return;

    File f = this->videoPlayerObject->getVideoPlayerObject()->getVideoPlayer()->getMovieFile();
    
    if(f.exists()) this->myVideoPlayerObject->openFile(f);
    else std::cout << "IRVideoAnnotater::updateThisVideoFile file does not exist!" << std::endl;

}
void IRVideoAnnotater::updateAnnotationWorkspace()
{
    /*
    if(this->videoPlayerObject == nullptr || this->myVideoPlayerObject.get() == nullptr) return;
    
    this->videoPlayerObject->getWorkspace()->copyAllDataToWorkspace(this->myVideoPlayerObject->getWorkspace());
    this->videoPlayerObject->resized();*/

}
void IRVideoAnnotater::updateVideoFileWorkspace()
{
    if(this->videoPlayerObject == nullptr || this->myVideoPlayerObject.get() == nullptr) return;

    File f = this->myVideoPlayerObject->getVideoPlayerObject()->getVideoPlayer()->getMovieFile();
    this->videoPlayerObject->openFile(f);

}
// ==================================================

void IRVideoAnnotater::updateVideoPlayerOfThis()
{
    if(this->videoPlayerObject != nullptr)
    {
        if(this->myVideoPlayerObject.get() != nullptr)
        {
            updateThisVideoFile();
        }
    }
}
void IRVideoAnnotater::updateVideoPlayerOfWorkspace()
{
    if(this->videoPlayerObject != nullptr && this->myVideoPlayerObject.get() != nullptr)
    {
        //this->myVideoPlayerObject->shareContentsWith(this->videoPlayerObject);

    }
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
    //this->myVideoPlayerObject->setAnnotationEvents(events);
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
        //this->videoTransport->setCurrentPlayingPosition(obj->getBeginTimeCode());
        
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
    //this->workspace->deselectAllObjects();
    this->myVideoPlayerObject->getWorkspace()->deselectAllObjects();
}

// ==================================================

void IRVideoAnnotater::loadAndApplySRTs()
{
    
    
    
    
    /*
    for(auto item : data)
    {
        
        std::cout << "begin " << item->getStartTimeString() << " : end " << item->getEndTimeString() << std::endl;
        std::string text = item->getText();
        std::cout << "text  = " << item->getText() << std::endl;
        
        std::string err;
        json11::Json saveData = json11::Json::parse(text, err);
        
        std::cout <<"error = " << err << std::endl;
        
        auto j1 = saveData["IRTextEditor"];
        auto j2 = j1["ArrangeController"];
        
        std::cout << "get data as json : " << j2["bounds"][0].int_value() << std::endl;
        
        
        // if text
        std::string annotationData = saveData["IRTextEditor"].string_value();
        if(annotationData.size() > 0)
        {
            createTextEventComponentFromSRT(item);
        }
        
        // if shape
        annotationData = saveData["shape"].string_value();
        if(annotationData.size() > 0)
        {
            
        }
        
        // if image
        annotationData = saveData["image"].string_value();
        if(annotationData.size() > 0)
        {
            
        }
        
    }*/
    
}


// ==================================================



// ==================================================
