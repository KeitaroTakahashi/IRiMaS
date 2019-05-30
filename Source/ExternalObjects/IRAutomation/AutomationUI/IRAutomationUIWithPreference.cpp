//
//  IRAutomationUIWithPreference.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/05/2019.
//

#include "IRAutomationUIWithPreference.hpp"

IRAutomationUIWithPreference::IRAutomationUIWithPreference(IRNodeObject* nodeObject) :
IRAutomationUI(nodeObject)
{
    this->preference = std::make_shared<IRAutomationObjectPreference>("Automation Preference", Rectangle<int>(400,720));
    this->preference->getUI()->getOpenAudioUI()->setOpenAudioButtonCallback([this]{openAudioFileAction();});
    this->preference->getUI()->getOpenAudioUI()->addChangeListener(this);

    
    setFps(33);
    std::cout << "IRAutomationUIWithPreference init\n";
}

IRAutomationUIWithPreference::~IRAutomationUIWithPreference()
{
    
}

void IRAutomationUIWithPreference::openAudioFileAction()
{
    startAnimation();
    openFile();
    
   
}

void IRAutomationUIWithPreference::updateAudioPreferenceUI()
{
    this->preference->getUI()->getOpenAudioUI()->setAudioFileURL(getFilePath());
    
    //get audio data
    auto data = getAudioData()->getData();
    // get descriptor
    auto de = data->getDescriptor();
    // get a list of descriptor name
    auto list = de->getDescriptorNameList();
    // add each descriptor name to the menu
    this->preference->getUI()->getOpenAudioUI()->clearDescriptorMenu();

    for(auto l : list)
    {
        this->preference->getUI()->getOpenAudioUI()->addDescriptorMenu(l);
        
        std::cout << " desctiptor = " << l << std::endl;
    }
    
    this->isFileImportCompletedFlag = false;
    stopAnimation();
    
    // initially set Magnitude
    // check if magnitude is already exctracted from the audio data
    if(de->isMagnitude()) setDescriptor(de->getMagnitude());
    else {
        std::cout << "not yet analyzed\n";
        data->operateAnalysis(FFTDescriptor::FFT_MAGNITUDE);
        if(de->isMagnitude()) setDescriptor(de->getMagnitude());
    }
    this->preference->getUI()->getOpenAudioUI()->setSelectedDescriptorItem(0);

}

void IRAutomationUIWithPreference::fileImportCompletedAction()
{
    this->isFileImportCompletedFlag = true;
}


void IRAutomationUIWithPreference::updateAnimationFrame()
{
    if(this->isFileImportCompletedFlag)
    {
        updateAudioPreferenceUI();
    }
    
    std::cout << "isFileImportCompletedFlag\n";
}

void IRAutomationUIWithPreference::hasDescriptorSelectedAction(FFTDescriptor descriptor)
{
    auto data = getAudioData()->getData();
    auto de = data->getDescriptor();
    
    std::cout << "(FFTDescriptor descriptor) = " << descriptor << std::endl;
    switch ( descriptor )
    {
        case FFTDescriptor::FFT_MAGNITUDE:
            // check if magnitude is already exctracted from the audio data
            if(de->isMagnitude()) setDescriptor(de->getMagnitude());
            else {
                std::cout << "not yet analyzed\n";
                data->operateAnalysis(descriptor);
                if(de->isMagnitude()) setDescriptor(de->getMagnitude());
            }
            break;
        case FFTDescriptor::FFT_CENTROID:
            // check if magnitude is already exctracted from the audio data
            if(de->isCentroid()) setDescriptor(de->getCentroid());
            else {
                std::cout << "not yet analyzed\n";
                data->operateAnalysis(descriptor);
                if(de->isCentroid()) setDescriptor(de->getCentroid());

            }
            break;
        case FFTDescriptor::FFT_SPREAD:
            // check if magnitude is already exctracted from the audio data
            if(de->isSpread()) setDescriptor(de->getSpread());
            else {
                std::cout << "not yet analyzed\n";
                data->operateAnalysis(descriptor);
                if(de->isSpread()) setDescriptor(de->getSpread());

            }
            break;
        case FFTDescriptor::FFT_FLATNESS:
            // check if magnitude is already exctracted from the audio data
            if(de->isFlatness()) setDescriptor(de->getFlatness());
            else {
                std::cout << "not yet analyzed\n";
                data->operateAnalysis(descriptor);
                if(de->isFlatness()) setDescriptor(de->getFlatness());
            }
            break;
        case FFTDescriptor::FFT_NOISINESS:
            break;
        case FFTDescriptor::FFT_PITCH:
            break;
        default:
            break;
    }
}

void IRAutomationUIWithPreference::changeListenerCallback(ChangeBroadcaster* source)
{
    
    // OpenAudioUI
    if(source == this->preference->getUI()->getOpenAudioUI())
    {
        auto status = this->preference->getUI()->getOpenAudioUI()->getStatus();
        
        if(status == AutomationPreferenceOpenAudioUI::AutomationpreferenceOpenAudioUIStatus::DescriptorMenuChanged)
        {
            hasDescriptorSelectedAction(this->preference->getUI()->getOpenAudioUI()->getSelectedDescriptor());

        }
    }
}


