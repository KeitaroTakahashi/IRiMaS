//
//  IRVideoAnnotatorDelegate.h
//  IRiMaS
//
//  Created by Keitaro on 12/02/2020.
//

#ifndef IRVideoAnnotatorDelegate_h
#define IRVideoAnnotatorDelegate_h

#include "JuceHeader.h"
#include "IRVideoAnnotaterBase.h"
class IRVideoAnnotaterDelegate
{
public:
    IRVideoAnnotaterDelegate(IRVideoAnnotaterBase* base) :
    base ( base )
    {
        
    }
    
    ~IRVideoAnnotaterDelegate() {}
    
    // ==================================================
    
    void eventModifiedAction(Component* modifiedEvent)
    {
        this->base->eventModifiedAction(modifiedEvent);
    }
    
    void eventSelectedAction(Component* selectedEvent)
    {
        this->base->eventSelectedAction(selectedEvent);
    }
    
    void newEventCreatedAction(Component* createdEvent)
    {
        this->base->newEventCreatedAction(createdEvent);
    }
    
    void eventTimeCodeChangedAction(Component* changedEvent)
    {
        this->base->eventTimeCodeChangedAction(changedEvent);
    }
    // ==================================================
    
    void setVideoSize(Point<int> newVideoSize)
    {
        this->base->setVideoSize( newVideoSize );
    }
    
    Point<int> getVideoSize() const
    {
        return this->base->getVideoSize();
    }
    
    
    // ==================================================

    IRVideoAnnotaterBase* getBase() { return this->base; }
private:
    // ==================================================
    
    IRVideoAnnotaterBase* base = nullptr;

    
    
    // ==================================================

};

#endif /* IRVideoAnnotatorDelegate_h */
