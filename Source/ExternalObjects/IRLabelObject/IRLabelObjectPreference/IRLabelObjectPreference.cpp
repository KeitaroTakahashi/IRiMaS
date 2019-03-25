
#include "IRLabelObjectPreference.hpp"




IRLabelObjectPreference::IRLabelObjectPreference(String title, Rectangle<int> frameRect) :
IRPreferenceObject(title,frameRect)
{
    std::cout << "IRLABELOBJECTPREFERENCE CONSTRUCTOR" << std::endl;
    // this->fontGUI = new FontGUI(title);
    this->fontGUI = std::make_unique<FontGUI>(title);
    addAndMakeVisible(this->fontGUI.get());
    this->fontGUI->addChangeListener(this);
}


IRLabelObjectPreference::~IRLabelObjectPreference()
{
    std::cout << "~IRLABELOBJECTPREFERENCE DESTRUCTOR" << std::endl;
}


void IRLabelObjectPreference::resized()
{
    IRPreferenceObject::resized();
    
    this->fontGUI->setBounds(0, 0, getWidth(), getHeight());
}


void IRLabelObjectPreference::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == this->fontGUI.get())
    {
        switch (this->fontGUI->getChangeStatus())
        {
            case FontChanged:
                break;
            case FontStyleChanged:
                break;
            case FontSizeChanged:
                break;
            case FontAlignChanged:
                break;
            case FontColourChanged:
                break;
            default:
                break;
        }
        
    }
}


FontGUI* IRLabelObjectPreference::getFontGUI()
{
    return this->fontGUI.get();
}




