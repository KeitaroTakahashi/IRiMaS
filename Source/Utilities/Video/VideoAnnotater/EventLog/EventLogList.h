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
#include "NothingSelectedUI.h"

class EventLogList : public IRViewPort
{
public:
    EventLogList(IRStr* str) : IRViewPort(str)
    {
        this->nothingSelectedUI.reset(new NothingSelectedUI(str));
        createLogComponent(this->nothingSelectedUI.get());
        
    }
    
    ~EventLogList()
    {
        this->viewPort.reset();
        this->logComponent.reset();
    }
    //==================================================
    void createLogComponent(Component* comp)
    {
        
        this->logComponent.reset( new EventLogListComponent(getStr()));
        this->logComponent->newComponentAddedCallback = [this]{ newComponentAddedAction(); };
        
        if(comp != nullptr) this->logComponent->setComponent(comp);
        
        this->viewPort.reset(new Component4ViewPort(this->logComponent.get()));
        setViewedComponent(this->viewPort.get());
        
        this->hasComponent = true;
    }
    
    //==================================================

    void resized() override
    {
        this->viewPort->setBounds(0,0,getWidth()-10, getHeight());
        this->logComponent->setBounds(0, 0, getWidth()-10, getHeight());
        this->nothingSelectedUI->setBounds(getLocalBounds());
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
    
    void removeLogComponent()
    {
        this->logComponent->setComponent(this->nothingSelectedUI.get());
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
            if(this->main != nullptr)
                this->main->setBounds(0, 0, getWidth(), getHeight());
        }
        
        void removeMainComponent()
        {
            this->main = nullptr;
        }
        
        void addMainComponent(Component* main)
        {
            addAndMakeVisible(main);
        }

    private:
        Component* main;
        
    };
    //==================================================
    std::shared_ptr<Component4ViewPort> viewPort;

    std::shared_ptr<EventLogListComponent> logComponent;
    
    //==================================================
    
    bool hasComponent = false;
    
    std::shared_ptr<NothingSelectedUI> nothingSelectedUI;
    //==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EventLogList)

};

#endif /* EventLogList_h */
