
#ifndef IRWaveform_hpp
#define IRWaveform_hpp

#include "IRFoundation.h"
#include "IRAudioReader.hpp"
#include "SoundPlayer.hpp"


class IRWaveform : public AudioAppComponent,
                   private ChangeListener,
                   public ChangeBroadcaster,
                   IRAudio::Listener
{
    
public:
    
    IRWaveform();
    ~IRWaveform();
    
    void openFile();
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void fileImportCompleted(IRAudio *obj) override;
    void fileStatusChanged(IRAudio *obj) override;
    
    virtual void resized() override;
    void paint(Graphics& g) override;
    
    void setStart(double ms);
    double getStart() const;
    void setDisplayDuration(double ms);
    double getDisplayDuration() const;
    double getTotalLength() const;
    void setChannelIndex(int channelIndex);
    void setZoomFactor(double zoomFactor);
    double getZoomFactor() const;
    double getSampleRate() const;
    bool isWaveformPainted() const;
    
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    virtual void releaseResources() override;
    
    void play(int start, int duration, int offset, bool looping);
    void stop();
    void pausing();
    
    SoundPlayerClass* getPlayer() const;

    
    IRAudio audioFile;
    SoundPlayerClass *player;

    
private:
    
    TextButton openButton;

    bool drawWaveform = false;
    
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;
    
    // parameters
    double start = 0.0;
    double duration = 0.0;
    int channelIndex = 0;
    double zoomFactor = 1.0;
    
    //thread lock
    CriticalSection callbackLock;

    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

};


#endif /* IRWaveform_hpp */




