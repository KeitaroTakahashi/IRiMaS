
#include "IRSoundPlayerObject.hpp"

IRSoundPlayerObject::IRSoundPlayerObject(Component* parent, IRStr* str)
: IRNodeObject(parent, "IRSoundPlayer", str, NodeObjectType(orginaryIRComponent))
{
    this->player = new AudioPlayer_threadSafe(this, str); // FOR SOME UNCLEAR REASON THIS DOES NOT LEAK (FD)
    addAndMakeVisible(this->player);
    childComponentManager(this->player);
    // in case we use any AudioSources, we must add them to the internal mixer via this method.
    addAudioComponent(this->player);
    
    setSize(100, 118);
}

IRSoundPlayerObject::~IRSoundPlayerObject()
{
    // de la même manière que nous ajoutons addAudioComponent au mixeur du projet
    std::cout << "~IRSOUNDPLAYEROBJECT DESTRUCTOR CALLED" << std::endl;
    removeAudioComponent(this->player);
     delete this->player;
}

IRNodeObject* IRSoundPlayerObject::copyThis()
{
    return new IRSoundPlayerObject(this->parent, getStr());
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
        
        g.setColour (getStr()->SYSTEMCOLOUR.contents);
        g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
    }
}

AudioPlayer_threadSafe* IRSoundPlayerObject::getAudioSource()
{
    return this->player;
}



