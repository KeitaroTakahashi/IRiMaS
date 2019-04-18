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
#include "IRFileManager.hpp"

class IRUIFoundation : public Component,
public IRComponents,
public KeyListener,
private IRNodeObject::Listener
{
public:
    IRUIFoundation(IRNodeObject* nodeObject);
    ~IRUIFoundation();
    
    // --------------------------------------------------
    // ==================================================
    // key events

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
    
    
    // called when IRFileManager is given or updated in IRNodeObject
    void updateFileManager(IRFileManager* fileManager);
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
    IRNodeObject* nodeObject;
    
};

#endif /* IRUIFoundation_hpp */
