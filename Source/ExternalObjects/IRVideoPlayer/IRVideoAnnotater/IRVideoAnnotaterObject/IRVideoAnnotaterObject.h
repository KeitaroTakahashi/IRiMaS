//
//  IRVideoAnnotaterObject.h
//  IRiMaS
//
//  Created by Keitaro on 13/12/2019.
//

#ifndef IRVideoAnnotaterObject_hpp
#define IRVideoAnnotaterObject_hpp

#include "IRVideoPlayerObject.hpp"
#include "AnnotationTextEventComponent.h"
#include "VideoAnnotationEventComponent.h"

class IRVideoAnnotaterObject : public IRVideoPlayerObject
{
public:
    
    IRVideoAnnotaterObject(Component* parent, IRStr* str, bool withOpenButton = true) :
    IRVideoPlayerObject(parent, str, withOpenButton)
    {
        
    }
    ~IRVideoAnnotaterObject()
    {
        clearVisibleAnnotationComponents();
    }
    // --------------------------------------------------
    virtual void onResized() override
    {
        IRVideoPlayerObject::onResized();
      
    }
    // --------------------------------------------------
    
    void updateAnnotationEventOfThis(VideoAnnotationEventComponent* event)
    {
        // if already registered
        if(findEventComponent(event))
        {
            
        }else // if not
        {
            
        }
    }

    void setAnnotationEvents(std::vector<VideoAnnotationEventComponent*> events)
    {
       
    }
    
    void createAnnotationComponent(VideoAnnotationEventComponent* event)
    {
        using t = VideoAnnotationEventComponent::VideoAnnotationType;
        switch(event->getType())
        {
            case t::TEXT:
                createTEXTAnnotationComponent(event);
                break;
            case t::SHAPE:
                break;
            case t::IMAGE:
                break;
            case t::AUDIO:
                break;
            default:
                break;
        }
    }
    
    // --------------------------------------------------

    void updateEventComponent(VideoAnnotationEventComponent* event)
    {
   /*
        for(auto c : this->annotationComponents)
        {
            if(c->getParentEvent() == event)
            {
                
                using t = VideoAnnotationEventComponent::VideoAnnotationType;
                switch(event->getType())
                {
                    case t::TEXT:
                        updateTEXTEventComponent(c, event);
                        break;
                    case t::SHAPE:
                        break;
                    case t::IMAGE:
                        break;
                    case t::AUDIO:
                        break;
                    default:
                        break;
                }
            }
        }*/
    }
    
    
    void updateTEXTEventComponent(VideoAnnotationEventComponent* c, VideoAnnotationEventComponent* e)
    {
        // just copy the textEditor
        auto comp = static_cast<AnnotationTextEventComponent* >(c);
        auto event = static_cast<AnnotationTextEventComponent* >(e);
        
        
        
        //this->event = event;
        
        
        
       // comp->setIRTextEditorObject(event->getIRTextEditorObject());


        // update time code
        //comp->setTimeCode(event->getBeginTimeCode(), event->getEndTimeCode());
        
        
    }
    
    // --------------------------------------------------

    VideoAnnotationEventComponent* getVideoAnnotationComponentOf(VideoAnnotationEventComponent* event)
    {
        for(auto c : this->annotationComponents)
        {
            if(c == event)
            {
                return c;
            }
        }
        return nullptr;
    }
    
    // --------------------------------------------------

    void videoPlayingUpdateCallback(double pos) override
    {
        //std::cout << "pos = " << pos << std::endl;
        // first reset the list of the visible annotation components
        clearVisibleAnnotationComponents();
        
        using t = VideoAnnotationEventComponent::VideoAnnotationType;
/*
        for(auto comp : this->annotationComponents)
        {
            
            if(pos >= comp->getBeginTimeInSec() && comp->getEndTimeInSec() > pos)
            {
                // check if the event is active
                if(comp->getEvent()->isActive())
                {
                    // add to the list
                    addVisibleAnnotationComponents(comp);
                    //highlight
                    comp->getEvent()->setSelected(true);
                    comp->getEvent()->selectedAction(); // to opearate any following actions by selecting
                    // set visible true
                    comp->setVisible(true);
                }
            }else{
                
                if(comp->isVisible())
                {
                    // if the component is visible, then remove from the visible list and set it invisible
                    removeVisibleAnnotationComponents(comp);
                    comp->getEvent()->setSelected(false);
                    comp->setVisible(false);
                }
            }
        }*/
        
        if(this->videoPlayingUpdate != nullptr)
            this->videoPlayingUpdate(pos);
        
    }
    std::function<void(double pos)> videoPlayingUpdate;
    
    // --------------------------------------------------
    
    bool findEventComponent(VideoAnnotationEventComponent* event)
    {
        return false;
    }
    // --------------------------------------------------
    // --------------------------------------------------

    
    
    
    // --------------------------------------------------

private:
    // --------------------------------------------------
    
    // --------------------------------------------------

    std::vector<VideoAnnotationEventComponent* > annotationComponents;
    std::vector<VideoAnnotationEventComponent* > visibleAnnotationComponents;
    void clearVisibleAnnotationComponents()
    {
    }
    
    void addVisibleAnnotationComponents(VideoAnnotationEventComponent* visibleComponent)
    {
    
    }
    
    void removeVisibleAnnotationComponents(VideoAnnotationEventComponent* visibleComponent)
    {

    }

    // --------------------------------------------------
    void clearAnnotationComponent()
    {

    }
    

    void addAnnotationComponent(VideoAnnotationEventComponent* component)
    {

    }
    
    void createTEXTAnnotationComponent(VideoAnnotationEventComponent* event)
    {

    }
    // --------------------------------------------------
    
    // --------------------------------------------------
    // --------------------------------------------------
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoAnnotaterObject)

};

#endif /* IRVideoAnnotaterObject_hpp */
