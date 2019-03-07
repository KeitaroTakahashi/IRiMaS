
#include "IRAudioPlayerObject.hpp"




IRSoundPlayerObject::IRSoundPlayerObject(Component* parent):
IRNodeObject(parent,"IRSoundPlayer")
{
    this->player = new AudioPlayer_threadSafe();
    addAndMakeVisible(this->player);
    childComponentManager(this->player);
    // in case we use any AudioSources, we must add them to the internal mixer via this method.
    addAudioComponent(this->player);
    
    setSize(100,118);
}


IRSoundPlayerObject::~IRSoundPlayerObject()
{
    
}


IRNodeObject* IRSoundPlayerObject::copyThis()
{
    return new IRSoundPlayerObject(this->parent);
}


void IRSoundPlayerObject::resized()
{
    this->player->setBounds(0,0,getWidth(),getHeight());
}

void IRSoundPlayerObject::paint(Graphics& g)
{
    if (isEditMode())
    {
        auto area = getLocalBounds().reduced (2);
        
        g.setColour (SYSTEMCOLOUR.contents);
        g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
    }
}


AudioPlayer_threadSafe* IRSoundPlayerObject::getAudioSource()
{
    return this->player;
}



