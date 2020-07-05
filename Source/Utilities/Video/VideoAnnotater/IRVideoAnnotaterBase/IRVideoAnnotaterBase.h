//
//  IRVideoAnnotaterBase.h
//  IRiMaS
//
//  Created by Keitaro on 12/02/2020.
//

#ifndef IRVideoAnnotaterBase_h
#define IRVideoAnnotaterBase_h

// the base class of IRVideoAnnotater.
// This class is passed to the IRVideoAnnotaterDelegate
// add any classes which might be called by other classes outside of IRVideoAnnotater
// so that we do not use many std::function<> stuff.


class IRVideoAnnotaterBase
{
public:
    IRVideoAnnotaterBase()
    {
        
    }
    
    virtual ~IRVideoAnnotaterBase() {}
    // ==================================================
    
    virtual void eventModifiedAction(Component* modifiedEvent) {};
    virtual void eventSelectedAction(Component* selectedEvent) {};
    virtual void newEventCreatedAction(Component* selectedEvent) {};
    virtual void eventTimeCodeChangedAction(Component* changedEvent) {};
    virtual void updateAnnotation() {};
    virtual void showEventPosition(Component* event) {};
    virtual void eventActivationChanged(Component* changedEvent) {};

    // ==================================================
    
    void setVideoSize(juce::Point<int> newVideoSize) { this->videoSize = newVideoSize; }
    juce::Point<int> getVideoSize() const { return this->videoSize; }
    // ==================================================
 

private:
    // ==================================================
    juce::Point<int> videoSize;

    // ==================================================
    // ==================================================

};

#endif /* IRVideoAnnotaterBase_h */
