//
//  IRSoundPlayer.hpp
//
// Advanced Sound Player consisting of Play, Pause, Stop, and Loop functions.
//  Created by Keitaro Takahashi on 17/08/2018.
//
//

#ifndef IRSoundPlayer_hpp
#define IRSoundPlayer_hpp

#include "IRNodeObject.hpp"
#include "IRAudio.hpp"


class SoundPlayerClass : public PositionableAudioSource,
                         public ChangeBroadcaster
{
    //============================================================
    
public:
    SoundPlayerClass(IRNodeObject* parent);

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
    
    int64 getStartPosition() const { return this->startPosition; }
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
    
    IRNodeObject* parent;

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


#endif /* IRSoundPlayer_hpp */
