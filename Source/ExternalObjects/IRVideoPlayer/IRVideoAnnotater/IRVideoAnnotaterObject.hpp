//
//  IRVideoAnnotaterObject.hpp
//  IRiMaS
//
//  Created by Keitaro on 13/12/2019.
//

#ifndef IRVideoAnnotaterObject_hpp
#define IRVideoAnnotaterObject_hpp

#include "IRVideoPlayerObject.hpp"
#include "IRVideoPlayerController.hpp"

class IRVideoAnnotaterObject : public IRVideoPlayerObject,
private ChangeListener
{
public:
    
    IRVideoAnnotaterObject(Component* parent, IRStr* str);
    ~IRVideoAnnotaterObject();
    // --------------------------------------------------
    // --------------------------------------------------
    
    void annotaterClosedAction();
    void annotaterOpenedAction();
    // --------------------------------------------------

    
    
private:
    // --------------------------------------------------

    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    // --------------------------------------------------
    void videoLoadCompletedCallback() override;
    // --------------------------------------------------

    std::shared_ptr<IRVideoPlayerController> controller;
    
    // --------------------------------------------------
    // --------------------------------------------------

};

#endif /* IRVideoAnnotaterObject_hpp */
