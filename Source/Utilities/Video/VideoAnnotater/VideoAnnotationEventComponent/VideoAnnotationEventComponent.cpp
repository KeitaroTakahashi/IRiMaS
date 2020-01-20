//
//  VideoAnnotationEventComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 11/12/2019.
//

#include "VideoAnnotationEventComponent.hpp"

VideoAnnotationEventComponent::VideoAnnotationEventComponent(IRStr* str, int videoLengthInSecond) :
IRStrComponent(str),
timeCodeUI(str, videoLengthInSecond),
videoLengthInSecond(videoLengthInSecond)
{
    this->activeButton.setImage(getStr()->ICONBANK.icon_active.white);
    this->activeButton.setDrawCircle(true);
    this->activeButton.setDrawRoundedSquare(false);
    addAndMakeVisible(&this->activeButton);
    this->activeButton.onClick = [this]{ changeActive(); };
}

VideoAnnotationEventComponent::~VideoAnnotationEventComponent()
{
    
}
// ==================================================

void VideoAnnotationEventComponent::paint(Graphics& g )
{
    if(isSelected())
        g.fillAll(getStr()->SYSTEMCOLOUR.contents.brighter());
    else
        g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    
}

void VideoAnnotationEventComponent::resized()
{
    int margin = 5;
    int h = getHeight();
    int buttonSize = h - margin*2;
    this->activeButton.setBounds(getWidth() - margin*2 - buttonSize, margin, buttonSize, buttonSize);
}
// ==================================================

void VideoAnnotationEventComponent::mouseDown(const MouseEvent& e)
{
    
    std::cout << "VideoAnnotationEventComponent mouseDown\n";
    selectedAction();
}
// ==================================================
void VideoAnnotationEventComponent::setVideoLength(int videoLengthInSecond)
{
    this->videoLengthInSecond = videoLengthInSecond;
    
}

// ==================================================

void VideoAnnotationEventComponent::selectedAction()
{
    callEventComponentSelected();
}

void VideoAnnotationEventComponent::eventModified()
{
    callEventModified();
}


// ==================================================
// LISTENER
void VideoAnnotationEventComponent::callEventComponentSelected()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.eventComponentSelected(this); });
    //check again
    if(checker.shouldBailOut()) return;
}

void VideoAnnotationEventComponent::callEventModified()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.eventModified(this); });
    //check again
    if(checker.shouldBailOut()) return;
}


void VideoAnnotationEventComponent::callEventActiveChanged()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.eventActiveChanged(this); });
    //check again
    if(checker.shouldBailOut()) return;
}
// ==================================================

void VideoAnnotationEventComponent::changeActive()
{
    if(isActive())
    {
        setActive(false);
        this->activeButton.setImage(getStr()->ICONBANK.icon_active.gray);
        this->activeButton.setDrawColour(Colour(100, 100, 100));
    }else{
        setActive(true);
        this->activeButton.setImage(getStr()->ICONBANK.icon_active.white);
        this->activeButton.setDrawColour(Colour(255, 255, 255));

    }
    callEventActiveChanged();
}

// ==================================================
