//
//  IRShapeObject.hpp
//  IRiMaS
//
//  Created by Keitaro on 26/07/2019.
//

#ifndef IRShapeObject_hpp
#define IRShapeObject_hpp

#include "IRNodeObject.hpp"
#include "IRShapeUI.hpp"

class IRShapeObject : public IRNodeObject,
public ChangeListener
{
public:
    IRShapeObject(Component* parent);
    ~IRShapeObject();
    
    IRNodeObject* copyThis() override;
    
    
    
    t_json saveThisToSaveData() override;
    void loadThisFromSaveData(t_json data) override;
    
    // ------------------------------------------------------------
    void paint(Graphics &g) override;
    void resized() override;
    // ------------------------------------------------------------
    
    void mouseDownEvent(const MouseEvent& e) override;
    //void mouseUpEvent(const MouseEvent& e) override;
    
private:
    
    std::shared_ptr<IRShapeUI> UI;
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }
    // ------------------------------------------------------------
    // call back function automatically called when the status of this object changed by others.
    // write some tasks here
    void statusChangedCallback(IRNodeComponentStatus status) override
    {
        switch(status)
        {
            case EditModeStatus:
                break;
            default:
                break;
        }
    
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRShapeObject)

    
};


#endif /* IRShapeObject_hpp */
