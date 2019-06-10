//
//  ISVPreferenceUI.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 01/06/2019.
//

#include "ISVPreferenceUI.hpp"

ISVPreferenceUI::ISVPreferenceUI(String title) :
transitionToInitialSphere       (this, "Transition to initial sphere", 0.0, 1.0, 1.0),
transitionBetweenPresets        (this, "Transition between presets", 0.0, 1.0, 0.0)

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
    this->PresetsBox.setText("Shape Presets", dontSendNotification);
    this->PresetsBox.addItem("Brightness", this->PresetsBox.getNumItems()+1);
    this->PresetsBox.addItem("Weight", this->PresetsBox.getNumItems()+1);
    this->PresetsBox.addItem("Tension", this->PresetsBox.getNumItems()+1);
    this->PresetsBox.addItem("Dissolution", this->PresetsBox.getNumItems()+1);
    this->PresetsBox.addItem("Thinness", this->PresetsBox.getNumItems()+1);
    this->PresetsBox.addListener(this);
    addAndMakeVisible(&this->PresetsBox);
    // --------------------------------------------------

    addAndMakeVisible(&this->transitionToInitialSphere);
    addAndMakeVisible(&this->transitionBetweenPresets);
    
    
    addAndMakeVisible(&this->toLabel);
    this->toLabel.setText("to Preset ", dontSendNotification);
    this->toLabel.setColour(Label::textColourId, SYSTEMCOLOUR.titleText);

    addAndMakeVisible(&this->toPreset);
    this->toPreset.setColour(ComboBox::backgroundColourId, SYSTEMCOLOUR.fundamental);
    this->toPreset.setColour(ComboBox::outlineColourId, SYSTEMCOLOUR.contents);
    this->toPreset.setColour(ComboBox::focusedOutlineColourId, SYSTEMCOLOUR.contents);
    this->toPreset.setText("Destination Presets", dontSendNotification);
    this->toPreset.addListener(this);


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
    int y = 10;
    // margin 10
    this->LabelTitle.setBounds           (20, y, 300, 30);
    
    y += 40;
    this->PresetsLabel.setBounds         (20, y, 100, 30);
    this->PresetsBox.setBounds           (90,
                                          y,
                                          getWidth() - 100,
                                          30);
    y += 40;
    this->transitionToInitialSphere.setBounds  (20, y, getWidth() - 40, 70);

    y += 70;
    this->toLabel.setBounds         (20, y, 100, 30);
    this->toPreset.setBounds        (90, y, getWidth() - 100, 30);
    
    y+= 30;
    this->transitionBetweenPresets.setBounds   (20, y, getWidth() - 40, 70);

    this->clearAllPresetsButton.setBounds      (20, 300, getWidth() - 40, 30);

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
    }else if(comboBoxThatHasChanged == &this->toPreset)
    {
        this->status = toPresetSelected;
        sendChangeMessage();
    }
}

// ==================================================
void ISVPreferenceUI::addPresetItem(Array<ISVPresetDataStr> presets)
{
    this->PresetsBox.clear();
    this->toPreset.clear();
    for(auto item : presets)
    {
        std::cout << "adding presets " << item.presetTitle << std::endl;
        this->PresetsBox.addItem(item.presetTitle, this->PresetsBox.getNumItems() + 1);
        this->toPreset.addItem(item.presetTitle, this->toPreset.getNumItems() + 1);
    }
}
// ==================================================

void ISVPreferenceUI::clearAllPresetsAction()
{
    this->status = ClearAllPresets;
    sendChangeMessage();
}

void ISVPreferenceUI::sliderUIValueChanged(sliderUI1 *obj)
{
    if(obj == &this->transitionToInitialSphere)
    {
        
        
        this->status = TransitionToInitialSphereChanged;
        sendChangeMessage();
    }else if(obj == &this->transitionBetweenPresets)
    {
        this->status = TransitionBetweenPresetsChanged;
        sendChangeMessage();
    }
}

