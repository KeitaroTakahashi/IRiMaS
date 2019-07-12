//
//  IRSpectrogramWithPreference.cpp
//  IRiMaS
//
//  Created by Keitaro on 08/07/2019.
//

#include "IRSpectrogramWithPreference.hpp"

IRSpectrogramWithPreference::IRSpectrogramWithPreference(IRNodeObject* nodeObject) : IRSpectrogramUI(nodeObject)
{
    this->preference = std::make_shared<IRSpectrogramObjectPreference>("Spectrogram Preference", Rectangle<int>(400,720));
    
    this->preference->getUI()->addChangeListener(this);

}

IRSpectrogramWithPreference::~IRSpectrogramWithPreference()
{
    
}

// --------------------------------------------------

void IRSpectrogramWithPreference::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == this->preference->getUI())
    {
        std::cout << "magnitudeAmount = " << this->preference->getUI()->getMagnitudeAmount() << std::endl;
        this->setMagnitudeAmount(this->preference->getUI()->getMagnitudeAmount());
    }
}

// --------------------------------------------------
void IRSpectrogramWithPreference::openAudioFileAction()
{
    
}
// --------------------------------------------------
void IRSpectrogramWithPreference::fileImportCompletedAction()
{
    this->isFileImportCompletedFlag = true;
}
// --------------------------------------------------
void IRSpectrogramWithPreference::updateAudioPreferenceUI()
{
}
// --------------------------------------------------
void IRSpectrogramWithPreference::updateAnimationFrame()
{
    if(this->isFileImportCompletedFlag)
    {
        
    }

}
// --------------------------------------------------

// --------------------------------------------------
