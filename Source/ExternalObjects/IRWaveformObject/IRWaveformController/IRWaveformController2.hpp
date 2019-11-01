//
//  IRWaveformController2.hpp
//  IRiMaS
//
//  Created by Keitaro on 10/10/2019.
//

#ifndef IRWaveformController2_hpp
#define IRWaveformController2_hpp
#include "IRObjectController.hpp"
#include "IRColourSelector.h"
#include "AudioObjectController.h"

class IRWaveformController2 : public IRObjectController,
public ChangeListener,
public ChangeBroadcaster
{
public:
    
    enum IRWaveformController2Status
    {
        OpenAudioFile,
        PlayWhole,
        Stop,
        Pause,
        NONE
        
    };
    
    
    IRWaveformController2(IRStr* str);
    ~IRWaveformController2();

    // ==================================================
    
    void resized() override;
    void paint(Graphics& g) override;
    
    // ==================================================

        
    IRWaveformController2Status getStatus() const { return this->status; }
    
    AudioObjectController audioController;

    
private:
    
    
    // ==================================================
    void changeListenerCallback(ChangeBroadcaster* source) override;

    
    // ==================================================

    IRWaveformController2Status status;
    
    Label LabelTitle;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRWaveformController2)

};
#endif /* IRWaveformController2_hpp */
