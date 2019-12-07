//
//  IRSpectrogramWithPreference.cpp
//  IRiMaS
//
//  Created by Keitaro on 08/07/2019.
//

#include "IRSpectrogramWithPreference.hpp"

IRSpectrogramWithPreference::IRSpectrogramWithPreference(IRNodeObject* nodeObject, IRStr* str) : IRSpectrogramUI(nodeObject, str)
{
    this->preference = std::make_shared<IRSpectrogramObjectPreference>("Spectrogram Preference", Rectangle<int>(400,720), str);
    
    this->preference->getUI()->addChangeListener(this);

}

IRSpectrogramWithPreference::~IRSpectrogramWithPreference()
{
    
}

// --------------------------------------------------

void IRSpectrogramWithPreference::changeListenerCallback (ChangeBroadcaster* source)
{
    
    auto pre = this->preference->getUI();
    if(source == pre)
    {
        
        if(pre->getStatus() == IRSpectrogramPreferenceUI::PreferenceStatus::magnitudeAmountChanged)
        {
            std::cout << "magnitudeAmount = " << this->preference->getUI()->getMagnitudeAmount() << std::endl;
            this->setMagnitudeAmount(this->preference->getUI()->getMagnitudeAmount());
        }else if (pre->getStatus() == IRSpectrogramPreferenceUI::PreferenceStatus::verticalScaleChanged)
        {
            this->setVerticalScale(pre->getVerticalScale());
        }
        else if (pre->getStatus() == IRSpectrogramPreferenceUI::PreferenceStatus::horizontalScaleChanged)
        {
            this->setHorizontalScale(pre->getHorizontalScale());
        }
    }
}

// --------------------------------------------------
void IRSpectrogramWithPreference::updateAudioPreferenceUI()
{
}
// --------------------------------------------------

// --------------------------------------------------

// --------------------------------------------------
