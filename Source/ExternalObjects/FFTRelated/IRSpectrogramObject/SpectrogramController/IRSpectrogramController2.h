//
//  IRSpectrogramController2.h
//  IRiMaS
//
//  Created by Keitaro on 26/11/2019.
//

#ifndef IRSpectrogramController2_h
#define IRSpectrogramController2_h

#include "IRObjectController.hpp"
#include "IRColourSelector.h"
#include "AudioObjectController.h"

class IRSpectrogramController2 : public IRObjectController,
public ChangeBroadcaster
{
public:
    
    enum IRSpectrogramController2Status
    {
        OpenAudioFile,
        PlayWhole,
        Stop,
        Pause,
        NONE
        
    };
    
    
    IRSpectrogramController2(IRStr* str) : IRObjectController("Spectrogram", str),
    audioController(str)
    {
        addAndMakeVisible(&this->LabelTitle);
        this->LabelTitle.setText("Spectrogram", dontSendNotification);
        this->LabelTitle.setFont(getStr()->SYSTEMCOLOUR.h3);
        this->LabelTitle.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
        this->LabelTitle.setJustificationType(Justification::left);
        
        addAndMakeVisible(&this->audioController);

    }
    ~IRSpectrogramController2()
    {
        
    }

    // ==================================================
    
    void ControllerResized() override
    {
        int y = 10;
        //int yIncrement = 30;
        int yBigIncrement = 40;
        
        this->LabelTitle.       setBounds(10, y, 300, 30);
        y += yBigIncrement;
        this->audioController.setBounds (0, y, getWidth(), 300);
    }
    void paint(Graphics& g) override
    {
        g.fillAll(getStr()->SYSTEMCOLOUR.contents);
        g.setColour(getStr()->SYSTEMCOLOUR.fundamental);
        g.drawLine(0,42.5,getWidth(),42.5, 2);
    }
    
    // ==================================================

        
    IRSpectrogramController2Status getStatus() const { return this->status; }
    
    AudioObjectController audioController;

    
private:
    
    
    // ==================================================
    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        
    }

    
    // ==================================================

    IRSpectrogramController2Status status;
    
    Label LabelTitle;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRSpectrogramController2)

};
#endif /* IRSpectrogramController2_h */
