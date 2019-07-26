//
//  IRAudio.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 25/03/2019.
//

#include "IRAudio.hpp"

// ------------------------------------------------------------------
IRAudio::IRAudio():
Thread("ImportAudioFile Background thread"),
zoomInfo(1.0, 1.0),
viewPortPos(Point<int>(0,0))
{
    this->formatManager.registerBasicFormats();
    
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
        
        std::cout << "loadFile " << this->path << " : " << p << std::endl;
        
        if(this->path.isNotEmpty())
        {
            this->file = File(this->path);
            this->fileName.swapWith(p);
        
            // start calling update func...
            this->isFileLoadFailed = false;
            this->isFileLoadCompleted = false;
            startAnimation();

            // go thread
            if(threadSafe)
            {
                notify();
            }else{
                //simply load audio file
                checkForPathToOpen();
                checkForBuffersToFree();
            }
            return true;

        }else{
            KLib().showErrorMessage("Error : Could not get file path properly. Please try again...");
            return false;
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
    
    std::cout << "animation " << this << " : " << std::endl;
    if(this->isFileLoadFailed)
    {
        std::cout << "audio file " << this->path << " load failed\n";
        stopAnimation();
        stopThread(4000);

    }else{
        if(this->isFileLoadCompleted)
        {
            stopThread(4000);

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
    if(this->path.isNotEmpty())
    {
        this->isFileOpened = true;
        
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
            
            this->sampleRate = this->reader->sampleRate;
            this->numChannels = this->reader->numChannels;
            this->numSamples = this->reader->lengthInSamples;
            this->bitsPerSample = this->reader->bitsPerSample;
            
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

void IRAudio::callZoomInOutOperatedFromComponent()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    
    // fire call back signal here!
    this->ImportAudioListeners.callChecked(checker, [this] (Listener& l) {l.zoomInOutOperatedFromComponent(this);});
    //check again
    if(checker.shouldBailOut()) return;
}


void IRAudio::callAudioPlayOperatedFromComponent()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    
    // fire call back signal here!
    this->ImportAudioListeners.callChecked(checker, [this] (Listener& l) {l.audioPlayOperatedFromComponent(this);});
    //check again
    if(checker.shouldBailOut()) return;
}

void IRAudio::callViewPortPositionFromComponent()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    
    // fire call back signal here!
    this->ImportAudioListeners.callChecked(checker, [this] (Listener& l) {l.viewPortPositionFromComponent(this);});
    //check again
    if(checker.shouldBailOut()) return;
}
// --------------------------------------------------
void IRAudio::operateBasicDescriptors()
{
    this->analyzer.calcBasicDescriptor();
}
// --------------------------------------------------
void IRAudio::operateFlatness()
{
    
    std::cout <<"calcFlatness\n";
    this->analyzer.calcFlatness();
 
    
}
// --------------------------------------------------
void IRAudio::operateLinearPower(int fftsize, int hopsize)
{
    operateFFTAnalysis(fftsize, hopsize);
}
// --------------------------------------------------
bool IRAudio::operateFFTAnalysis(int fftsize, int hopsize)
{
    this->analyzer.operateAnalysis(getAudioBuffer(), fftsize, hopsize);
}

// --------------------------------------------------

bool IRAudio::operateAnalysis(FFTDescriptor descriptor, int fftsize, int hopsize)
{
   
    this->analyzer.operateAnalysis(getAudioBuffer(), fftsize, hopsize);
    
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
        //case FFTDescriptor::FFT_LinearPower:
         //   operateLinearPower(fft);
          //  break;
        default:
            return "UNKNOWN";
            break;
            
    }
    
    return true;
}
// --------------------------------------------------

bool IRAudio::isCalculated(FFTDescriptor d, int fftsize)
{
    return this->analyzer.isDescriptor(d, this->analyzer.getListFromFFTSize(fftsize));
}

IRDescriptorStr* IRAudio::getDescriptor(FFTDescriptor d, int fftsize)
{
    return this->analyzer.getDescriptor(d, fftsize);
}
// --------------------------------------------------


void IRAudio::linkZoomInOutWithSharedComponents(Component* comp)
{
    this->emittingComponent = comp;
    callZoomInOutOperatedFromComponent();
}
void IRAudio::linkAudioPlaywithSharedComponents(Component* comp)
{
    this->emittingComponent = comp;
    callAudioPlayOperatedFromComponent();
}
void IRAudio::linkViewPortPositionWithSharedComponents(Component* comp)
{
    std::cout << "linkViewPort!\n";
    this->emittingComponent = comp;
    callViewPortPositionFromComponent();
}
// --------------------------------------------------


void IRAudio::setZoomInfo(float w, float h)
{
    this->zoomInfo = Point<float>(w, h);
    setZoomInfo(this->zoomInfo);
}

void IRAudio::setZoomInfo(Point<float> zoom)
{
    this->zoomInfo = zoom;
}
// --------------------------------------------------
