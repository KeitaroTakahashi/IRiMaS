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

    virtual void IRKeyPressed(int keyCode, String keyText);
    virtual void IRKeyReleased(int keyCode, String keyText);
    
    // ==================================================
    // status change
    
    virtual void setEditMode(bool newEditMode) {};
    // --------------------------------------------------

    virtual void audioPtrDelivery(IRAudio* obj) {};
    virtual void textPtrDelivery(IRText* obj) {};
    virtual void imagePtrDelivery(IRImage* obj) {};
    virtual void dataPtrDelivery(IRData* obj) {};
    virtual void videoPtrDelivery(IRVideo* obj) {};
    // --------------------------------------------------
    // ==================================================
    
    // ==================================================

    // object
    IRNodeObject* nodeObject;
    // --------------------------------------------------
private:
    bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override;
    bool keyPressed(const KeyPress &key,
                    Component* originatingComponent) override;
    
    // ==================================================
    // get signal from IRNodeComponent when IRNodeObject status changed.
    void NodeObjectStatusChanged(IRNodeComponentStatus status);
    
    // ==================================================
    // NodeObject Listener
    void receiveAudioLink(IRNodeObject* obj) override;
    void receiveTextLink(IRNodeObject* obj) override;
    void receiveImageLink(IRNodeObject* obj) override;
    void receiveDataLink(IRNodeObject* obj) override;
    void receiveVideoLink(IRNodeObject* obj) override;
    
    // called when IRFileManager is given or updated in IRNodeObject
    void updateIRFileManager(IRFileManager* fileManager) override;
    // --------------------------------------------------
    
    

    // --------------------------------------------------
    // for this class
    void setEditModeBase(bool newEditMode);
    
    // ==================================================
    

private:
    
    //key event
    int pressedKeyCode;
    String pressedKeyText;
    
    // status
    bool editModeFlag = true;

    
};

#endif /* IRUIFoundation_hpp */
