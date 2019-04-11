//
//  IRSoundPlayer.cpp
//  MixerAudioSource_Study - App
//
//  Created by Keitaro on 17/08/2018.
//

#include "SoundPlayer.hpp"


SoundPlayerClass::SoundPlayerClass(IRNodeObject* parent)
{
    this->parent = parent;
}

SoundPlayerClass::~SoundPlayerClass()
{
    
}

// ----------------------------------------------------------------------------------------------------
void SoundPlayerClass::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    const ScopedLock sl (this->callbackLock); // lock thread
    this->inputStreamEOF = false;
    this->isPrepared = true;
}
// ----------------------------------------------------------------------------------------------------
void SoundPlayerClass::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    const ScopedLock sl (this->callbackLock); // lock thread
    
    if(this->buffer != nullptr && ! this->stopped)
    {
        bufferToFill.clearActiveBufferRegion();
        
        // get playSamples. Attention, this should not be the totalSamples of the audio data.
        const int bufferSize = this->playSamples;
        const int samplesNeeded = bufferToFill.numSamples;
        const int samplesToCpy = jmin (bufferSize - this->playPosition, samplesNeeded);
        
        // how many samples to be copied from the audio data over the time scheduling.
        int offsetLeft = 0;
        
        // check if there is any samples to play...
        if(samplesToCpy > 0)
        {
            int maxInChannels = this->buffer->getNumChannels();
            int maxOutChannels = bufferToFill.buffer->getNumChannels();
            //# Here, we fill each indicated channel of a bufferToFill.buffer.
            int channelCount = (int)this->outputChannels.size();
            // playing...
            // -------------------------------------------------
                // here only copying channels that are given.
                for(int i=0; i< channelCount; ++i)
                {
                    int chan = this->outputChannels[i];
                    if(chan < maxOutChannels)
                    {
                        // calculate remaining offset
                        int left = this->offset - this->currentOffsetPosition;

                        if(left >= samplesNeeded)
                            bufferToFill.buffer->clear(bufferToFill.startSample, bufferToFill.numSamples);
                        else
                        {
                            if(left > 0) // 0 < offsetLeft < samplesToCpy
                            {
                                bufferToFill.buffer->clear(bufferToFill.startSample, left);
                                bufferToFill.buffer->copyFrom(chan,
                                                              bufferToFill.startSample + left,
                                                              *this->buffer,
                                                              chan % maxInChannels,
                                                              this->startPosition + this->playPosition, // copy from playPosition
                                                              (samplesToCpy - left)); // copied samples
                                
                                offsetLeft = samplesToCpy - left;
                    
                            }else{
                            
                                bufferToFill.buffer->copyFrom(chan,
                                                              bufferToFill.startSample,
                                                              *this->buffer,
                                                              chan % maxInChannels,
                                                              this->startPosition + this->playPosition, // copy from playPosition
                                                              samplesToCpy); // copied samples
                                
                                offsetLeft = samplesToCpy;

                                // make ramp if this process is just after pausing in order to avoid producing a clip noise.
                                if(this->afterPausing)
                                {
                                    bufferToFill.buffer->applyGainRamp(chan, bufferToFill.startSample, jmin(256, bufferToFill.numSamples), 0.0f, 1.0f);
                                    this->afterPausing = false;
                                }
                            }
                        }

                    }
                }
            
                // playPosition increases only after the time scheduling over
                this->playPosition += offsetLeft;
                
                // currentOffsetPosition increases from the beginning.
                this->currentOffsetPosition += samplesNeeded;
                
                // if loop mode
                if(this->looping) this->playPosition %= bufferSize;
            
            // if the next NextReadPosition is beyond a total length of the audio data
            // and looping mode is false, do the following process.
            // This process operated when the input stream of data runs out.
                if(this->getNextReadPosition() >= this->playSamples && ! this->looping)
                {
                    this->playing = false;
                    this->inputStreamEOF = true;
                    //inform the status change
                    sendChangeMessage();
                }
            
           
            // -------------------------------------------------

            if(! this->playing) // stopping...
            {
                // make a short ramp to 0 if it does not reach to the end of the audio data in order to avoid producing a clip noise.
                for(int i=0; i< channelCount; i++)
                {
                    // again process only the given channels.
                    int chan = this->outputChannels[i];
                    if(chan < maxOutChannels)
                    {
                        // make ramp : very short fade out
                        bufferToFill.buffer->applyGainRamp(chan, bufferToFill.startSample, jmin(256, bufferToFill.numSamples), 1.0f, 0.0f);
                        
                        //if numSamples is larger than the ramp size, then do 0 padding over the ramp size.
                        if(bufferToFill.numSamples > 256)
                            bufferToFill.buffer->clear(bufferToFill.startSample + 256, bufferToFill.numSamples - 256);
                    }
                }
                
                // stopped
                this->stopped = true;
            }
        }
    }else
    {
        bufferToFill.clearActiveBufferRegion();
        //this->stopped = true;
        
    }
}
// ----------------------------------------------------------------------------------------------------
void SoundPlayerClass::setParameters(int startPosition, int playSamples, int offset, bool shouldLoop)
{
    setNextReadPosition(startPosition);
    jassert(playSamples >= 0);
    this->playSamples = playSamples;
    this->looping = shouldLoop;
}
// ----------------------------------------------------------------------------------------------------
void SoundPlayerClass::setOutputChannels(std::vector<int>outputChannels)
{
    this->outputChannels = outputChannels;
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
void SoundPlayerClass::setNextReadPosition(int64 newPosition)
{
    // check if newPosition is larger than 0
    jassert(newPosition >= 0);
    
    if(this->looping && this->buffer != nullptr)
        newPosition = newPosition % static_cast<int64> (this->buffer->getNumSamples());
    
    this->startPosition = jmin (this->buffer->getNumSamples(), static_cast<int> (newPosition));
    this->playPosition = 0;
    this->currentOffsetPosition = 0;
}
// ----------------------------------------------------------------------------------------------------
void SoundPlayerClass::setAudioBuffer(AudioBuffer<float>* audioBuffer, bool ownership, double sampleRate, std::vector<int> outputChannels)
{
    const ScopedLock sl (this->callbackLock);
    //here buffer is allocated
    this->buffer.set(audioBuffer, ownership);
    this->outputChannels = outputChannels;
    this->sampleRate = sampleRate;
    this->inputStreamEOF = false;
    
    //initialize playSamples with buffer samples
    this->playSamples = this->buffer->getNumSamples();
    
    this->currentOffsetPosition = 0;
    this->playPosition = 0;
}
// ----------------------------------------------------------------------------------------------------
void SoundPlayerClass::start()
{
    if((! this->playing) && this->buffer != nullptr)
    {
        //lock this procee
        {
            const ScopedLock sl (this->callbackLock);
            this->playing = true;
            this->stopped = false;
            this->inputStreamEOF = false;
            
            if(this->pausing)
            {
                this->afterPausing = true;
                this->pausing = false;
            }
        }
        // inform the status change
        sendChangeMessage();
    }
}
// ----------------------------------------------------------------------------------------------------
void SoundPlayerClass::pause()
{
    if(this->playing)
    {
        // lock this process
        {
            const ScopedLock sl (this->callbackLock);
            this->playing = false;
            this->pausing = true;
        }
        int n = 500;
        while (--n >= 0 && ! stopped)
            Thread::sleep(2);
        
        //inform the status change
        sendChangeMessage();
    }
}
// ----------------------------------------------------------------------------------------------------

void SoundPlayerClass::stop()
{
    if(this->playing)
    {
        // lock this process
        {
            const ScopedLock sl (this->callbackLock);
            this->playing = false;
            this->pausing = false;
            
            //reset for time scheduling
            this->currentOffsetPosition = 0;
            //setNextReadPosition(this->startPosition);
        }
        int n = 500;
        while (--n >= 0 && ! stopped)
            Thread::sleep(2);
        
        //inform the status change
        sendChangeMessage();
    }
}
// ----------------------------------------------------------------------------------------------------
