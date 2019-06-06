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
    
    this->preference->setControllerUI(&this->controlUI);
    this->preference->getUI()->addChangeListener(this);
    
    
    // control UI
    this->controlUI.addChangeListener(this);
    this->controlUI.paramChanged = [this] { uiUpdated(); };
    this->controlUI.colourChanged = [this] { colourUpdated(); };
    this->controlUI.saveAction = [this] { saveAction(); };

    std::cout << "IR3DGraphicUIWithPreference init done " << this->preference.get() << std::endl;
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
// ==================================================


// ==================================================


void IR3DGraphicUIWithPreference::changeListenerCallback(ChangeBroadcaster* source)
{
    
    if(source == this->preference->getUI())
    {
        ISVPreferenceUI::ISVPreferenceStatus status = this->preference->getUI()->getStatus();
        if(status == ISVPreferenceUI::ISVPreferenceStatus::PresetSelected)
        {
            int presetIndex = this->preference->getSelectedPresetIndex();
            loadPreset(presetIndex);
        }else if(status == ISVPreferenceUI::ISVPreferenceStatus::ClearAllPresets)
        {
            clearAllPresets();
        }
    }
}

void IR3DGraphicUIWithPreference::setSliderParams(ISVPresetDataStr preset)
{
    
    this->controlUI.setPreset(preset);
   
    this->vertex.setLightPosition(preset.lightPosition);
    this->vertex.setIntensity(preset.intensity);
    this->vertex.setFineness(preset.fineness);
    
    this->vertex.setQuaterStretchX(preset.quaterStretchX);
    this->vertex.setQuaterStretchY(preset.quaterStretchY);
    
    this->vertex.setHalfStretchX(preset.halfStretchX);
    this->vertex.setHalfStretchY(preset.halfStretchY);
    this->vertex.setHalfStretchZ(preset.halfStretchZ);
    
    this->vertex.setTransform(preset.transform);
    this->vertex.setStretchAmount(preset.amount);
    
}
void IR3DGraphicUIWithPreference::uiUpdated()
{
    ISVPresetDataStr preset = this->controlUI.getPreset1();
    setSliderParams(preset);
}

void IR3DGraphicUIWithPreference::setColourParams(ISVPresetDataStr preset)
{
    this->vertex.setMaterialColour(preset.materialColour);
    this->vertex.setLightingColour(preset.lightingColour);
}

void IR3DGraphicUIWithPreference::colourUpdated()
{
    
    ISVPresetDataStr preset = this->controlUI.getPreset1();
    
    setColourParams(preset);
}

void IR3DGraphicUIWithPreference::saveAction()
{
    
    std::cout << "IR3DGraphicUIWithPreference saving...\n";
    String presetName = this->controlUI.getPresetSaveName();
    
    int index = 0;
    for(auto item : this->presetData)
    {
        if(item.presetTitle == presetName)
        {
            this->presetData.remove(index);
            break;
        }
        index ++;
    }
    ISVPresetDataStr preset = this->controlUI.getPreset1();
    preset.presetTitle = presetName;
    
    this->presetData.add(preset);
    
    this->preference->addPresetItem(this->presetData);
    
    std::cout << "preset name = " << presetName << " saved! " << std::endl;

    AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                      "Saved!",
                                      presetName,
                                      "OK");
    
}

void IR3DGraphicUIWithPreference::loadPreset(int index)
{
    
    std::cout << "loadPreset " << index << std::endl;
    if(index > this->presetData.size())
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                          "ERROR! loadPreset()",
                                          "Wrong index number",
                                          "OK");
        return;
    }
    
    
    if(index > 0)
    {
    
        ISVPresetDataStr preset = this->presetData[index - 1];
        setSliderParams(preset);
        setColourParams(preset);
    }
}

void IR3DGraphicUIWithPreference::clearAllPresets()
{
    this->presetData.clear();
    this->preference->addPresetItem(this->presetData);

}

void IR3DGraphicUIWithPreference::addPreset(ISVPresetDataStr newPreset)
{
    String presetName = newPreset.presetTitle;
    int index = 0;
    
    for(auto item : this->presetData)
    {
        if(item.presetTitle == presetName)
        {
            this->presetData.remove(index);
            break;
        }
        index ++;
    }
    newPreset.presetTitle = presetName;
    
    this->presetData.add(newPreset);
    
    this->preference->addPresetItem(this->presetData);
    
    std::cout << "preset name = " << presetName << " saved! " << std::endl;
    
    AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                      "Saved!",
                                      presetName,
                                      "OK");
    
}
