//
//  IRVideoController.cpp
//  IRiMaS
//
//  Created by Keitaro on 24/10/2019.
//

#include "IRVideoPlayerController.hpp"


IRVideoPlayerController::IRVideoPlayerController(IRStr* str, IRVideoAnnotaterObject2* videoPlayerObject) : IRObjectController("VideoPlayer", str),
UI(str, videoPlayerObject)
{
    
    std::cout << "IRVideoPlayerController init\n";
    addAndMakeVisible(&this->LabelTitle);
    this->LabelTitle.setText("VideoPlayer", dontSendNotification);
    this->LabelTitle.setFont(getStr()->SYSTEMCOLOUR.h3);
    this->LabelTitle.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    this->LabelTitle.setJustificationType(Justification::left);
    
    addAndMakeVisible(&this->UI);
    this->UI.addChangeListener(this);
    
    this->arrangeController = std::make_unique<
    ArrangeController>(str);
    addAndMakeVisible(this->arrangeController.get());
    this->arrangeController->setVisible(false);
    this->arrangeController->addChangeListener(this);
}

IRVideoPlayerController::~IRVideoPlayerController()
{
    this->arrangeController.reset();
}
// ==================================================

void IRVideoPlayerController::ControllerResized()
{
    
    int y = 10;
    int yIncrement = 30;
    int yBigIncrement = 40;
    
    this->LabelTitle.       setBounds(10, y, getWidth() - 20, 30);
    
    y += yBigIncrement;
    y += yIncrement;
    this->UI.               setBounds(0, y, getWidth(), getHeight() - y);
    this->arrangeController->setBounds(0, y, getWidth(), getHeight() - y);
    
}
void IRVideoPlayerController::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    g.setColour(Colours::black);
    g.drawLine(0,42.5,getWidth(),42.5);
}
// ==================================================

void IRVideoPlayerController::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == &this->UI)
    {
        switch (this->UI.getStatus())
        {
            case VideoController::OpenVideoFile:
                this->status = OpenMovieFile;
                sendChangeMessage();
                break;
            case VideoController::OpenVideoAnnotater:
                this->status = OpenVideoAnnotater;
                sendChangeMessage();
                break;
            case VideoController::CloseVideoAnnotater:
                this->status = CloseVideoAnnotater;
                sendChangeMessage();
                break;
            case VideoController::ApplyAnnotation:
                this->status = ApplyAnnotation;
                sendChangeMessage();
                break;
            default:
                break;
        }
    }
}
// ==================================================

void IRVideoPlayerController::updateAnnotater()
{
    this->UI.updateAnnotater();
}

void IRVideoPlayerController::updateParentVideoPlayerObject()
{
    this->UI.updateParentVideoPlayerObject();
}
// ==================================================

ArrangeController* IRVideoPlayerController::getArrangeController()
{
    return this->arrangeController.get();
}


// ==================================================


void IRVideoPlayerController::mainControllerSelected()
{
    this->UI.setVisible(true);
    this->arrangeController->setVisible(false);
}

void IRVideoPlayerController::arrangeControllerSelected()
{
    this->UI.setVisible(false);
    this->arrangeController->setVisible(true);
}

// ==================================================
// ==================================================
