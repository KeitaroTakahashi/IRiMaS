//
//  IRAudioReader.hpp
//  Thread safe audio file reader
//
//  Created by Keitaro on 17/08/2018.
//

#ifndef IRAudioReader_hpp
#define IRAudioReader_hpp


#include "JuceHeader.h"
// #include "juce_audio_utils/juce_audio_utils.h"

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


class IRAudioReader : public Component,
                      public ChangeBroadcaster,
                      private Thread
{
public:
    IRAudioReader()
    :   Thread("ImportAudioFile Background thread")
    {
        this->formatManager.registerBasicFormats();
        startThread();
    }
    ~IRAudioReader()
    {
        stopThread(4000);
    }
    
    void clear(ReferenceCountedBuffer::Ptr currentBuffer)
    {
        currentBuffer = nullptr;
    }
    
    AudioThumbnail* getThumbnail()
    {
        return this->thumbnail;
    }
    
    bool openFile()
    {
        FileChooser chooser("Select an audio file to play...",
                            File::nonexistent,
                            "*.wav, *.aif, *.aiff");
        if(chooser.browseForFileToOpen())
        {
            auto file = chooser.getResult();
            auto path = file.getFullPathName();
            this->path.swapWith(path);
            notify();
            return true;
        }
        return false;
    }
    
    bool openFileFromPath(String path)
    {
        this->path.swapWith(path);
        notify();
        return true;
    }
    
    // ------------------------------------------------------------------
    // audio file info
    // ------------------------------------------------------------------
    double getSampleRate() const { return this->sampleRate; }
    int getNumChannels() const { return this->numChannels; }
    int64 getNumSamples() const { return this->numSamples; }
    unsigned int getBitsPerSample() const { return this->bitsPerSample; }
    String getPath() const { return this->filePath; }
    String getFileName() const { return this->fileName; }

    void showAudioFileInfo();
    // ------------------------------------------------------------------
    AudioSampleBuffer* getAudioBuffer(){ return this->buffer->getAudioSampleBuffer(); }
    
    std::vector<float> getAudioBufferInVector()
    {
        AudioSampleBuffer* sampleBuffer = this->buffer->getAudioSampleBuffer();
        float* startPtr = sampleBuffer->getWritePointer(0);
        int size = sampleBuffer->getNumSamples();
        std::vector<float> vec (startPtr, startPtr + size);
        return vec;
    }

    // ===========================================================================
    // Listener
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        // called when audio file has been loaded
        virtual void fileImportCompleted(IRAudioReader *obj) = 0;
        // called when file status changed (will be loaded, completed, deleted etc.)
        virtual void fileStatusChanged(IRAudioReader *obj) = 0;
    };
    
    void addListener(Listener* newListener) { this->ImportAudioListeners.add(newListener); }
    void removeListener(Listener* listener) { this->ImportAudioListeners.remove(listener); }
    
    // ---------------------------------------------------------------------------
    
    std::function<void()> onImportCompleted;
    std::function<void()> onFileStatusChanged;
    
    // ===========================================================================
    
    
private:
    //thread
        void run() override
        {
            while(! threadShouldExit())
            {
                checkForPathToOpen();
                checkForBuffersToFree();
                wait(500);
            }
        }
    
    void checkForBuffersToFree()
    {
        for(auto i = this->buffers.size(); --i >= 0;)
        {
            ReferenceCountedBuffer::Ptr buffer (this->buffers.getUnchecked(i));
            if(this->buffer->getReferenceCount() == 2)
            {
                this->buffers.remove(i);
            }
        }
    }
    
    void checkForPathToOpen()
    {
        String pathToOpen;
        pathToOpen.swapWith(this->path);

        if(pathToOpen.isNotEmpty())
        {
            this->isFileOpened = true;
            
            Component::BailOutChecker checker(this);
            
            File file (pathToOpen);
            if((this->reader = this->formatManager.createReaderFor(file)))
            {
                
                ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(file.getFileName(),
                                                                                   this->reader->numChannels,
                                                                                   (int) reader->lengthInSamples);
                
                this->reader->read(newBuffer->getAudioSampleBuffer(),
                                   0,
                                   (int) reader->lengthInSamples,
                                   0,
                                   true,
                                   true);
                this->buffer = newBuffer;
                this->buffers.add (newBuffer);
                
                // thumbail thread safe
                //ReferenceCountedThumbnail::Ptr newThumbnail = new ReferenceCountedThumbnail(file.getFileName());
                
                //newThumbnail->getThumbnail()->setSource(new FileInputSource(file));
                
                this->sampleRate = this->reader->sampleRate;
                this->numChannels = this->reader->numChannels;
                this->numSamples = this->reader->lengthInSamples;
                this->bitsPerSample = this->reader->bitsPerSample;
                
                std::cout << "reading samples of "<<(int) reader->lengthInSamples<<std::endl;
                //==========
                // check if the objects are not deleted, if deleted, return
                if(checker.shouldBailOut()) return;
                
                // fire call back signal here!
                this->ImportAudioListeners.callChecked(checker, [this] (Listener& l) {l.fileImportCompleted(this);});
                //check again
                if(checker.shouldBailOut()) return;
                //std::function
                if(this->onImportCompleted != nullptr) this->onImportCompleted();
                
                this->isFileLoadCompleted = true;
                this->fileName = file.getFileName();
                this->filePath = file.getFullPathName();
                sendChangeMessage();
                
            }else{
                std::cout << "read failed\n";
                
            }
        }else{
            this->isFileOpened = false;
            this->isFileLoadCompleted = false;
            sendChangeMessage();
        }
    }
    
public:
    bool isFileOpened = false;
    bool isFileLoadCompleted = false;

private:
    
    String path;
    String filePath;
    String fileName;
    double sampleRate = 0;
    int numChannels = 0;
    int64 numSamples = 0;
    unsigned int bitsPerSample = 0;
    
    
    // --------------------------------------------------------
    
    // listner
    ListenerList<Listener> ImportAudioListeners;
    
    // --------------------------------------------------------
    
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioFormatReader *reader;
    
    ReferenceCountedArray<ReferenceCountedBuffer> buffers;
    ReferenceCountedBuffer::Ptr buffer; // AudioBuffer<float>
    
    AudioFormatManager formatManager;
    
    //thumbnail
    AudioThumbnail* thumbnail;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRAudioReader)
    
};

#endif /* IRAudioReader_hpp */
