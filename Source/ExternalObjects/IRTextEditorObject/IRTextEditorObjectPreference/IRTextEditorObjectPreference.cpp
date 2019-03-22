
#include "IRTextEditorObjectPreference.hpp"




IRTextEditorObjectPreference::IRTextEditorObjectPreference(String title, Rectangle<int> frameRect) :
IRPreferenceObject(title,frameRect)

{
    // this->fontGUI = new FontGUI(title);
    this->fontGUI = std::make_unique<FontGUI>(title);
    addAndMakeVisible(this->fontGUI.get());
    this->fontGUI->addChangeListener(this);
}


IRTextEditorObjectPreference::~IRTextEditorObjectPreference()
{
    
}


void IRTextEditorObjectPreference::resized()
{
    IRPreferenceObject::resized();
    
    this->fontGUI->setBounds(0,0,getWidth(), getHeight());
}


void IRTextEditorObjectPreference::initialize()
{
    
}


void IRTextEditorObjectPreference::changeListenerCallback(ChangeBroadcaster* source)
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


FontGUI* IRTextEditorObjectPreference::getFontGUI()
{
    return this->fontGUI.get();
}





