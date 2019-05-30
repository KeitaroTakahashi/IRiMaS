//
//  IRAudio.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 25/03/2019.
//

#ifndef IRAudio_hpp
#define IRAudio_hpp

#include "JuceHeader.h"
#include "IRAudioReader.hpp"
#include "IRDescriptor.h"
#include "IRAudioDescriptor.hpp"
#include "KLib.h"
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
    
    // get Thumbnail of audio data, it returns nullptr if not exists
    AudioThumbnail* getThumbnail();
    
    // =================================================================
    // audio file reader
    // -----------------------------------------------------------------
    // open dialog to select a file
    bool openFile(bool threadSafe = true);
    // open audio file by path
    bool openFileFromPath(String path, bool threadSafe = true);
    // open audio file by File
    bool loadFile(File file, bool threadSafe = true);
    
    // -----------------------------------------------------------------
    // getter of the audio file info
    // -----------------------------------------------------------------
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
        // give IRAudio* which is THIS object, in order to notify when this object is deallocated by giving nullptr. see deconstructer of this class
        virtual void fileStatusChanged(IRAudio *obj) = 0;
    };
    
    void addListener(Listener* newListener) { this->ImportAudioListeners.add(newListener); }
    void removeListener(Listener* listener) { this->ImportAudioListeners.remove(listener); }
    
    // ---------------------------------------------------------------------------
    
    std::function<void()> onImportCompleted;
    
    // ===========================================================================
    // public members
    bool isFileOpened = false;
    bool isFileLoadCompleted = false;
    
    File getFile() const { return this->file; }
    
    IRDescriptor* getDescriptor() { return this->descriptor.get(); }
    
    bool operateAnalysis(FFTDescriptor descriptor);
    
    void operateBasicDescriptors();
    void operateFlatness();
private:
    // ===========================================================================
    //thread related methods
    // ---------------------------------------------------------------------------
    // check
    void run() override;
    
    void checkForBuffersToFree();
    
    // impoart audio file and read the data
    void checkForPathToOpen();
    void OpenPath(File file);
    
    // ---------------------------------------------------------------------------
    void callFileImportCompleted();
    void callFileStatusChanged(IRAudio* obj);
    // ---------------------------------------------------------------------------

    File file;
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
    
    std::shared_ptr<IRFFTDescriptor> analyzer;
    
    // ===========================================================================
    // descriptor
    std::shared_ptr<IRDescriptor> descriptor;

    // ===========================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRAudio)
    
};
#endif /* IRAudio_hpp */
