//
//  IRAudio.cpp
//  Thread safe audio file reader
//
//  Created by Keitaro on 17/08/2018.
//

#include "IRAudioReader.hpp"
// ------------------------------------------------------------------
IRAudio::IRAudio():
Thread("ImportAudioFile Background thread")
{
    this->formatManager.registerBasicFormats();
    startThread();
}
// ------------------------------------------------------------------
IRAudio::~IRAudio()
{
    if (this->reader)
        delete this->reader;
    stopThread(4000);
}
// ------------------------------------------------------------------
void IRAudio::clear(ReferenceCountedBuffer::Ptr currentBuffer)
{
    currentBuffer = nullptr;
}
// ------------------------------------------------------------------
AudioThumbnail* IRAudio::getThumbnail()
{
    return this->thumbnail;
}
// ------------------------------------------------------------------
bool IRAudio::openFile()
{
    FileChooser chooser("Select an audio file to play...",
                        {},
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
// ------------------------------------------------------------------
bool IRAudio::openFileFromPath(String path)
{
    this->path.swapWith(path);
    notify();
    return true;
}
// ------------------------------------------------------------------

std::vector<float> IRAudio::getAudioBufferInVector()
{
    AudioSampleBuffer* sampleBuffer = this->buffer->getAudioSampleBuffer();
    float* startPtr = sampleBuffer->getWritePointer(0);
    int size = sampleBuffer->getNumSamples();
    std::vector<float> vec (startPtr, startPtr + size);
    return vec;
}

// ------------------------------------------------------------------
void IRAudio::run()
{
    while(! threadShouldExit())
    {
        checkForPathToOpen();
        checkForBuffersToFree();
        wait(500);
    }
}
// ------------------------------------------------------------------

void IRAudio::checkForBuffersToFree()
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
// ------------------------------------------------------------------

void IRAudio::checkForPathToOpen()
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
