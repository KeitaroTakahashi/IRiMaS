//
//  IRAudioReader.hpp
//  Thread safe audio file reader
//
//  Created by Keitaro on 17/08/2018.
//

#ifndef IRAudioReader_hpp
#define IRAudioReader_hpp


#include "JuceHeader.h"


// ===========================================================================
// Thread Safe Reference Buffer class for Audio Reader : Do NOT REMOVE IT FROM HERE!
// ===========================================================================

class ReferenceCountedBuffer: public ReferenceCountedObject
{
public:
    typedef ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;
    ReferenceCountedBuffer (const String& nameToUse,
                            int numChannels,
                            int numSamples)
    : name(nameToUse),
    buffer(numChannels, numSamples)
    {
        DBG(String("Buffer named '") + name + "' constructed. numChannels = " + String(numChannels) + ", numSamples = " + String(numSamples));
    }
    
    ~ReferenceCountedBuffer()
    {
        DBG(String("Buffer named '") + name + "' destroyed");
    }
    
    AudioSampleBuffer* getAudioSampleBuffer()
    {
        return &buffer;
    }
    
    int position = 0;
    
private:
    String name;
    AudioSampleBuffer buffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReferenceCountedBuffer)
    
};

// ===========================================================================
// Thread Safe Reference Buffer class for Thumbnail Reader : Do NOT REMOVE IT FROM HERE!
// ===========================================================================

class ReferenceCountedThumbnail: public ReferenceCountedObject
{
public:
    typedef ReferenceCountedObjectPtr<ReferenceCountedThumbnail> Ptr;
    ReferenceCountedThumbnail (const String& nameToUse)
    : name(nameToUse),
    thumbnailCache(5),
    thumbnail(512, formatManager, thumbnailCache)
    {
        this->formatManager.registerBasicFormats();
        DBG(String("Thumbail named '") + name + "' constructed.");
    }
    
    ~ReferenceCountedThumbnail()
    {
        DBG(String("Thumbail named '") + name + "' destroyed");
    }
    
    AudioThumbnail* getThumbnail()
    {
        return &thumbnail;
    }
    
    int position = 0;
    
private:
    String name;
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReferenceCountedThumbnail)
    
};

// ===========================================================================
// import audio file
// ===========================================================================

class IRAudio : public Component,
                      public ChangeBroadcaster,
                      private Thread
{
public:
    // Constructing IRAudio and Thread class
    IRAudio();
    
    // stop thread processing...
    ~IRAudio();
    
    // clear ReferenceCountedBuffer::Ptr here
    void clear(ReferenceCountedBuffer::Ptr currentBuffer);
    
    // get Thumbnail of audio data, return nullptr if not exists
    AudioThumbnail* getThumbnail();
    
    // ===========================================================================
    // audio file reader
    // ------------------------------------------------------------------
    // open dialog to select a file
    bool openFile();
    // open audio file by path
    bool openFileFromPath(String path);
    // ------------------------------------------------------------------
    // getter of the audio file info
    // ------------------------------------------------------------------
    double getSampleRate() const { return this->sampleRate; }
    
    int getNumChannels() const { return this->numChannels; }
    
    int64 getNumSamples() const { return this->numSamples; }
    
    unsigned int getBitsPerSample() const { return this->bitsPerSample; }
    
    String getPath() const { return this->filePath; }
    
    String getFileName() const { return this->fileName; }

    // print audio file info for debug
    void showAudioFileInfo();
    // ------------------------------------------------------------------
    // getter of the audio file buffer
    // ------------------------------------------------------------------
    AudioSampleBuffer* getAudioBuffer(){ return this->buffer->getAudioSampleBuffer(); }
    
    // return audio buffer in vector<float> data type
    std::vector<float> getAudioBufferInVector();
    
    // ===========================================================================
    // Listener
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        // called when audio file has been loaded
        virtual void fileImportCompleted(IRAudio *obj) = 0;
        // called when file status changed (will be loaded, completed, deleted etc.)
        virtual void fileStatusChanged(IRAudio *obj) = 0;
    };
    
    void addListener(Listener* newListener) { this->ImportAudioListeners.add(newListener); }
    void removeListener(Listener* listener) { this->ImportAudioListeners.remove(listener); }
    
    // ---------------------------------------------------------------------------
    
    std::function<void()> onImportCompleted;
    std::function<void()> onFileStatusChanged;
    
    // ===========================================================================
    // public members
    bool isFileOpened = false;
    bool isFileLoadCompleted = false;
    
private:
    // ===========================================================================
    //thread related methods
    // ---------------------------------------------------------------------------
    // check
    void run() override;
    
    void checkForBuffersToFree();
    
    // impoart audio file and read the data
    void checkForPathToOpen();
    
    // ---------------------------------------------------------------------------
    
    String path;
    String filePath;
    String fileName;
    double sampleRate = 0;
    int numChannels = 0;
    int64 numSamples = 0;
    unsigned int bitsPerSample = 0;
    
    // ===========================================================================
    // listner
    ListenerList<Listener> ImportAudioListeners;
    
    // ===========================================================================
    // thread safe reference pointer

    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioFormatReader *reader = nullptr;
    
    ReferenceCountedArray<ReferenceCountedBuffer> buffers;
    ReferenceCountedBuffer::Ptr buffer; // AudioBuffer<float>
    
    AudioFormatManager formatManager;
    
    //thumbnail
    AudioThumbnail* thumbnail = nullptr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRAudio)
    
};

#endif /* IRAudioReader_hpp */
