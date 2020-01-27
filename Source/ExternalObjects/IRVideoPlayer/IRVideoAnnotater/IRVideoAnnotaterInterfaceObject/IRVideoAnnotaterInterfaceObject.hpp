//
//  IRVideoAnnotaterInterfaceObject.hpp
//  IRiMaS
//
//  Created by Keitaro on 19/01/2020.
//

#ifndef IRVideoAnnotaterInterfaceObject_hpp
#define IRVideoAnnotaterInterfaceObject_hpp

#include "IRVideoAnnotaterObject.hpp"
#include "IRVideoPlayerController.hpp"


class IRVideoAnnotaterInterfaceObject : public IRVideoAnnotaterObject,
private ChangeListener
{
public:
    IRVideoAnnotaterInterfaceObject(Component* parent, IRStr* str);
    ~IRVideoAnnotaterInterfaceObject();
    // --------------------------------------------------
    // copy related methods
    
    IRNodeObject* copyThis() override;
    
    IRNodeObject* copyContents(IRNodeObject* object) override;
    
    IRNodeObject* copyDragDropContents(IRNodeObject* object) override;
    
    t_json saveThisToSaveData() override;

    void loadThisFromSaveData(t_json data) override;

    // --------------------------------------------------
    
    
    void annotaterClosedAction();
    void annotaterOpenedAction();
    // --------------------------------------------------
    
    
    
    // --------------------------------------------------

private:
    // --------------------------------------------------
    void videoLoadCompletedCallback() override;
    void changeListenerCallback (ChangeBroadcaster* source) override;

    // --------------------------------------------------

    std::shared_ptr<IRVideoPlayerController> controller;
    // --------------------------------------------------
    // --------------------------------------------------

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoAnnotaterInterfaceObject)

};

#endif /* IRVideoAnnotaterInterfaceObject_hpp */
