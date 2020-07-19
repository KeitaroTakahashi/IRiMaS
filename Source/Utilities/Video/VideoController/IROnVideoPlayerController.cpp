//
//  IROnVideoPlayerController.cpp
//  IRiMaS
//
//  Created by Keitaro on 30/06/2020.
//

#include "IROnVideoPlayerController.hpp"

IROnVideoPlayerController::IROnVideoPlayerController(IRStr* str) :
IRStrComponent(str),
IRHeavyWeightComponent(this, "IROnVideoPlayerController"),
playPauseButton(str),
chart(str),
timeCode(str, 0)
{
    addAndMakeVisible(&this->playPauseButton);
    this->playPauseButton.onClick = [this]{ playPauseButtonClickedAction(); };
    
    createTimeCodeSlider();
    createChart();
}

IROnVideoPlayerController::~IROnVideoPlayerController()
{
    
}
// ==================================================
void IROnVideoPlayerController::resized()
{
    int x = 10;
    int y = 10;
    
    int yIncrement = 30;
    
    
    
    int playCtlY = getHeight() - 50;
    
    this->playPauseButton.setBounds(x, y, 30, 30);
    this->playPauseButton.setCentrePosition(x + this->playPauseButton.getWidth()/2,
                                            playCtlY);
    
    
    int marginX = x + 35;
    
    //chart
    this->chart.setBounds(marginX + 2, 10, getWidth() - marginX - 14, 30);
    
    this->timeCodeSlider.setBounds(marginX - 5,
                                   37,
                                   getWidth() - marginX, 20);
    
    this->timeCode.setBounds(0, 0, 110, 30);
    this->timeCode.setCentrePosition(getWidth()/2, getHeight() - 23);
    
    y += yIncrement;
    
}

void IROnVideoPlayerController::paint(Graphics& g)
{
    
    std::cout << "IROnVideoPlayerController paint\n";

    g.fillAll(Colours::transparentWhite);
    g.setColour(Colours::grey);
    g.fillRoundedRectangle(getLocalBounds().reduced(6).toFloat(), 2);
}

// ==================================================

void IROnVideoPlayerController::setVideoLengthInSec(float length)
{
    // stop app when legnth is under 0
    jassert(length < 0);

    this->videoLengthInSec = length;
    
    // reset
    this->timeCodeSlider.setRange(0.0, (double)length, 0.1);
    this->timeCodeSlider.setValue(0.0);
    this->timeCode.setVideoLengthInSec(length);


}

void IROnVideoPlayerController::setCurrentPlayPosition(float sec)
{
    this->timeCodeSlider.setValue(sec);
}


// ==================================================

void IROnVideoPlayerController::playAction()
{
    this->status = PLAY;
    sendChangeMessage();
}

void IROnVideoPlayerController::pauseAction()
{
    this->status = PAUSE;
    sendChangeMessage();
}

// ==================================================

void IROnVideoPlayerController::playPauseButtonClickedAction()
{
    auto status = this->playPauseButton.getStatus();
    
    switch(status)
    {
        case IRPlayPauseButton::PLAY:
            playAction();
            break;
        case IRPlayPauseButton::PAUSE:
            pauseAction();
            break;
        default:
            break;
    }
    
}
// ==================================================

void IROnVideoPlayerController::setStatus(IROnVideoPlayerControllerStatus status)
{
    this->status = status;
}

// ==================================================

void IROnVideoPlayerController::createTimeCodeSlider()
{
    this->timeCodeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    this->timeCodeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox,
                                            true, 0, 0);
    addAndMakeVisible(this->timeCodeSlider);
    this->timeCodeSlider.addListener(this);
    
    addAndMakeVisible(this->timeCode);
    
    
}

void IROnVideoPlayerController::createChart()
{
    addAndMakeVisible(&this->chart);
}

void IROnVideoPlayerController::sliderValueChanged (Slider *slider)
{
    std::cout << "sliderValueChanged\n";
    if(slider == &this->timeCodeSlider)
    {
        float val = slider->getValue();
        std::cout << "val = " << val << " : emitflag = " << emitChangeBroadcaster4CurrentPlayPosition << std::endl;
        this->currentPlayPosition = val;
        this->timeCode.setLabelVal(val);
        
        if(this->emitChangeBroadcaster4CurrentPlayPosition)
        {
            this->status = playPositionChanged;
            sendChangeMessage();
        }else{
            //reset
            this->emitChangeBroadcaster4CurrentPlayPosition = true;
        }
    }
}

void IROnVideoPlayerController::setSliderValue(float val, bool emitChangeBroadcaster)
{
    this->emitChangeBroadcaster4CurrentPlayPosition = emitChangeBroadcaster;
    this->timeCodeSlider.setValue(val);
}


// ==================================================
void IROnVideoPlayerController::addMouseListenerToChildren(MouseListener* l)
{
    this->timeCodeSlider.addMouseListener(l, true);
    this->timeCode.addMouseListener(l, true);
    this->playPauseButton.addMouseListener(l, true);
}
// ==================================================


void IROnVideoPlayerController::setAnnotationData(std::vector<AnnotationChart::annotationData> data)
{
    
    this->chart.init();
    this->chart.setVideoLength(videoLengthInSec);
    
    for(auto d : data)
    {
        this->chart.addEvent(d.beginTime, d.endTime);
    }
    this->chart.reCalc();
}
// ==================================================
