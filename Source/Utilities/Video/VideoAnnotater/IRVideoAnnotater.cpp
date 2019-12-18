//
//  IRVideoAnnotater.cpp
//  IRiMaS
//
//  Created by Keitaro on 07/12/2019.
//

#include "IRVideoAnnotater.hpp"

IRVideoAnnotater::IRVideoAnnotater(IRStr* str, IRVideoPlayerObject* videoPlayerObject) : IRStrComponent(str),
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
    
    if(this->myVideoPlayerObject.get() != nullptr)
        this->myVideoPlayerObject->setBounds(this->videoArea.getX(),
                                             this->videoArea.getY(),
                                             this->videoArea.getWidth(),
                                             this->videoArea.getHeight());
    
    this->videoTransport.setBounds(10,
                                   ha,
                                   getWidth() - 20,
                                   40);
    
    this->openVideoButton.setBounds(10, ha, 100, 30);
    
    this->eventListComponent->setBounds(this->workArea);
    eventComponentResized();
}

void IRVideoAnnotater::eventComponentResized()
{

}
// ==================================================

void IRVideoAnnotater::bindVideoPlayerObject()
{
    if(this->videoPlayerObject != nullptr)
    {
        // create IRVideoPlayerObject without OpenButton
        this->myVideoPlayerObject.reset( new IRVideoPlayerObject(this->videoPlayerObject->getParent(),
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
}

void IRVideoAnnotater::openFile(File file)
{
    this->myVideoPlayerObject->openFile(file);
}

bool IRVideoAnnotater::hsaVideo() const { return this->isVideoLoaded; }


void IRVideoAnnotater::myVideoLoadCompleted()
{
    // update videoPlayerObject
    updateVideoPlayerOfWorkspace();
}


// ==================================================

void IRVideoAnnotater::openAnnotationMenu ()
{
    std::cout << "openAnnotationMenu\n";
    
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
        std::cout <<"changeListner\n";
        if(source == this->annotationMenu.get())
        {
            std::cout <<"aa\n";
            annotationMenuChangeListener();
        }
    }
    
}

void IRVideoAnnotater::videoTransportChangeListener()
{
    auto status = this->videoTransport.getStatus();
    switch (status)
    {
        case IRVideoTransport::OpenVideoFile:
            openFile();
            break;
        case IRVideoTransport::addEventButtonClicked:
            addEventButtonAction();
            break;
        case IRVideoTransport::deleteEventButtonClicked:
            deleteEventButtonAction();
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
    
}

void IRVideoAnnotater::clearEventComponent(VideoAnnotationEventComponent* eventComponent)
{
   
}

void IRVideoAnnotater::addEventComponent(VideoAnnotationEventComponent* eventComponent)
{
    
}
// ==================================================

void IRVideoAnnotater::createTextEventComponent()
{
    std::cout << "IRVideoAnnotater::createTextEventComponent\n";
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
    std::cout << "keyPressed! IRVideoAnnotator\n";
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

// ==================================================
