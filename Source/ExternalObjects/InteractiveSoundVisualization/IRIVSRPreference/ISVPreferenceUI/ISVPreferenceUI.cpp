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
    this->openControllerButton.setButtonText("Open Controller");
    addAndMakeVisible(&this->openControllerButton);
    this->openControllerButton.onClick = [this]{ openControllerAction(); };
    
    
    
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

    addAndMakeVisible(&this->PresetsBox);
    // --------------------------------------------------

    this->IntensityLabel.setText("Intensity", dontSendNotification);
    this->IntensityLabel.setColour(Label::textColourId, SYSTEMCOLOUR.titleText);
    addAndMakeVisible(&this->IntensityLabel);
    
    this->IntensitySlider.setRange(0.0, 10.0, 0.001);
    this->IntensitySlider.addListener(this);
    
    addAndMakeVisible(&this->IntensitySlider);
    
    // --------------------------------------------------

    this->FinenessLabel.setText("Fineness", dontSendNotification);
    this->FinenessLabel.setColour(Label::textColourId, SYSTEMCOLOUR.titleText);
    addAndMakeVisible(&this->FinenessLabel);
    
    this->FinenessSlider.setRange(0.0, 1.0, 0.001);
    this->FinenessSlider.addListener(this);
    
    addAndMakeVisible(&this->FinenessSlider);
    
    // --------------------------------------------------
    
    this->LightingColourLabel.setText("Lighting Colour", dontSendNotification);
    this->LightingColourLabel.setColour(Label::textColourId, SYSTEMCOLOUR.titleText);
    addAndMakeVisible(&this->LightingColourLabel);
    
    
    addAndMakeVisible(&this->lightingColour);
    this->lightingColour.setCurrentColour(Colours::black);
    this->lightingColour.addChangeListener(this);
    
    
    this->MaterialColourLabel.setText("Material Colour", dontSendNotification);
    this->MaterialColourLabel.setColour(Label::textColourId, SYSTEMCOLOUR.titleText);
    addAndMakeVisible(&this->MaterialColourLabel);
    
    addAndMakeVisible(&this->materialColour);
    this->materialColour.setCurrentColour(Colours::black);
    this->materialColour.addChangeListener(this);

    
    // --------------------------------------------------


}

ISVPreferenceUI::~ISVPreferenceUI()
{
    if(this->controllerWindow != nullptr)
        delete this->controllerWindow;
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
    
    this->IntensityLabel.setBounds       (20, 80, 100, 30);
    this->IntensitySlider.setBounds      (20, 110, getWidth() - 40, 30);
    
    this->FinenessLabel.setBounds        (20, 140, 100, 30);
    this->FinenessSlider.setBounds       (20, 170, getWidth() - 40, 30);
    
    this->MaterialColourLabel.setBounds  (20, 200, 100, 30);
    this->materialColour.setBounds       (20, 230, getWidth() - 40, 200);
    
    this->LightingColourLabel.setBounds  (20, 430, 100, 30);
    this->lightingColour.setBounds       (20, 460, getWidth() - 40, 200);
    
    this->openControllerButton.setBounds(20, 670, getWidth() - 40, 30);


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
    if( source == &this->lightingColour)
    {
        callLightingColourChanged(this);
    }else if( source == &this->materialColour)
    {
        callMaterialColourChanged(this);
    }else if( source == this->controllerWindow->getControlUI())
    {
        ISVParameterControlUI* u = this->controllerWindow->getControlUI();
        if(u->getStatus() == ISVParameterControlUI::ISVParameterControlUIStatus::SliderValueChanged)
        {
            callStretchValueChanged(this);
        }
    }
}

// ==================================================

void ISVPreferenceUI::sliderValueChanged (Slider* slider)
{
   if(slider == &this->IntensitySlider)
   {
       callIntentisySliderChanged(this);
   }else if(slider == &this->FinenessSlider)
   {
       callFinenessSliderChanged(this);
   }
}

void ISVPreferenceUI::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    
}
// ==================================================

float ISVPreferenceUI::getIntensitySliderValue() const
{
    return this->IntensitySlider.getValue();
}
float ISVPreferenceUI::getFinenessSliderValue() const
{
    return this->FinenessSlider.getValue();
}
Colour ISVPreferenceUI::getLightingColour() const
{
    return this->lightingColour.getCurrentColour();
}
Colour ISVPreferenceUI::getMaterialColour() const
{
    return this->materialColour.getCurrentColour();
}

// ==================================================

void ISVPreferenceUI::callIntentisySliderChanged(ISVPreferenceUI* obj)
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if (checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.intensitySliderChanged(this); });
    //check again
    if (checker.shouldBailOut()) return;
}

void ISVPreferenceUI::callFinenessSliderChanged(ISVPreferenceUI* obj)
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if (checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.finenessSliderChanged(this); });
    //check again
    if (checker.shouldBailOut()) return;
}

void ISVPreferenceUI::callLightingColourChanged(ISVPreferenceUI* obj)
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if (checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.lightingColourChanged(this); });
    //check again
    if (checker.shouldBailOut()) return;
}

void ISVPreferenceUI::callMaterialColourChanged(ISVPreferenceUI* obj)
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if (checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.materialColourChanged(this); });
    //check again
    if (checker.shouldBailOut()) return;
}

void ISVPreferenceUI::callStretchValueChanged(ISVPreferenceUI* obj)
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if (checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.stretchValueChanged(this); });
    //check again
    if (checker.shouldBailOut()) return;
}

// ==================================================

void ISVPreferenceUI::openControllerAction()
{
    std::cout << "openControllerAction\n";
    if(this->controllerWindow == nullptr)
    {
        Rectangle<int> winSize (0, 0, 1280, 800);
        this->controllerWindow = new ISVParameterControlWindow("ISV controller", winSize);
        this->controllerWindow->getControlUI()->addChangeListener(this);

    }else
    {
        
    }
        
    this->controllerWindow->show();
}

// ==================================================

float ISVPreferenceUI::getStretch_x_sei() const
{
    ISVParameterControlUI* u = this->controllerWindow->getControlUI();
    return u->getStretch_x_sei();
}
float ISVPreferenceUI::getStretch_x_fu() const
{
    ISVParameterControlUI* u = this->controllerWindow->getControlUI();
    return u->getStretch_x_fu();
}

float ISVPreferenceUI::getStretch_y_sei() const
{
    ISVParameterControlUI* u = this->controllerWindow->getControlUI();
    return u->getStretch_y_sei();
}
float ISVPreferenceUI::getStretch_y_fu() const
{
    ISVParameterControlUI* u = this->controllerWindow->getControlUI();
    return u->getStretch_y_fu();
}

float ISVPreferenceUI::getStretch_z_sei() const
{
    ISVParameterControlUI* u = this->controllerWindow->getControlUI();
    return u->getStretch_z_sei();
}
float ISVPreferenceUI::getStretch_z_fu() const
{
    ISVParameterControlUI* u = this->controllerWindow->getControlUI();
    return u->getStretch_z_fu();
}

float ISVPreferenceUI::getStretchAmount() const
{
    ISVParameterControlUI* u = this->controllerWindow->getControlUI();
    return u->getStretchAmount();
}

float ISVPreferenceUI::getZIndex() const
{
    ISVParameterControlUI* u = this->controllerWindow->getControlUI();
    return u->getZIndex();
}
