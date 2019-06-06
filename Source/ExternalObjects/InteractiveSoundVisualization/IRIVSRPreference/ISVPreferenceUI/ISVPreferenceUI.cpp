//
//  ISVPreferenceUI.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 01/06/2019.
//

#include "ISVPreferenceUI.hpp"

ISVPreferenceUI::ISVPreferenceUI(String title)
{
    addAndMakeVisible(&this->LabelTitle);
    this->LabelTitle.setText("Interactive Sound Visualizer", dontSendNotification);
    this->LabelTitle.setColour(Label::textColourId, Colours::black);
    this->LabelTitle.setJustificationType(Justification::left);
    
    // --------------------------------------------------
   
    
    this->PresetsLabel.setText("Presets", dontSendNotification);
    this->PresetsLabel.setColour(Label::textColourId, SYSTEMCOLOUR.titleText);
    addAndMakeVisible(&this->PresetsLabel);
    
    this->PresetsBox.setColour(ComboBox::backgroundColourId, SYSTEMCOLOUR.fundamental);
    this->PresetsBox.setColour(ComboBox::outlineColourId, SYSTEMCOLOUR.contents);
    this->PresetsBox.setColour(ComboBox::focusedOutlineColourId, SYSTEMCOLOUR.contents);
    this->PresetsBox.setText("Animation Presets", dontSendNotification);
    this->PresetsBox.addItem("Brightness", this->PresetsBox.getNumItems()+1);
    this->PresetsBox.addItem("Weight", this->PresetsBox.getNumItems()+1);
    this->PresetsBox.addItem("Tension", this->PresetsBox.getNumItems()+1);
    this->PresetsBox.addItem("Dissolution", this->PresetsBox.getNumItems()+1);
    this->PresetsBox.addItem("Thinness", this->PresetsBox.getNumItems()+1);
    this->PresetsBox.addListener(this);
    addAndMakeVisible(&this->PresetsBox);
    // --------------------------------------------------
    this->clearAllPresetsButton.setButtonText("Clear All Presets");
    this->clearAllPresetsButton.onClick = [this] { clearAllPresetsAction(); };
    addAndMakeVisible(&this->clearAllPresetsButton);

    
    // --------------------------------------------------
    
    std::cout << "ISVPreferenceUI init done\n";

}

ISVPreferenceUI::~ISVPreferenceUI()
{
    
}

void ISVPreferenceUI::resized()
{
    
    int marginX = 20;
    
    // margin 10
    this->LabelTitle.setBounds           (20, 10, 300, 30);
    
    this->PresetsLabel.setBounds         (20, 50, 100, 30);
    this->PresetsBox.setBounds           (70 + marginX,
                                          50,
                                          getWidth() - 120 - marginX,
                                          30);
    
    this->clearAllPresetsButton.setBounds (20, 200, getWidth() - 40, 30);


}
// ==================================================

void ISVPreferenceUI::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.contents.brighter());
    g.setColour(Colours::black);
    g.drawLine(0,42.5,getWidth(),42.5);
    //g.drawLine(0,250, getWidth(), 250);
}
// ==================================================

void ISVPreferenceUI::changeListenerCallback(ChangeBroadcaster* source)
{
   
}

// ==================================================

void ISVPreferenceUI::sliderValueChanged (Slider* slider)
{
  
}

void ISVPreferenceUI::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    
    if(comboBoxThatHasChanged == &this->PresetsBox)
    {
        this->status = PresetSelected;
        sendChangeMessage();
    }
}

// ==================================================
void ISVPreferenceUI::addPresetItem(Array<ISVPresetDataStr> presets)
{
    this->PresetsBox.clear();
    for(auto item : presets)
    {
        std::cout << "adding presets " << item.presetTitle << std::endl;
        this->PresetsBox.addItem(item.presetTitle, this->PresetsBox.getNumItems() + 1);
    }
}
// ==================================================

void ISVPreferenceUI::clearAllPresetsAction()
{
    this->status = ClearAllPresets;
    sendChangeMessage();
}
