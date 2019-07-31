//
//  IRSpectrogramObject.hpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#ifndef IRSpectrogramObject_hpp
#define IRSpectrogramObject_hpp

#include "IRSpectrogramWithPreference.hpp"

class IRSpectrogramObject : public IRNodeObject,
IRNodeObject::Listener,
private ChangeListener
{
public:
    IRSpectrogramObject(Component* parent);
    ~IRSpectrogramObject();
    // ==================================================

    IRNodeObject* copyThis() override; // copy constructor
    IRNodeObject* copyContents(IRNodeObject* object) override;

    t_json saveThisToSaveData() override;
    void loadThisFromSaveData(t_json data) override;
    // ==================================================

    void resized() override;
    void paint(Graphics& g) override;
    
    // refresh OpenGL when added
    void heavyComponentRefreshed() override;
    // ==================================================
    void mouseUpEvent(const MouseEvent& e) override;
    
    // ==================================================

private:
    // ==================================================

    void statusChangedCallback(IRNodeComponentStatus status) override;
    // events
    void mouseDownEvent(const MouseEvent& e) override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    bool isBeingResized = false;

    // ==================================================

    std::shared_ptr<IRSpectrogramWithPreference> UI;
    // ==================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramObject)

};

#endif /* IRSpectrogramObject_hpp */
