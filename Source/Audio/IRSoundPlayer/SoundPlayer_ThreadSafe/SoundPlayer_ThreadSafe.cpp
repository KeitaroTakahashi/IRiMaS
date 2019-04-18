//
//  SoundPlayer_ThreadSafe.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 10/04/2019.
//

#include "SoundPlayer_ThreadSafe.hpp"

// =============================================================
//# AudioPlayer_threadSafe
// =============================================================
AudioPlayer_threadSafe::AudioPlayer_threadSafe(IRNodeObject* parent) :
IRUIAudioFoundation(parent),
player(parent)
{
    this->parent = parent;
    setBounds(0,0,100,98);
    
    addAndMakeVisible(&this->openButton);
    this->openButton.setButtonText("open");
    this->openButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.contents);
    this->openButton.setEnabled(true);
    this->openButton.onClick = [this]{ openButtonClicked(); };
    
    addAndMakeVisible(&this->playButton);
    this->playButton.setButtonText("Play");
    this->playButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.contents);
    this->playButton.setEnabled(false);
    this->playButton.onClick = [this]{ playButtonClicked(); };
    
    addAndMakeVisible(&this->loopButton);
    this->loopButton.setButtonText("Loop");
    this->loopButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.contents);
    this->loopButton.setEnabled(true);
    this->loopButton.onClick = [this]{ loopButtonClicked(); };
    
    addAndMakeVisible(&this->pauseButton);
    this->pauseButton.setButtonText("pause");
    this->pauseButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.contents);
    this->pauseButton.setEnabled(false);
    this->pauseButton.onClick = [this]{ pauseButtonClicked(); };
    
    addAndMakeVisible(&this->stopButton);
    this->stopButton.setButtonText("stop");
    this->stopButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.contents);
    this->stopButton.setEnabled(false);
    this->stopButton.onClick = [this]{ stopButtonClicked(); };
    
    this->audioFile.addListener(this);
    this->audioFile.addChangeListener(this);
    
    this->player.addChangeListener(this);
    
}
