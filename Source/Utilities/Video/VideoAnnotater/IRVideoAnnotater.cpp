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
videoTransport(str)
{
    addAndMakeVisible(&this->openVideoButton);
    this->openVideoButton.setButtonText("Open Video Annotater");
    this->openVideoButton.onClick = [this] { openVideoButtonClicked(); };

    addAndMakeVisible(&this->videoTransport);
    this->videoTransport.addChangeListener(this);
    setVisible(true);
    
    this->eventListComponent.reset(new VideoEventList(str));
    addAndMakeVisible(this->eventListComponent.get());
    std::function<void()> callback = [this] { eventModifiedAction(); };
    this->eventListComponent->addEventModifiedCallback(callback);
    
    setWantsKeyboardFocus(true);
}

IRVideoAnnotater::~IRVideoAnnotater()
{
    closeAnnotationMenu();
    this->myVideoPlayerObject.reset();
    this->eventListComponent.reset();
    
    
    clearAllEventComponent();
}

// ==================================================
void IRVideoAnnotater::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);

    g.setColour(getStr()->SYSTEMCOLOUR.contents);
    
    g.fillRect(this->videoArea);
    
    g.fillRect(this->workArea);
    
}

void IRVideoAnnotater::resized()
{
    
    this->videoArea = Rectangle<int>(10, 10, getWidth() * 0.6666, getHeight() * 0.5);
    int ha = this->videoArea.getY() + this->videoArea.getHeight() + 5;
    this->workArea  = Rectangle<int>(10,
                                     ha + 45,
                                     getWidth() - 20,
                                     getHeight() - (ha + 55));
    
    videoResized();
    
    this->videoTransport.setBounds(10,
                                   ha,
                                   getWidth() - 20,
                                   40);
    
    this->openVideoButton.setBounds(10, ha, 100, 30);
    
    this->eventListComponent->setBounds(this->workArea);
    eventComponentResized();
}

void IRVideoAnnotater::videoResized()
{
    if(this->myVideoPlayerObject.get() != nullptr)
    {
        this->myVideoPlayerObject->setBounds(this->videoArea.getX(),
                                             this->videoArea.getY(),
                                             this->videoArea.getWidth(),
                                             this->videoArea.getHeight());
        
    }
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
        // create IRVideoPlayerObject without OpenButton
        this->myVideoPlayerObject.reset( new IRVideoAnnotaterObject(this->videoPlayerObject->getParent(),
                                                                getStr(),
                                                                 false));
        
        this->myVideoPlayerObject->videoLoadCompletedCallbackFunc = [this] { myVideoLoadCompleted(); };
        
        this->videoPlayerObject->copyContents(this->myVideoPlayerObject.get());

        this->myVideoPlayerObject->setMovable(false, false, false);
        this->myVideoPlayerObject->setResizable(false);
        
        addAndMakeVisible(this->myVideoPlayerObject.get());
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
    
}
// --------------------------------------------------

bool IRVideoAnnotater::hsaVideo() const { return this->isVideoLoaded; }


void IRVideoAnnotater::myVideoLoadCompleted()
{
    //disable controller
    this->myVideoPlayerObject->getVideoPlayer()->setNeedController(false);
    // update videoPlayerObject
    updateVideoPlayerOfWorkspace();
    //update video length
    this->videoTransport.setVideoLengthInSec(myVideoPlayerObject->getVideoPlayer()->getVideoLength());
    std::cout << myVideoPlayerObject->getVideoPlayer()->getVideoLength() << std::endl;
    resized();
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

void IRVideoAnnotater::playPositionChangedAction()
{
    float p = this->videoTransport.getPlayPosition();
    this->myVideoPlayerObject->getVideoPlayer()->setPlayPosition(p);
}

void IRVideoAnnotater::playAction()
{
    this->myVideoPlayerObject->play();
}
void IRVideoAnnotater::stopAction()
{
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

void IRVideoAnnotater::setEventModifiedCallback(std::function<void()> callback)
{
    this->eventModifiedCallback = callback;
}


void IRVideoAnnotater::eventModifiedAction()
{
    // stop playing video first
    this->videoPlayerObject->stop();
    this->myVideoPlayerObject->stop();
    
    // apply
    applyEventsOnTheLoadedVideo();
    
    
    if(this->eventModifiedCallback != nullptr)
        this->eventModifiedCallback();
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
            playPositionChangedAction();
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
}

void IRVideoAnnotater::deleteSelectedEvents()
{
    this->eventListComponent->deleteSelectedEventComponent();
    
    eventModifiedAction();
}

// ==================================================

void IRVideoAnnotater::createTextEventComponent()
{
    this->eventListComponent->createTextEventComponent();
    this->annotationMenu->closeAction();
}

void IRVideoAnnotater::createShapeEventComponent()
{
    this->eventListComponent->createShapeEventComponent();
    this->annotationMenu->closeAction();
}

void IRVideoAnnotater::createImageEventComponent()
{
    
}

void IRVideoAnnotater::createAudioEventComponent()
{
    
}
// ==================================================

bool IRVideoAnnotater::keyPressed(const KeyPress& key, Component* originatingComponent)
{
    if(key.getKeyCode() == key.deleteKey || key.getKeyCode() == key.backspaceKey)
    {
       DeleteKeyPressed();
        return true;
    }

    // open Annotation Event Menu
    if(key.getTextDescription() == "A")
    {
        AKeyPressed();
        return true;
    }
    
    // close window
    if(key.getTextDescription() == "Command + W")
    {
        AKeyPressed();
        return true;
    }

    return false;
}

void IRVideoAnnotater::DeleteKeyPressed()
{
    
}

void IRVideoAnnotater::AKeyPressed()
{
    openAnnotationMenu();
}

// ==================================================

void IRVideoAnnotater::updateVideoPlayerOfThis()
{
    if(this->videoPlayerObject != nullptr)
    {
        if(this->myVideoPlayerObject.get() != nullptr)
        {
            this->videoPlayerObject->shareContentsWith(this->myVideoPlayerObject.get());
            resized();
        }
    }
    
}
void IRVideoAnnotater::updateVideoPlayerOfWorkspace()
{
    if(this->videoPlayerObject != nullptr)
    {
        if(this->myVideoPlayerObject.get() != nullptr)
            this->myVideoPlayerObject->shareContentsWith(this->videoPlayerObject);

    }
    
}
// ==================================================

void IRVideoAnnotater::applyEventsOnTheLoadedVideo()
{
    if(this->myVideoPlayerObject.get() == nullptr) return;
    
    auto events = this->eventListComponent->getEventComponents();
    
    //first update the videoObject on the Annotater
    this->myVideoPlayerObject->setAnnotationEvents(events);
    
    //update the videoObject on the workspace
    updateVideoPlayerOfWorkspace();
}


// ==================================================
