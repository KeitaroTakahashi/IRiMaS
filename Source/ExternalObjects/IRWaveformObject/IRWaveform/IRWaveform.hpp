
#ifndef IRWaveform_hpp
#define IRWaveform_hpp

#include "IRFoundation.h"
#include "IRUIAudioFoundation.hpp"
#include "IRAudioReader.hpp"
#include "SoundPlayer.hpp"


class IRWaveform : public IRUIAudioFoundation,
                   private ChangeListener,
                   public ChangeBroadcaster
{
    
public:
    
    IRWaveform(IRNodeObject* parent);
    ~IRWaveform();
    
    void openFile();
    void openFile(String path);

    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void fileImportCompleted();
    void getFilePtr(File file);
    void makeThumbnail(String path);
    
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
    
    // ==================================================
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    virtual void releaseResources() override;
    // ==================================================

    void play(int start, int duration, int offset, bool looping);
    void stop();
    void pausing();
    
    SoundPlayerClass* getPlayer() const;

    String getPath() const { return this->path; }
    
    DataAllocationManager<IRAudio>* audioData = nullptr;

    //IRAudio audioFile;
    SoundPlayerClass *player;
    
    
    // ==================================================
    // link system
    virtual void audioPtrDelivery(IRAudio *obj) override;

    // ==================================================
    
    void deinitializeAudioData();

private:
    
    File file;
    String path;
    IRNodeObject* parent;
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




