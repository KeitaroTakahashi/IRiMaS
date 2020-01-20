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
