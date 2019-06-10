//
//  IRAudio.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 25/03/2019.
//

#include "IRAudio.hpp"

// ------------------------------------------------------------------
IRAudio::IRAudio():
Thread("ImportAudioFile Background thread")
{
    this->formatManager.registerBasicFormats();
    this->descriptor = std::make_shared<IRDescriptor>();
    this->analyzer = std::make_shared<IRFFTDescriptor>(2048, 1024);
    
    setFps(33);
    startThread();
}
// ------------------------------------------------------------------
IRAudio::~IRAudio()
{
    if (this->reader){
        delete this->reader;
        
        // because this class is about to deleted, notify all objects using this object that this object is null.
        callFileStatusChanged(nullptr);
    }
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
bool IRAudio::openFile(bool threadSafe)
{
    FileChooser chooser("Select an audio file to play...",
                        {},
                        "*.wav, *.aif, *.aiff, *.mp3");
    if(chooser.browseForFileToOpen())
    {
        this->file = chooser.getResult();
        return loadFile(this->file, threadSafe);
    }
    return false;
}
// ------------------------------------------------------------------
bool IRAudio::openFileFromPath(String path, bool threadSafe)
{
    this->file = File(path);
    return loadFile(this->file, threadSafe);
}

bool IRAudio::loadFile(File file, bool threadSafe)
{
    if(file.exists())
    {
        String p = file.getFullPathName();
        this->path.swapWith(p);
        this->filePath.swapWith(p);
        
        this->fileName.swapWith(p);
        
        // start calling update func...
        this->isFileLoadFailed = false;
        this->isFileLoadCompleted = false;
        startAnimation();

        // go thread
        if(threadSafe)
        {
            notify();
            return true;
        }else{
            //simply load audio file
            checkForPathToOpen();
            checkForBuffersToFree();
            return true;
        }
    }else return false;
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

void IRAudio::updateAnimationFrame()
{
    if(this->isFileLoadFailed)
    {
        
    }else{
        if(this->isFileLoadCompleted)
        {
            
            std::cout<< "file Import Completed\n";
            callFileImportCompleted();
            callFileStatusChanged(this);
            
            //this->isFileLoadCompleted = true;
            //sendChangeMessage();
            
            stopAnimation();
        }
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
        
        this->file = File(pathToOpen);
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
            
            //std::cout << "newBuffer " << newBuffer->getAudioSampleBuffer()->getNumSamples() << std::endl;
            this->buffer = newBuffer;
            this->buffers.add (newBuffer);
            
            this->sampleRate = this->reader->sampleRate;
            this->numChannels = this->reader->numChannels;
            this->numSamples = this->reader->lengthInSamples;
            this->bitsPerSample = this->reader->bitsPerSample;
            
            //std::cout << "reading samples of "<<(int) reader->lengthInSamples<<std::endl;
          
            this->isFileLoadCompleted = true;
            
        }else{
            //std::cout << "read failed\n";
            
        }
    }else{
       // could not load audio file
        this->isFileLoadCompleted = false;
        this->isFileLoadFailed = true;
    }
}
// --------------------------------------------------

void IRAudio::callFileImportCompleted()
{
    std::cout << "callFileImportCompleted()\n";
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    
    // fire call back signal here!
    this->ImportAudioListeners.callChecked(checker, [this] (Listener& l) {l.fileImportCompleted(this);});
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->onImportCompleted != nullptr) this->onImportCompleted();
}
// --------------------------------------------------

void IRAudio::callFileStatusChanged(IRAudio* obj)
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    
    // fire call back signal here!
    this->ImportAudioListeners.callChecked(checker, [this] (Listener& l) {l.fileStatusChanged(this);});
    //check again
    if(checker.shouldBailOut()) return;
}

// --------------------------------------------------
void IRAudio::operateBasicDescriptors()
{
    this->analyzer->calcCentroid();

    IRAnalysisDataStr newMag (this->analyzer->getFFTSize(), this->analyzer->getHopSize());
    newMag.setRowDataAndNoamalize(this->analyzer->getMagnitude());
    getDescriptor()->setMagnitude(newMag);

    IRAnalysisDataStr newCent (this->analyzer->getFFTSize(), this->analyzer->getHopSize());
    newCent.setRowDataAndNoamalize(this->analyzer->getCentroid());
    getDescriptor()->setCentroid(newCent);

    IRAnalysisDataStr newSp (this->analyzer->getFFTSize(), this->analyzer->getHopSize());
    newSp.setRowDataAndNoamalize(this->analyzer->getSpread());
    getDescriptor()->setSpread(newSp);
}
// --------------------------------------------------
void IRAudio::operateFlatness()
{
    this->analyzer->calcFlatness();
 
    IRAnalysisDataStr newFlat (this->analyzer->getFFTSize(), this->analyzer->getHopSize());
    newFlat.setRowDataAndNoamalize(this->analyzer->getFlatness());
    getDescriptor()->setFlatness(newFlat);
}
// --------------------------------------------------

bool IRAudio::operateAnalysis(FFTDescriptor descriptor)
{
    
    // if FFT is not yet operated, then do it.
    if(!this->analyzer->hasFFTOperated()){
        
        if(this->analyzer->getNumFrame() == 0)
            this->analyzer->setAudioData(this->getAudioBufferInVector());
        this->analyzer->operateAnalysis();
        
    }
    
    // calcCentroid calculates Magnitude, Centroid, and Spread
    switch (descriptor)
    {
        case FFTDescriptor::FFT_MAGNITUDE:
            operateBasicDescriptors();
            break;
        case FFTDescriptor::FFT_CENTROID:
            operateBasicDescriptors();
            break;
        case FFTDescriptor::FFT_SPREAD:
            operateBasicDescriptors();
            break;
        case FFTDescriptor::FFT_FLATNESS:
            operateFlatness();
            break;
        case FFTDescriptor::FFT_NOISINESS:
            return "Noisiness";
            break;
            
        case FFTDescriptor::FFT_PITCH:
            return "Pitch";
            break;
            
        case FFTDescriptor::FFT_MFCS:
            return "MFCs";
            break;
            
        case FFTDescriptor::FFT_MFCCS:
            return "MFCCs";
            break;
            
        case FFTDescriptor::FFT_BFCS:
            return "BFCs";
            break;
            
        case FFTDescriptor::FFT_BFCCS:
            return "BFCCs";
            break;
        default:
            return "UNKNOWN";
            break;
            
    }
    
    return true;
}
// --------------------------------------------------
