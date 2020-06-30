//
//  IRVideoAnnotaterInterfaceObject.hpp
//  IRiMaS
//
//  Created by Keitaro on 19/01/2020.
//

#ifndef IRVideoAnnotaterInterfaceObject_hpp
#define IRVideoAnnotaterInterfaceObject_hpp

#include "IRVideoAnnotaterObject2.hpp"
#include "IRVideoPlayerController.hpp"


class IRVideoAnnotaterInterfaceObject : public IRVideoAnnotaterObject2
{
public:
    IRVideoAnnotaterInterfaceObject(Component* parent, IRStr* str);
    ~IRVideoAnnotaterInterfaceObject();
    // --------------------------------------------------
    
    void onResized() override;
    // copy related methods
    
    IRNodeObject* copyThisObject() override;
    
    IRNodeObject* copyContents(IRNodeObject* object) override;
    
    IRNodeObject* copyDragDropContents(IRNodeObject* object) override;
    
    t_json saveThisToSaveData() override;

    void loadThisFromSaveData(t_json data) override;
    
    // called when this object position is changed
    void ObjectPositionChanged(int x, int y) override;


    // --------------------------------------------------
    
    
    void annotaterClosedAction();
    void annotaterOpenedAction();
    void applyAnnotation();
    // --------------------------------------------------
    
    
    
    // --------------------------------------------------

private:
    // --------------------------------------------------
    void videoLoadCompletedCallback() override;
    void IRChangeListenerCallback (ChangeBroadcaster* source) override;
    
    void arrangeControllerChangedCallback(ArrangeController* source);
    // --------------------------------------------------

    std::shared_ptr<IRVideoPlayerController> controller;
    // --------------------------------------------------
    
    void ObjectBoundsChanged(Rectangle<int> bounds) override;
    // --------------------------------------------------

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoAnnotaterInterfaceObject)

};

#endif /* IRVideoAnnotaterInterfaceObject_hpp */
