//
//  IRUIFoundation.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 04/03/2019.
//

#include "IRUIFoundation.hpp"

IRUIFoundation::IRUIFoundation(IRNodeObject* nodeObject)
{
    // to make it efficient to redrawn...
    setOpaque(false);
    
    this->nodeObject = nodeObject;
    this->nodeObject->statusChangeCompleted = [this](IRNodeComponentStatus status){ NodeObjectStatusChanged(status); };

    //this->nodeObject->fileManagerUpdated = [this](IRFileManager* fileManager) { updateFileManager(fileManager); };
    
    this->nodeObject->addListener(this);
    
    std::cout <<"==========" << nodeObject->name << " : IRUIFoundation inited\n";
    
}
// --------------------------------------------------
IRUIFoundation::~IRUIFoundation()
{
    
}
// --------------------------------------------------
void IRUIFoundation::IRKeyPressed(int keyCode, String keyText)
{
    std::cout << "IRUIFoundation IRKeyPressed = " << keyCode << std::endl;

}
// --------------------------------------------------
void IRUIFoundation::IRKeyReleased(int keyCode, String keyText)
{
    std::cout << "IRUIFoundation IRKeyReleased = " << keyCode << std::endl;

}
// --------------------------------------------------
bool IRUIFoundation::keyStateChanged(bool isKeyDown, Component* originatingComponent)
{
    
    ///std::cout << "IRUIFoundation Key state changed = " << isKeyDown << std::endl;
    
    // IRKeyPressed() should be callsed in keyPressed event
    if(!isKeyDown) IRKeyReleased(this->pressedKeyCode, this->pressedKeyText);
    
    return true;
    
}
// --------------------------------------------------

bool IRUIFoundation::keyPressed(const KeyPress &key,
                                Component* originatingComponent)
{
    // reserved key commands
    if(key.getTextDescription() == "command + E")
    {
        // inform parent the change of editMode
        this->nodeObject->setEditMode(!this->nodeObject->isEditMode());
        this->nodeObject->callEditModeChangedInNodeObject();
        
        return true;
    }
    // Link Mode
    else if(key.getTextDescription() == "command + L")
    {
        this->nodeObject->callLinkModeChangedInNodeObject();
        return true;
    }
    // save project key command
    else if(key.getTextDescription() == "command + S")
    {
        this->nodeObject->callSaveProject();
        return true;
    }
    // close project key command
    else if(key.getTextDescription() == "command + W")
    {
        
        // ignore this at this moment, as it does not work properly...
        // window does not close
        //parent->callCloseProject();
        return false; // return false at thie moment.
    }
    // open project key command
    else if(key.getTextDescription() == "command + O")
    {
        this->nodeObject->callOpenProject();
        return true;
    }
    
    else if(key.getTextDescription() == "command + 8")
    {
        this->nodeObject->callOpenFileInspecter();
        return true;
    }
    
    else if(key.getTextDescription() == "command + 9")
    {
        this->nodeObject->callOpenPreferenceWindow();
        return true;
    }
    
    // user defined key commands
    this->pressedKeyCode = key.getKeyCode();
    this->pressedKeyText = key.getTextDescription();
    IRKeyPressed(this->pressedKeyCode, this->pressedKeyText);
    //std::cout << "IRUIFoundation keyPressed = " << key.getKeyCode() << std::endl;

    return true;
}
// --------------------------------------------------

void IRUIFoundation::NodeObjectStatusChanged(IRNodeComponentStatus status)
{
    //std::cout << "NodeObjectStatusChanged : status = " << status << std::endl;
    
    switch (status)
    {
        case IRNodeComponentStatus::EditModeStatus:
            
            // control KeyEvent
            // editmode = does not receive any KeyEvent,
            // controlmode = receive keyEvent
            setEditModeBase(this->nodeObject->isEditMode());
            
            break;
            
        default:
            break;
    }
    
}
// --------------------------------------------------

void IRUIFoundation::setEditModeBase(bool newEditMode)
{
    this->editModeFlag = newEditMode;
    
    // if not editMode, get keyboardFocus
    if (! this->editModeFlag)
    {
        setWantsKeyboardFocus(true);
        addKeyListener(this);
    }
    else
    {
        // otherwise out of keyboard Focus
        setWantsKeyboardFocus(false);
        removeKeyListener(this);
    }
    // call it for child class
    setEditMode(newEditMode);
}
// --------------------------------------------------

void IRUIFoundation::updateIRFileManager(IRFileManager* fileManager)
{
    setIRFileManager(fileManager);
    
    std::cout << "IRUIFoundation : updateFileManager updated! for " << nodeObject->name << " : " << fileManager << std::endl;
}

// --------------------------------------------------

void IRUIFoundation::receiveAudioLink(IRNodeObject* obj)
{
    if(obj->getAudioLink() != nullptr)
    {
        audioPtrDelivery(obj->getAudioLink());
        //std::cout << obj << " : delivered audio file = " << obj->getAudioLink()->getFile().getFileName() << " : filename = "<<  obj->getAudioLink()->getFileName() <<std::endl;
    }
}
void IRUIFoundation::receiveTextLink(IRNodeObject* obj)
{
    if(obj->getTextLink() != nullptr) textPtrDelivery(obj->getTextLink());
}
void IRUIFoundation::receiveImageLink(IRNodeObject* obj)
{
    if(obj->getImageLink() != nullptr) imagePtrDelivery(obj->getImageLink());
}
void IRUIFoundation::receiveDataLink(IRNodeObject* obj)
{
    if(obj->getDataLink() != nullptr) dataPtrDelivery(obj->getDataLink());
}
void IRUIFoundation::receiveVideoLink(IRNodeObject* obj)
{
    if(obj->getVideoLink() != nullptr) videoPtrDelivery(obj->getVideoLink());
}
// --------------------------------------------------
