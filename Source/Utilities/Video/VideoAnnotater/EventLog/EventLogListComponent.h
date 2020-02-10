//
//  EventLogListComponent.h
//  IRiMaS
//
//  Created by Keitaro on 19/12/2019.
//

#ifndef EventLogListComponent_h
#define EventLogListComponent_h

class EventLogListComponent : public Component,
public IRStrComponent
{
public:
    EventLogListComponent(IRStr* str) : IRStrComponent(str)
    {
        
    }
    
    ~EventLogListComponent()
    {
        
    }
    // ==================================================
    void resized() override
    {
        if(this->comp != nullptr)
        {
            this->comp->setBounds(getLocalBounds());
        }
    }
    
    // ==================================================

    int getTotalComponentHeight()
    {
        if(this->comp == nullptr) return 0;
        else return this->comp->getHeight();
    }
    // ==================================================
    // ==================================================
    // ==================================================
    // ==================================================
    // ==================================================
    
    std::function<void()> newComponentAddedCallback;

private:
    // ==================================================
    
    Component* comp = nullptr;
    // ==================================================
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EventLogListComponent)

};
#endif /* EventLogListComponent_h */
