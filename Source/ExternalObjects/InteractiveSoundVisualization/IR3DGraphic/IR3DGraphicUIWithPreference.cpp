//
//  IR3DGraphicUIWithPreference.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 01/06/2019.
//

#include "IR3DGraphicUIWithPreference.hpp"

IR3DGraphicUIWithPreference::IR3DGraphicUIWithPreference(IRNodeObject* nodeObject) : IR3DGraphicUI(nodeObject)
{
    this->preference = std::make_shared<IRISVObjectPreference>("ISV Preference", Rectangle<int>(400,720));
    
    this->preference->getUI()->addChangeListener(this);
    this->preference->getUI()->addListener(this);

}

IR3DGraphicUIWithPreference::~IR3DGraphicUIWithPreference()
{
    
}

// ==================================================
void IR3DGraphicUIWithPreference::resized()
{
    IR3DGraphicUI::resized();
}
// ==================================================

void IR3DGraphicUIWithPreference::intensitySliderChanged(ISVPreferenceUI* obj)
{
    std::cout << "slider changed " << obj->getIntensitySliderValue() << std::endl;
    getOpenGLContents()->setIntensity(obj->getIntensitySliderValue());
}

void IR3DGraphicUIWithPreference::finenessSliderChanged(ISVPreferenceUI* obj)
{
    std::cout << "slider changed " << obj->getFinenessSliderValue() << std::endl;
    getOpenGLContents()->setFineness(obj->getFinenessSliderValue());
}

void IR3DGraphicUIWithPreference::lightingColourChanged(ISVPreferenceUI* obj)
{
    std::cout << "colour changed " << obj->getLightingColour().getFloatRed() << std::endl;
    
    getOpenGLContents()->setLightingColour(obj->getLightingColour());

}

void IR3DGraphicUIWithPreference::materialColourChanged(ISVPreferenceUI* obj)
{
    std::cout << "colour changed " << obj->getMaterialColour().getFloatRed() << std::endl;
    
    getOpenGLContents()->setMaterialColour(obj->getMaterialColour());


}


void IR3DGraphicUIWithPreference::stretchValueChanged(ISVPreferenceUI* obj)
{
    float p_x = obj->getStretch_x_sei();
    float p_y = obj->getStretch_y_sei();
    float p_z = obj->getStretch_z_sei();

    getOpenGLContents()->setPositiveStretch(p_x, p_y, p_z);
    
    float n_x = obj->getStretch_x_fu();
    float n_y = obj->getStretch_y_fu();
    float n_z = obj->getStretch_z_fu();
    
    getOpenGLContents()->setNegativeStretch(n_x, n_y, n_z);
    
    float amount = obj->getStretchAmount();
    getOpenGLContents()->setStretchAmount(amount);
    
    float zIndex = obj->getZIndex();
    getOpenGLContents()->setPositionZ(zIndex);
    
    std::cout << "stretchValueChanged\n";
}

// ==================================================


// ==================================================


void IR3DGraphicUIWithPreference::changeListenerCallback(ChangeBroadcaster* source)
{
    ISVPreferenceUI* ui = this->preference->getUI();
    if( source == ui)
    {
        ISVPreferenceUI::ISVPreferenceStatus status = ui->getStatus();
        switch (status)
        {
            case ISVPreferenceUI::ISVPreferenceStatus::PresetChanged:
                break;
            case ISVPreferenceUI::ISVPreferenceStatus::IntensitySliderChanged:
                break;
            case ISVPreferenceUI::ISVPreferenceStatus::FinenessSliderChanged:
                break;
            case ISVPreferenceUI::ISVPreferenceStatus::LightingColourChanged:
                break;
            case ISVPreferenceUI::ISVPreferenceStatus::MaterialColourChanged:
                break;
            default:
                break;
                
        }
    }
}
