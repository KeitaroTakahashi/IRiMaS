//
//  IRSpectrogramObject.hpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#ifndef IRSpectrogramObject_hpp
#define IRSpectrogramObject_hpp

#include "IRSpectrogramWithPreference.hpp"
#include "IRSpectrogramController2.h"
class IRSpectrogramObject : public IRNodeObject,
IRNodeObject::Listener
{
public:
    IRSpectrogramObject(Component* parent, IRStr* str);
    ~IRSpectrogramObject();
    // ==================================================

    IRNodeObject* copyThisObject() override; // copy constructor
    IRNodeObject* copyContents(IRNodeObject* object) override;

    t_json saveThisToSaveData() override;
    void loadThisFromSaveData(t_json data) override;
    // ==================================================

    void onResized() override;
    void paint(Graphics& g) override;
    // ==================================================

    // refresh OpenGL when added
    void heavyComponentRefreshed() override;
    // ==================================================
    void mouseUpEvent(const MouseEvent& e) override;
    
    // ==================================================
    void audioFileOpenAction();
    // for callback
    void audioFileImportCompleted();
    
    // ==================================================

    
    std::unique_ptr<IRSpectrogramController2> controller;
    // ==================================================


private:
    // ==================================================

    void statusChangedCallback(IRNodeComponentStatus status) override;
    // events
    void mouseDownEvent(const MouseEvent& e) override;
    
    void IRChangeListenerCallback (ChangeBroadcaster* source) override;
    void audioControllerChangeListener();

    
    bool isBeingResized = false;

    // from IRNodeObject
    void moveToFrontAction() override;
    // ==================================================

    std::shared_ptr<IRSpectrogramWithPreference> UI;
    // ==================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramObject)

};

#endif /* IRSpectrogramObject_hpp */
