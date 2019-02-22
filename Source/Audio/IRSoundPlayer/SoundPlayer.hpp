//
//  IRSoundPlayer.hpp
//
// Advanced Sound Player consisting of Play, Pause, Stop, and Loop functions.
//  Created by Keitaro Takahashi on 17/08/2018.
//
//

#ifndef IRSoundPlayer_hpp
#define IRSoundPlayer_hpp

#include "IRAudioReader.hpp"
#include "IRNodeObject.hpp"


class SoundPlayerClass : public PositionableAudioSource, public ChangeBroadcaster
{
    //============================================================
    
public:
    SoundPlayerClass();
    ~SoundPlayerClass();
    
    //------------------------------------------------------------
    virtual void prepareToPlay(int samplesPerBlockExpected, double newSampleRate) override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    virtual void releaseResources() override {}
    
    //------------------------------------------------------------
    //controller
    // these controller will fire sendChangeMessage()
    
    void start();
    
    void pause();
    
    void stop();
    
    //------------------------------------------------------------
    // ===========================================================
    // parameters
    // ===========================================================
    // -----------------------------------------------------------
    // set an AudioBuffer to be played
    void setAudioBuffer(AudioBuffer<float>* audioBuffer, bool ownership, double sampleRate, std::vector<int> outputChannels);
    // -----------------------------------------------------------
    // @param1 startPosition in sample of audio data to be played
    // @param2 duration in sample of audio data to be played from startPosition
    // @param3 wait samples time of playing audio data
    // @param4 shouldLoop true = loop mode
    void setParameters(int startPosition, int playSamples, int offset, bool shouldLoop);
    // -----------------------------------------------------------
    // set outputChannels e.g. {0, 1} = 1 and 2 outputs
    void setOutputChannels(std::vector<int>outputChannels);
    // -----------------------------------------------------------
    // set position to play
    void setNextReadPosition(int64 newPosition) override;
    // ------------------------------------------------------------
    //get next read position in audio data in sample
    int64 getNextReadPosition() const override { return static_cast<int>(this->playPosition); }
    // ------------------------------------------------------------
    void setPlaySamples(int samples) { this->playSamples = samples; }
    // ------------------------------------------------------------
    //get total length of the audio data in sample
    int64 getTotalLength() const override { return static_cast<int64> (this->buffer->getNumSamples()); }
    // ------------------------------------------------------------
    //loop flag
    bool isLooping() const override { return this->looping; }
    void setLooping(bool shouldLoop) override { this->looping = shouldLoop; }
    // ------------------------------------------------------------
    // return true if the player has stopped because its input stream ran out of data
    bool hasStreamFinished() const noexcept { return this->inputStreamEOF; }
    bool isPlaying() const noexcept { return this->playing; }
    bool isPausing() const noexcept { return this->pausing; }
    // ------------------------------------------------------------
    //time scheduler. samples in audio processing time
    void setSchedulerInSample(double samples) { this->offset = samples; }
    void setSchedulerInSec(double second) { this->offset = second * this->sampleRate; }
    
    //============================================================
private:
    // holds a pointer to an object which can optionally be deleted when this pointer goes out of scope.
    OptionalScopedPointer<AudioBuffer<float>> buffer;
    
    // sample playing position
    int playPosition = 0;
    // parameter : duration to be played
    int playSamples = 0;
    // parameter : start position to be played
    int startPosition = 0;
    
    int offset = 0;
    // current reading position : offset + playPosition
    int currentOffsetPosition = 0;
    bool looping = false;
    std::vector<int> outputChannels;
    double sampleRate = 44100.0;
    
    //play status
    bool playing = false;
    bool stopped = true;
    bool pausing = false;
    bool afterPausing = false;
    bool inputStreamEOF = false;
    bool isPrepared = false;
    
    //other parameters
    float lastGrain = 1.0;
    float gain = 1.0;
    
    //thread lock
    CriticalSection callbackLock;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoundPlayerClass)
};

//==========================================================================

class AudioPlayer_threadSafe
: public    AudioAppComponent,
private   ChangeListener,
IRAudio::Listener
{
public:
    AudioPlayer_threadSafe();
    
    ~AudioPlayer_threadSafe()
    {
        
    }
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if(source == &this->player)
        {
            if(this->player.isPlaying())
                changeState(Playing);
            else if(this->player.isPausing()){
                // Stopped does not reset playingPosition but just freeze playing process
                changeState(Pausing);
            }else{
                //first call Stopping to reset playingPosition
                changeState(Stopping);
                //then call Stopped to stop
                changeState(Stopped);
            }
        }else if(source == &this->audioFile)
        {
            if(this->audioFile.isFileLoadCompleted)
            {
                
                
            }
            
            if(this->audioFile.isFileOpened)
            {
                
            }
        }
    }
    
    
    // AudioAppComponent
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        this->player.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override
    {
        this->player.getNextAudioBlock(bufferToFill);
    }
    virtual void releaseResources() override
    {
        this->player.releaseResources();
    }
    
    void resized() override
    {
        int numButton = 5;
        
        //distance between buttons in y
        int dis = 2;
        int x2 = this->xMargin * 2;
        int w = getWidth() - x2;
        int h = (getHeight() - this->yMargin*2)/numButton - dis;
        
        int yPosition = this->yMargin + dis;
        this->openButton.setBounds  (this->xMargin, yPosition, w, h);
        yPosition += dis + h;
        this->playButton.setBounds  (this->xMargin, yPosition, w, h);
        yPosition += dis + h;
        this->loopButton.setBounds  (this->xMargin, yPosition, w, h);
        yPosition += dis + h;
        this->pauseButton.setBounds (this->xMargin, yPosition, w, h);
        yPosition += dis + h;
        this->stopButton.setBounds  (this->xMargin, yPosition, w, h);
        
    }
    
    void openFile()
    {
        if(this->audioFile.openFile()){
            this->stopButton.setEnabled(true);
            this->playButton.setEnabled(true);
        }else{
            std::cout << "Error : could not open\n";
        }
    }
    
    
    void openButtonClicked()
    {
        openFile();
    }
    
    void playButtonClicked()
    {
        this->player.start();
        changeState(Starting);
    }
    
    void loopButtonClicked()
    {
        bool flag = ! this->player.isLooping();
        this->player.setLooping(flag);
        changeState(Looping);
    }
    
    void stopButtonClicked()
    {
        this->player.stop();
        changeState(Stopping);
    }
    
    void pauseButtonClicked()
    {
        this->player.pause();
        changeState(Pausing);
    }
    
    
    //==========================================================================
    // Callback functions
    //==========================================================================
    
    
    // this method is called when the file import completed.
    virtual void fileImportCompleted(IRAudio *obj) override
    {
        //set audioBuffer to player
        std::vector<int>v = {0,1};
        this->player.setAudioBuffer(this->audioFile.getAudioBuffer(), false, this->audioFile.getSampleRate(),v);
        //this->player.setParameters(44100, 22050, 44100, false);
        //std::cout << " fileImported next position = " << this->player.getNextReadPosition() << std::endl;
    }
    
    //this method is called when file status changed
    virtual void fileStatusChanged(IRAudio *obj) override
    {
        
    }
    
    //==========================================================================
    //change labels
    
    
private:
    //==========================================================================
    
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Looping,
        Stopping,
        Pausing
    };
    
    void changeState (TransportState newState)
    {
        if(this->state != newState)
        {
            this->state = newState;
            switch(this->state)
            {
                case Stopped: // called when clickling a stop button or by callback
                    this->stopButton.setEnabled(false);
                    this->playButton.setEnabled(true);
                    this->pauseButton.setEnabled(false);
                    //reset playing position
                    this->player.setNextReadPosition(0);
                    break;
                case Starting:
                    this->playButton.setEnabled(false);
                    break;
                case Playing:
                    this->stopButton.setEnabled(true);
                    this->pauseButton.setEnabled(true);
                    break;
                case Looping:
                    this->loopButton.setEnabled(false);
                    break;
                case Stopping: // called when clicking a stop button
                    
                    this->player.setLooping(false);
                    this->loopButton.setEnabled(true);
                    break;
                case Pausing:
                    this->pauseButton.setEnabled(false);
                    this->playButton.setEnabled(true);
                    this->stopButton.setEnabled(true);
                    break;
            }
        }
    }
    //==========================================================================
    
    IRAudio audioFile;
    SoundPlayerClass player;
    
    TransportState state;
    
    //buttons
    TextButton playButton;
    TextButton openButton;
    TextButton stopButton;
    TextButton pauseButton;
    TextButton loopButton;
    
    int buttonWidth = 90;
    int buttonHeight = 20;
    int xMargin = 5;
    int yMargin = 4;
    
    
    // ===========================================================================
    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

};



#endif /* IRSoundPlayer_hpp */
