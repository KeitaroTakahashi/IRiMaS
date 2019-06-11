//
//  IRISVPreference.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 01/06/2019.
//

#ifndef IRISVPreference_h
#define IRISVPreference_h

#include "IRPreferenceObject.hpp"
#include "ISVPreferenceUI.hpp"


class IRISVObjectPreference : public IRPreferenceObject
{
public:
    IRISVObjectPreference(String title, Rectangle<int> frameRect) :
    IRPreferenceObject(title, frameRect)
    {

        this->UI = std::make_shared<ISVPreferenceUI>(title);
        addAndMakeVisible(this->UI.get());
        
        this->openControllerButton.setButtonText("Open Controller");
        this->openControllerButton.onClick = [this] { openControllerAction(); };
        this->openControllerButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.fundamental);
        addAndMakeVisible(&this->openControllerButton);
        
        
        std::cout<< "IRISVObjectPreference init done " << this->UI.get() << std::endl;
    }
    
    ~IRISVObjectPreference()
    {

    }
    //===============================================================
    
    void resized() override
    {
        std::cout << "preference width = " << getWidth() << std::endl;
        
        
        int openControlButtonSpace = 40;
        this->UI->setBounds(0,0,getWidth(), getHeight() - openControlButtonSpace);
        
        this->openControllerButton.setBounds(20,
                                             getHeight() - openControlButtonSpace,
                                             getWidth() - 40,
                                             30);
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(SYSTEMCOLOUR.contents.brighter());
    }
    //===============================================================
    // dangerous
    void setControllerUI(ISVParameterControlUI* controlUI)
    {
        this->controlUI = controlUI;
        Rectangle<int> winSize (0, 0, 780, 920);
        this->controllerWindow = std::make_shared<ISVParameterControlWindow>(this->controlUI, "ISV Controller", winSize);
    }
    
    void openControllerAction()
    {
        this->controllerWindow->show();
    }
    
   
    
    void addPresetItem(Array<ISVPresetDataStr> presets)
    {
        this->UI->addPresetItem(presets);
    }
    
    int getSelectedPresetIndex() const { return this->UI->getSelectedPresetIndex(); }
    
    int getSelectedToPresetIndex() const { return this->UI->getSelectedToPresetIndex(); }

    
    //===============================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }
    
    ISVPreferenceUI* getUI() { return this->UI.get(); }
    
private:
    
    std::shared_ptr<ISVPreferenceUI> UI;
    ISVParameterControlUI* controlUI = nullptr;
    
    
    // detail
    TextButton openControllerButton;
    
    
    std::shared_ptr<ISVParameterControlWindow> controllerWindow;
    
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRISVObjectPreference)

    
};
#endif /* IRISVPreference_h */
