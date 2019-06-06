//
//  IRIVSRObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 31/05/2019.
//

#ifndef IRIVSRObject_hpp
#define IRIVSRObject_hpp

#include "IRNodeObject.hpp"
#include "IRISVObjectPreference.h"
#include "IR3DGraphicUIWithPreference.hpp"

class IRISVObject : public IRNodeObject,
public ChangeListener
{
public:
    IRISVObject(Component* parent);
    ~IRISVObject();
    
    // ------------------------------------------------------------
    IRNodeObject* copyThis() override
    {
        IRISVObject* newObj = new IRISVObject(this->parent);
        return newObj;
    }
    // ------------------------------------------------------------
    
    t_json saveThisToSaveData() override;
    void loadThisFromSaveData(t_json data) override;
    
    // ------------------------------------------------------------
    void paint(Graphics &g) override;
    void resized() override;
    // ------------------------------------------------------------

    void mouseDownEvent(const MouseEvent& e) override;
    void mouseUpEvent(const MouseEvent& e) override;
    
    
    IRISVObjectPreference *preference;

private:
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }
    
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
                    printf("editMode\n");
                    this->UI->stopRendering();
                }else{
                    printf("controlMode\n");
                    this->UI->startRendering();
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
    
    
    std::shared_ptr<IR3DGraphicUIWithPreference> UI;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRISVObject)

};
#endif /* IRIVSRObject_hpp */
