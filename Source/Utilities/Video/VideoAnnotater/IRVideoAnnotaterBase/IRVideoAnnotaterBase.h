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

    // ==================================================
    
    void setVideoSize(Point<int> newVideoSize) { this->videoSize = newVideoSize; }
    Point<int> getVideoSize() const { return this->videoSize; }
    // ==================================================
 

private:
    // ==================================================
    Point<int> videoSize;

    // ==================================================
    // ==================================================

};

#endif /* IRVideoAnnotaterBase_h */
