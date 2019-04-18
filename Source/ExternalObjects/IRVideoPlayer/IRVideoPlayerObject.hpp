//
//  IRVideoPlayerObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/01/2019.
//

#ifndef IRVideoPlayerObject_hpp
#define IRVideoPlayerObject_hpp

#include "IRNodeObject.hpp"
#include "IRVideoPlayer.hpp"


class IRVideoPlayerObject : public IRNodeObject
{
    
public:
    
    IRVideoPlayerObject(Component* parent) : IRNodeObject(parent, "IRVideoPlayer", NodeObjectType(heavyWeightComponent))
    {
        // this->videoPlayer = new IRVideoPlayer();
        this->videoPlayer = std::make_shared<IRVideoPlayer>(this);
        this->videoPlayer->setBounds(getLocalBounds().reduced(5));
        this->videoPlayer->videoLoadCompleted = [this]{ videoLoadCompletedAction(); };
        addAndMakeVisible(this->videoPlayer.get());
    }
    ~IRVideoPlayerObject()
    {
        // delete this->videoPlayer;
    }
    
    // --------------------------------------------------
    // copy related methods
    
    IRNodeObject* copyThis() override;
    
    IRNodeObject* copyContents(IRNodeObject* object) override;
    
    IRNodeObject* copyDragDropContents(IRNodeObject* object) override;
    
    // --------------------------------------------------
    
    t_json saveThisToSaveData() override;
    
    // --------------------------------------------------
    
    void loadThisFromSaveData(t_json data) override;
    
    // --------------------------------------------------
    // resize reated methods
    void resized() override;
    
    void resizeThisComponentEvent(const MouseEvent& e) override;
    
    // --------------------------------------------------
    
    void mouseUpEvent(const MouseEvent& e) override;
    
    // --------------------------------------------------

    void paint(Graphics& g) override;
    
    // --------------------------------------------------
    // call back function by IRVideoPlayer
    void videoLoadCompletedAction();
    
    // --------------------------------------------------

    IRVideoPlayer* getVideoPlayer() { return this->videoPlayer.get(); }

    // --------------------------------------------------

private:
    // IRVideoPlayer *videoPlayer;
    std::shared_ptr<IRVideoPlayer> videoPlayer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoPlayerObject)
};

#endif /* IRVideoPlayerObject_hpp */
