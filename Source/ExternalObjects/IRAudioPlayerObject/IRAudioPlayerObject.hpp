//
//  IRAudioPlayerObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 23/08/2018.
//

#ifndef IRAudioPlayerObject_hpp
#define IRAudioPlayerObject_hpp

#include "IRAudioReader.hpp"
#include "SoundPlayer.hpp"
#include "IRNodeObject.hpp"

class IRSoundPlayerObject : public IRNodeObject
{
public:
    IRSoundPlayerObject(Component* parent):
    IRNodeObject(parent,"IRSoundPlayer")
    {
        this->player = new AudioPlayer_threadSafe();
        addAndMakeVisible(this->player);
        childComponentManager(this->player);
        // in case we use any AudioSources, we must add them to the internal mixer via this method.
        addAudioComponent(this->player);
        
        setSize(100,118);
    }
    ~IRSoundPlayerObject() {}

    IRNodeObject* copyThis() override { return new IRSoundPlayerObject(this->parent); }
    void resized() override
    {
        this->player->setBounds(0,0,getWidth(),getHeight());
    }
    
    void paint(Graphics& g) override
    {
        if(isEditMode())
        {
            auto area = getLocalBounds().reduced (2);
        
            g.setColour (SYSTEMCOLOUR.contents);
            g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
        }
    }

    // ------------------------------------------------------------
    
    AudioPlayer_threadSafe* getAudioSource() { return this->player; }
    
    
private:
    AudioPlayer_threadSafe *player;
    
};

#endif /* IRAudioPlayerObject_hpp */
