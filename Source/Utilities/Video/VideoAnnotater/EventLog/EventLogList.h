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
    EventLogList(IRStr* str) : IRViewPort(str)
    {
        this->logComponent.reset( new EventLogListComponent(str));
        this->logComponent->newComponentAddedCallback = [this]{ newComponentAddedAction(); };
        
        this->viewPort.reset(new Component4ViewPort(this->logComponent.get()));
        setViewedComponent(this->viewPort.get());

    }
    
    ~EventLogList()
    {
        this->viewPort.reset();
        this->logComponent.reset();
    }
    //==================================================

    void resized() override
    {
        
        this->viewPort->setBounds(0,0,getWidth()-10, getHeight());
        this->logComponent->setBounds(0, 0, getWidth()-10, getHeight());

        /*
        int listCompHeight = this->logComponent->getTotalComponentHeight();
        this->viewPort->setBounds(0,0,getWidth()-10, listCompHeight);
        this->logComponent->setSize(getWidth()-10, listCompHeight);
         */
    }
    
    void paint(Graphics& g) override
    {
        
    }
    //==================================================
    
    void newComponentAddedAction()
    {
        
    }
    //==================================================
    
    void setLogComponent(Component* comp)
    {
        this->logComponent->setComponent(comp);
        
    }
 
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

    std::shared_ptr<EventLogListComponent> logComponent;
    
    //==================================================
    //==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EventLogList)

};

#endif /* EventLogList_h */
