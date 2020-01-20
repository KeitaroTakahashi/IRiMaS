//
//  EventLogList.h
//  IRiMaS
//
//  Created by Keitaro on 19/12/2019.
//

#ifndef EventLogList_h
#define EventLogList_h
#include "IRViewPort.hpp"
#include "EventLogListComponent.h"

class EventLogList : public IRViewPort
{
public:
    EventLogList(IRStr* str) : IRStrComponent(str)
    {
        
    }
    
    ~EventLogList()
    {
        clearAllEventComponent();
    }
    //==================================================

    void resized() override
    {
        
    }
    
    void paint(Graphics& g) override
    {
        
    }
    //==================================================
    //==================================================
    //==================================================

private:
    
    class Component4ViewPort : public Component
    {
    public:
        Component4ViewPort(Component* main) : main(main)
        {
            addAndMakeVisible(main);
        }
        
        ~Component4ViewPort() {}
        
        void resized() override
        {
            this->main->setBounds(0, 0, getWidth(), getHeight());
        }

    private:
        Component* main;
        
    };
    //==================================================
    std::shared_ptr<Component4ViewPort> viewPort;

    //==================================================
    //==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EventLogList)

};

#endif /* EventLogList_h */
