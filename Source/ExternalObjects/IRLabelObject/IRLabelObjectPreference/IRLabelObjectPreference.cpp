
#include "IRLabelObjectPreference.hpp"




IRLabelObjectPreference::IRLabelObjectPreference(String title, Rectangle<int> frameRect) :
IRPreferenceObject(title,frameRect)
{
    std::cout << "IRLABELOBJECTPREFERENCE CREATED??" << std::endl;
    this->fontGUI = new FontGUI(title);
    addAndMakeVisible(this->fontGUI);
    this->fontGUI->addChangeListener(this);
}


IRLabelObjectPreference::~IRLabelObjectPreference()
{
    std::cout << "IRLABELOBJECTPREFERENCE DESTROYED??" << std::endl;
}


void IRLabelObjectPreference::resized()
{
    IRPreferenceObject::resized();
    
    this->fontGUI->setBounds(0, 0, getWidth(), getHeight());
}


void IRLabelObjectPreference::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == this->fontGUI)
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
    return this->fontGUI;
}




