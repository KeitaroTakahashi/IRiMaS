//
//  IRAnalysisUI.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#include "IRAnalysisUI.hpp"

IRAnalysisUI::IRAnalysisUI(IRNodeObject* parent) :
IRUIAudioFoundation(parent)
{
    this->parent = parent;
    
    this->FFTSequence = std::make_shared<IRFFTDescriptor>(this->fftsize,
                                                        this->hopsize,
                                                        IRWindow::TYPE::HAMMING);
    
    this->FFTSequence->setDescriptor(2,
                                     FFTDescriptor::FFT_MAGNITUDE,
                                     FFTDescriptor::FFT_CENTROID);
    
    addAndMakeVisible(&this->openButton);
    this->openButton.setButtonText("open audio");
    
    this->openButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.fundamental);
    this->setEnabled(true);
    this->openButton.onClick = [this]{ openFile(); };
}

IRAnalysisUI::~IRAnalysisUI()
{
    if(this->audioData != nullptr)
        getFileManager()->discardFilePtr(IRFileType::IRAUDIO, this->audioData, this->parent, this->file);
}
// --------------------------------------------------

void IRAnalysisUI::resized()
{
    this->openButton.setBounds(0,0,getWidth(),getHeight());

}

void IRAnalysisUI::paint(Graphics& g)
{
    
}
// --------------------------------------------------
void IRAnalysisUI::deinitializeAudioData()
{
    if(this->audioData != nullptr)
        getFileManager()->discardFilePtr(IRFileType::IRAUDIO, this->audioData, this->parent, this->file);
}
// --------------------------------------------------

void IRAnalysisUI::openFile()
{
    
    FileChooser chooser("Select an image file...",
                        {},
                        "*.wav, *.aif, *.aiff, *.mp3");
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        this->file = file;
        auto p = file.getFullPathName();
        this->path = p;
        
        String pathToOpen;
        pathToOpen.swapWith(p);
        
        if(pathToOpen.isNotEmpty())
        {
            getFilePtr(this->file);
        }
    }
}
// --------------------------------------------------

void IRAnalysisUI::openFile(String path)
{
    if(path.isNotEmpty())
    {
        File f(path);
        this->file = f;
        this->path = path;
        
        if(f.exists())
        {
            getFilePtr(this->file);
        }
    }
}
// --------------------------------------------------

void IRAnalysisUI::getFilePtr(File file)
{
    
    std::cout << "getFilePtr\n";
    // set a callback function which is called when file load is completed.
    // get a pointer of the audio file
    std::function<void()> callback = [this]{fileImportCompleted();};
    getFileManager()->getFilePtrWithCallBack(IRFileType::IRAUDIO,
                                             file,
                                             this->parent,
                                             callback);
    
    
    // notify changes to IRProject to update IRFileInspecter
    this->parent->notifyNodeObjectModification();
    
}
// --------------------------------------------------

//==========================================================================
// Callback functions
//==========================================================================

void IRAnalysisUI::fileImportCompleted()
{
    
    std::cout << "fileImportCompleted \n";
    this->audioData = static_cast<DataAllocationManager<IRAudio>*>(getFileManager()->getFileObject());
    
    // copy audio data into vector format.
    //this->rawData.clear();
    //this->rawData = this->audioData->getData()->getAudioBuffer();
    
    auto sampleBuffer = this->audioData->getData()->getAudioBuffer();
    float* startPtr = sampleBuffer->getWritePointer(0);

    /*
    for(auto val : this->rawData)
    {
        std::cout << val << std::endl;
    }
     */
    this->FFTSequence->setAudioData(startPtr, this->audioData->getData()->getNumSamples());
    
    executeAnalysis();
    
    // set received Ptr to the Link System
    this->parent->setAudioLink(this->audioData->getData());
    
   
    
}


double IRAnalysisUI::getSampleRate() const
{
    return this->audioData->getData()->getSampleRate();
}

// ==================================================
void IRAnalysisUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
}

void IRAnalysisUI::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
}

void IRAnalysisUI::releaseResources()
{
}
// ==================================================

void IRAnalysisUI::audioPtrDelivery(IRAudio *obj)
{
    std::cout << "audioPtrDelivery filename = " << obj->getFile().getFullPathName() << std::endl;
    //makeThumbnail(obj->getFile().getFullPathName());
    
    this->file = obj->getFile();
    
    getFilePtr(this->file);
}

void IRAnalysisUI::executeAnalysis()
{
    
    if(this->FFTSequence.get() != nullptr)
    {
        
        std::cout << "opearate analysis for " << this->FFTSequence->getNumFrame() << " frames\n";

        if(this->FFTSequence->getNumFrame() > 0)
        {
            this->nframe = this->FFTSequence->getNumFrame();
            this->FFTSequence->FFT();
        }
    }
    
    /*
    //this->FFTSequence->cartopol();
    
    //std::cout << "power size = " << this->FFTSequence->getPower().size() << " : frames = " << this->FFTSequence->getPower()[0].size() << std::endl;
    
    //this->FFTSequence->calcMagnitude();
    
    this->magData = std::make_shared<IRAnalysisDataStr>();
    this->magData->setFFTSize(this->fftsize);
    this->magData->setRowDataAndNormalize(this->FFTSequence->getMagnitude());
    
    // register extracted descriptors to IRDescriptor through IRAudio
    // descriptors are stored in IRAudio class in the file manager
    IRDescriptor* descriptor = this->audioData->getData()->getDescriptor();
    if(!descriptor->isMagnitude())
    {
        descriptor->setMagnitude(*this->magData.get());
        descriptor->addDescriptorList(FFTDescriptor::FFT_MAGNITUDE);
    }
    
    if(this->completeAnalysis != nullptr) this->completeAnalysis();
    */
    std::cout << "Analysis Completed\n";

}
