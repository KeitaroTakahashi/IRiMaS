
#ifndef IRWaveform_hpp
#define IRWaveform_hpp

#include "IRFoundation.h"
#include "IRUIAudioFoundation.hpp"
#include "IRAudioReader.hpp"
#include "SoundPlayer.hpp"
#include "KeAnimationComponent.h"


class IRWaveform : public IRUIAudioFoundation,
                   private ChangeListener,
                   public ChangeBroadcaster,
public IRAudio::Listener,
public KeAnimationComponent
{
    
public:
    
    enum IRWaveformStatus
    {
        DRAGOUT,
        DROPOUT,
        EDITMODECHANGE,
        PROJECTSAVE,
        zoomInfoShared,
        currentPlayedFrameShared,
        viewPosShared
    };
    
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
    void createPlayingLine(int64 currentFrame);


    // ==================================================
    // link system
    virtual void audioPtrDelivery(IRAudio *obj) override;
    
    // ==================================================
    //sharedInfo
    
    void setZoomInfo(Point<float> zoom) {
        this->zoomInfo = zoom;
        if(this->audioData != nullptr)
        {
            auto data = this->audioData->getData();
            data->setZoomInfo(this->zoomInfo);
        }
    }
    
    void setZoomInfo(float w, float h) {
        this->zoomInfo = Point<float>(w, h);
        setZoomInfo(this->zoomInfo);
    }
    
    void linkZoomInfo(Component* comp);
    void linkViewPosition(Component* comp);
    
    Point<float> getZoomInfo() const { return this->zoomInfo; }
    
    void setCurrentPlayedFrame(int64 frame) { this->currentPlayedFrame = frame; }
    int getCurrentPlayedFrame() const { return this->currentPlayedFrame; }
    
    void linkCurrentPlayedFrame(Component* comp);
    
    void setVisiblePos(Point<int>pos) { this->visiblePos = pos; }
    Point<int> getVisiblePos() const { return this->visiblePos; }
    
    // called by IRAudio
    void zoomInOutOperatedFromComponent(IRAudio* obj) override;
    void audioPlayOperatedFromComponent(IRAudio* obj) override;
    void viewPortPositionFromComponent(IRAudio* obj) override;

    IRWaveformStatus getStatus() const { return this->status; }
    IRWaveformStatus status;

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
    
    Rectangle<int> playingLine;
    
    // ---------------------------------------------------------------------------
    // sharedInformation
    
    int64 currentPlayedFrame = 0;
    Point<int>visiblePos;
    Point<float> zoomInfo;
    
    // ---------------------------------------------------------------------------
    
    void updateAnimationFrame() override;
    // ---------------------------------------------------------------------------

    //thread lock
    CriticalSection callbackLock;

    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

};


#endif /* IRWaveform_hpp */




