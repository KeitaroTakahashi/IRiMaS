
#include "IRPreferenceSpace.hpp"



IRPreferenceSpace::IRPreferenceSpace(String title, Rectangle<int> frameRect)
{
    this->title = title;
    setSize(frameRect.getWidth(), frameRect.getHeight());
    
    
}


IRPreferenceSpace::~IRPreferenceSpace()
{
    
}


void IRPreferenceSpace::resized()
{
    if (this->isPrefereneObjAlreadySet == true)
    {
        preferenceObj->setBounds(10, 50, 100, 400);
    }
}


void IRPreferenceSpace::paint(Graphics& g)
{
    g.fillAll(Colours::white);
}


String IRPreferenceSpace::getTitle()
{
    return this->title;
}


void IRPreferenceSpace::changeListenerCallback(ChangeBroadcaster* source)
{
    
}


void IRPreferenceSpace::setPreferenceObj(IRPreferenceObject* preferenceObj)
{
    removeAllChildren();
    addAndMakeVisible(preferenceObj);
    this->preferenceObj = preferenceObj;
    preferenceObj->setBounds(10, 50, 100, 400);
    
    this->isPrefereneObjAlreadySet = true;
}


IRPreferenceObject* IRPreferenceSpace::getPreferenceObj()
{
    return this->preferenceObj;
}




