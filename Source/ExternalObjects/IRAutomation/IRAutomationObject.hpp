//
//  IRAutomationObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 23/10/2018.
//

#ifndef IRAutomationObject_hpp
#define IRAutomationObject_hpp

#include "IRNodeObject.hpp"
#include "IRObjectSelection.hpp"
#include "IRAutomationObjectPreference.h"
#include "IRAutomationUI.hpp"
#include "IRAnalysisData.h"

class IRAutomationObject : public IRNodeObject,
                           public ChangeListener
{
public:
    IRAutomationObject(Component *parent) :
    IRNodeObject(parent, "IRAutomation")
    {
        this->preference = new IRAutomationObjectPreference("Automation Preference", Rectangle<int>(400,720));

        this->UI = new IRAutomationUI(this);
        this->UI->setBounds(this->xMargin,
                            this->yMargin,
                            getWidth()-(this->xMargin*2),
                            getHeight()-(this->yMargin*2));
        addAndMakeVisible(this->UI);
        this->UI->demoData(1260);
        
        setSize(300, 100);
        
        // get
        IRObjectPtr val = nullptr;
        String id = "IRAnalysisMagnitude";
        val = this->callGetObjectGlobal(id);
        
        if(val != nullptr)
        {
            IRAnalysisDataStr* magData = static_cast<IRAnalysisDataStr*>(val);
            std::cout << "fftsize = " << magData->fftsize << " : nframe = " << magData->nframe << std::endl;
        }else
        {
            std::cout << "Error : could not load " << id << std::endl;
        }

    }
    
    ~IRAutomationObject()
    {
        delete this->preference;
        delete this->UI;
    }
    
    // ------------------------------------------------------------
    IRNodeObject* copyThis() override
    {
        IRAutomationObject* newObj = new IRAutomationObject(this->parent);
        return newObj;
    }
    // ------------------------------------------------------------
    void loadThisFromSaveData(t_json saveData) override
    {
        
    }
    
    // ------------------------------------------------------------
    void paint(Graphics &g) override
    {
        
        auto area = getLocalBounds().reduced (2);
            
        g.setColour (SYSTEMCOLOUR.contents);
            //g.drawRoundedRectangle (area.toFloat(), 5.0f, 4.0f);
        g.drawRect(getLocalBounds().toFloat(), 1.0);
      
        
    }
    // ------------------------------------------------------------
    void resized() override
    {
        this->UI->setSize(getWidth(), getHeight());
    }
    // ------------------------------------------------------------
    
    void mouseDownEvent(const MouseEvent& e) override
    {
        //change preference Window if not yet
        
        IRPreferenceSpace* space = getPreferenceWindow()->getPreferenceSpace();
        
        IRPreferenceObject* current = space->getPreferenceObj();
        
        std::cout << "PreferenceOBject = " << preference << std::endl;
        
        if(current != preference){
            space->setPreferenceObj(preference);
        }
        
    }
    //===============================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }
    //===============================================================

    IRAutomationObjectPreference *preference;
    
private:
    
    /*
    bool keyPressed (const KeyPress& key,
                     Component* originatingComponent) override
    {
        
        return true;
    }*/
    
    
    // ------------------------------------------------------------
    // call back function automatically called when the status of this object changed by others.
    // write some tasks here
    void statusChangedCallback(IRNodeComponentStatus status) override
    {
        switch (status)
        {
            case EditModeStatus:
                if(isEditMode())
                {
                    setWantsKeyboardFocus(false);
                    //removeKeyListener(this);
                }else{
                    setWantsKeyboardFocus(true);
                    //addKeyListener(this);
                    
                }
                
                break;
            case SelectableStatus:
                break;
            case HasResizedStatus:
                break;
            default:
                break;
        }
    }
    
    IRAutomationUI *UI;
    
    int xMargin = 5;
    int yMargin = 5;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRAutomationObject)

};


#endif /* IRAutomationObject_hpp */
