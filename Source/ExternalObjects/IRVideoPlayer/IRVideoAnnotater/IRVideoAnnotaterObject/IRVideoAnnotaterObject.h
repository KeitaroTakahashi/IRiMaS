//
//  IRVideoAnnotaterObject.h
//  IRiMaS
//
//  Created by Keitaro on 13/12/2019.
//

#ifndef IRVideoAnnotaterObject_hpp
#define IRVideoAnnotaterObject_hpp

#include "IRVideoPlayerObject.hpp"
#include "IRVideoAnnotationComponentHeader.h"
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
    virtual void resized() override
    {
        IRVideoPlayerObject::resized();
        
        int visibleIndex = 1;
        
        int videoY = getVideoPlayer()->getY();

        for(auto comp : this->visibleAnnotationComponents)
        {
            //margin x = 5
            //comp->setBounds(0, getHeight() - videoY - (60 * visibleIndex), getWidth(), 60);
            //auto a = comp->getEvent()->getIRTextEditorObject()->controller->getArrangeController();
            
            if(comp->getEvent()->getType() == VideoAnnotationEventComponent::VideoAnnotationType::TEXT)
            {
                auto c = static_cast<IRVideoAnnotationTextComponent* >(comp);
                auto a = c->getIRTextEditorObject()->controller->getArrangeController();
                
                std::cout << "text ecent x = " << a->getRectangle().getX() << ", " << a->getRectangle().getY() << " : " << a->getRectangle().getWidth() << " , " << a->getRectangle().getHeight() << std::endl;
                    
                c->setBounds(a->getRectangle());
                //comp->setBounds(a->getRectangle());
            }
            
            //auto a = comp->getEvent()->getIRTextEditorObject()->controller->getArrangeController();
            //int h = getHeight() - videoY - comp->getHeight() ;
            /*
            if(h > getHeight())
            {
                h = getHeight();
            }*/
            //comp->setTopLeftPosition(0, h);
            //comp->setTopLeftPosition(comp->getX() - 230, comp->getY() - 5);
            //if(comp->getHeight() > getHeight())
              //  comp->setSize(getWidth(), getHeight());
             //comp->setSize(getWidth(), comp->getHeight());
            
            visibleIndex ++;
        }
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
        // reset
        clearVisibleAnnotationComponents();
        clearAnnotationComponent();
        
        for(auto e : events)
        {
            createAnnotationComponent(e);
        }
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
        }
    }
    
    void updateTEXTEventComponent(IRVideoAnnotationComponent* c, VideoAnnotationEventComponent* e)
    {
        // just copy the textEditor
        auto comp = static_cast<IRVideoAnnotationTextComponent* >(c);
        auto event = static_cast<AnnotationTextEventComponent* >(e);
        this->event = event;
        
        // adjust eventComponent's width size to the current video size
        // this is temporarl remedy
        //auto videoBounds = this->getVideoPlayer()->getCurrentVideoBounds();
        
        //auto arrangeController = event->getIRTextEditorObject()->controller->getArrangeController();
        //
        
        //IRVideoAnnotationTextComponent
        //comp->getIRTextEditorObject()->controller->getArrangeController()->setArrangeController(arrangeController);
        
        //std::cout << "arrangeController " << arrangeController->getLabelWidth() << ", " << arrangeController->getLabelHeight() << std::endl;
        // textEditor height can not be greater than the video height
        /*
        int h = 0;
        if(event->getHeight() > videoBounds.getHeight())
        {
            h = videoBounds.getHeight();
            std::cout << "fixed height = " << h << std::endl;
        }else h = event->getHeight();
        */
        //comp->setSize(videoBounds.getWidth(), h);
        //comp->setSize(arrangeController->getLabelWidth(), arrangeController->getLabelHeight());
        
        comp->setIRTextEditorObject(event->getIRTextEditorObject());
        // set previous size so that component does not need to be resized everytime when the size is the same.
        //comp->setPreviousBounds(getWidth(), event->getHeight());
        //comp->setPreviousBounds(arrangeController->getLabelWidth(), arrangeController->getLabelHeight());

        // update time code
        comp->setTimeCode(event->getBeginTimeCode(), event->getEndTimeCode());
        
        
    }
    
    // --------------------------------------------------

    IRVideoAnnotationComponent* getVideoAnnotationComponentOf(VideoAnnotationEventComponent* event)
    {
        for(auto c : this->annotationComponents)
        {
            if(c->getParentEvent() == event)
            {
                return c;
            }
        }
    }
    
    // --------------------------------------------------

    void videoPlayingUpdateCallback(double pos) override
    {
        //std::cout << "pos = " << pos << std::endl;
        // first reset the list of the visible annotation components
        clearVisibleAnnotationComponents();
        
        using t = VideoAnnotationEventComponent::VideoAnnotationType;

        for(auto comp : this->annotationComponents)
        {
            
            //std::cout << "begin = " << comp->getBeginTimeInSec() << " : end = " << comp->getEndTimeInSec() << std::endl;
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
        }
        
        if(this->videoPlayingUpdate != nullptr)
            this->videoPlayingUpdate(pos);
        
        resized();
    }
    std::function<void(double pos)> videoPlayingUpdate;
    
    // --------------------------------------------------
    
    bool findEventComponent(VideoAnnotationEventComponent* event)
    {
        return false;
    }
    // --------------------------------------------------
    // --------------------------------------------------

    
private:
    // --------------------------------------------------
    std::vector<IRVideoAnnotationComponent* > annotationComponents;
    std::vector<IRVideoAnnotationComponent* > visibleAnnotationComponents;
    void clearVisibleAnnotationComponents()
    {
        this->visibleAnnotationComponents.clear();
    }
    void addVisibleAnnotationComponents(IRVideoAnnotationComponent* visibleComponent)
    {
        this->visibleAnnotationComponents.push_back(visibleComponent);
    }
    void removeVisibleAnnotationComponents(IRVideoAnnotationComponent* visibleComponent)
    {
        auto it = std::find(this->visibleAnnotationComponents.begin(),
                            this->visibleAnnotationComponents.end(),
                            visibleComponent);
        if (it != this->visibleAnnotationComponents.end())
        {
          this->visibleAnnotationComponents.erase(it);
        }
    }

    // --------------------------------------------------
    void clearAnnotationComponent()
    {
        for(auto comp : this->annotationComponents)
        {
            delete comp;
        }
        this->annotationComponents.clear();
    }
    

    void addAnnotationComponent(IRVideoAnnotationComponent* component)
    {
        component->sizeChanged = [this]{ resized(); };
        this->annotationComponents.push_back(component);
    }
    
    void createTEXTAnnotationComponent(VideoAnnotationEventComponent* event)
    {
        std::cout << "createTEXTAnnotationComponent\n";
        auto e = static_cast<AnnotationTextEventComponent* >(event);
        
        IRVideoAnnotationTextComponent* comp = new IRVideoAnnotationTextComponent(getStr(), e);
        
        comp->setIRTextEditorObject(e->getIRTextEditorObject());
        comp->getIRTextEditorObject()->setObjectBounds(0, 0, 100, 100);
        comp->setParentEvent(event);
        addAndMakeVisible(comp);
        comp->setVisible(false);
        addAnnotationComponent(comp);
        
        resized();
    }
    // --------------------------------------------------

    AnnotationTextEventComponent* event = nullptr;
    // --------------------------------------------------
    // --------------------------------------------------
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoAnnotaterObject)

};

#endif /* IRVideoAnnotaterObject_hpp */
