//
//  IRUIFoundation.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 04/03/2019.
//

#ifndef IRUIFoundation_hpp
#define IRUIFoundation_hpp

#include "JuceHeader.h"
#include "IRNodeObject.hpp"

class IRUIFoundation : public Component,
public KeyListener
{
public:
    IRUIFoundation(IRNodeObject* parent);
    ~IRUIFoundation();
    
    // --------------------------------------------------
    // ==================================================
    // key events
    
public:
    virtual void IRKeyPressed(int keyCode);
    virtual void IRKeyReleased(int keyCode);
    
    // ==================================================
    // status change
    
    virtual void setEditMode(bool newEditMode) {};
    // --------------------------------------------------
    



    // --------------------------------------------------
private:
    bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override;
    bool keyPressed(const KeyPress &key,
                    Component* originatingComponent) override;
    
    // ==================================================
    // get signal from IRNodeComponent when IRNodeObject status changed.
    void NodeObjectStatusChanged(IRNodeComponentStatus status);
    // --------------------------------------------------
    
    

    // --------------------------------------------------
    // for this class
    void setEditModeBase(bool newEditMode);
    
    // ==================================================


private:
    
    //key event
    int pressedKeyCode;
    
    // status
    bool editModeFlag = true;
        
    // object
    
    IRNodeObject* parent;
    
};

#endif /* IRUIFoundation_hpp */
