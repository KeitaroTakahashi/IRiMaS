
#include "IRNodeObject.hpp"

IRNodeObject::IRNodeObject(Component* parent,
                           String name,
                           IRStr* str,
                           NodeObjectType objectType) :
IRNodeComponent(parent, name, str, objectType)
{
    this->parent = parent;
    
    createEnclosedObject();
    
}


IRNodeObject::~IRNodeObject()
{
    
    //this->enclosedObject.reset();
    //notify any modification
    notifyNodeObjectModification();
}
// ==================================================


void IRNodeObject::resizedOrMoved()
{
    setBoundsGoBackToEncloseButton();
}

// ==================================================

//copy constructor
IRNodeObject* IRNodeObject::copyThis()
{
    auto obj = copyThisObject();
    obj->copyArrangeController(this);
    
    return obj;
}

IRNodeObject* IRNodeObject::copyThisObject()
{
    return new IRNodeObject(this->parent, "IRNodeObject", getStr(), NodeObjectType());
}


// copy constructor with contents
IRNodeObject* IRNodeObject::copyContents(IRNodeObject* object)
{
    return new IRNodeObject(this->parent, "IRNodeObject", getStr(), NodeObjectType());
}

IRNodeObject* IRNodeObject::copyDragDropContents(IRNodeObject* object)
{
    return new IRNodeObject(this->parent, "IRNodeObject", getStr(), NodeObjectType());
}

// ==================================================

t_json IRNodeObject::saveThisToSaveData()
{
    t_json a;
    return a;
}


// load data contents from save data
void IRNodeObject::loadThisFromSaveData(t_json saveData)
{
    
}
// ==================================================
void IRNodeObject::setObjController(IRObjectController* objCtl)
{
    this->objController = objCtl;
    
    if(this->objController->hasArrangeController())
    {
        setArrangeController(this->objController->getArrangeController());
    }
}


void IRNodeObject::setArrangeController(ArrangeController* controller)
{
    this->arrangeController = controller;
    this->arrangeController->addChangeListener(this);
    
    
    //if controlelr has arrangeController, then set wrap colour
    if(getArrangeController() != nullptr)
        getStatusStr()->wrapColour = getArrangeController()->getEncloseColour();
}

void IRNodeObject::copyArrangeController(IRNodeObject* copiedObject)
{
    auto c = copiedObject;
    if(c == nullptr) return;
    
    if(c->getArrangeController() == nullptr) return;
    
    setBoundType(c->getBoundType());
    
    if(c->getBoundType() == IRNodeComponentBoundsType::RELATIVE)
        setObjectBoundsRelative(c->getObjectBoundsRelative());
    else if(c->getBoundType() == IRNodeComponentBoundsType::ORDINARY)
        setObjectBounds(c->getBounds());
    
    if(c->getEncloseMode() == 1) setEncloseMode(c->getEncloseMode());
    
    setEncloseColour(c->getArrangeController()->getEncloseColour());
    
    setStartTimeSec(c->getStartTimeSec());
    setEndTimeSec(c->getEndTimeSec());
}


t_json IRNodeObject::getArrangeControllerSaveData()
{
    
    auto s = getStatusStr();
    
    // update relative bounds
    calcRelativeEncloseBounds();
    calcRelativeOrdinaryBounds();
    
    std::string contents = "";

    // ----------
    contents += "{";
    
    contents += "\"boundType\": " + std::to_string(s->boundType) + ", ";
    
    
    contents += "\"bounds\": [" + std::to_string(s->bounds.getX()) +
    ", " + std::to_string(s->bounds.getY()) +
    ", " + std::to_string(s->bounds.getWidth()) +
    ", " + std::to_string(s->bounds.getHeight()) + "], ";

    contents += "\"relativeBounds\": [" + std::to_string(s->relativeBounds.getX()) +
    ", " + std::to_string(s->relativeBounds.getY()) +
    ", " + std::to_string(s->relativeBounds.getWidth()) +
    ", " + std::to_string(s->relativeBounds.getHeight()) + "], ";
    
    contents += "\"ordinaryBounds\": [" + std::to_string(s->ordinaryBounds.getX()) +
    ", " + std::to_string(s->ordinaryBounds.getY()) +
    ", " + std::to_string(s->ordinaryBounds.getWidth()) +
    ", " + std::to_string(s->ordinaryBounds.getHeight()) + "], ";
    
    std::cout << "save arrangeCtl bounds " << s->relativeBounds.getX() << " ; ordinary " << s->ordinaryBounds.getX() << std::endl;
    
    contents += "\"encloseBounds\": [" + std::to_string(s->encloseBounds.getX()) +
    ", " + std::to_string(s->encloseBounds.getY()) +
    ", " + std::to_string(s->encloseBounds.getWidth()) +
    ", " + std::to_string(s->encloseBounds.getHeight()) + "], ";
    
    contents += "\"ordinaryBoundsRelative\": [" + std::to_string(s->ordinaryBoundsRelative.getX()) +
    ", " + std::to_string(s->ordinaryBoundsRelative.getY()) +
    ", " + std::to_string(s->ordinaryBoundsRelative.getWidth()) +
    ", " + std::to_string(s->ordinaryBoundsRelative.getHeight()) + "], ";
    
    std::cout <<" saving... " << s->ordinaryBoundsRelative.getWidth() << std::endl;
    
    contents += "\"encloseBoundsRelative\": [" + std::to_string(s->encloseBoundsRelative.getX()) +
    ", " + std::to_string(s->encloseBoundsRelative.getY()) +
    ", " + std::to_string(s->encloseBoundsRelative.getWidth()) +
    ", " + std::to_string(s->encloseBoundsRelative.getHeight()) + "], ";
    
    contents += "\"wrap\": " + std::to_string(s->wrap) + ", ";
    
    contents += "\"hasWrap\": " + std::to_string(s->hasEncloseObjectAlreadyDefined) + ", ";

    contents += "\"wrapColour\": [" + std::to_string(s->wrapColour.getRed()) +
    ", " + std::to_string(s->wrapColour.getGreen()) +
    ", " + std::to_string(s->wrapColour.getBlue()) +
    ", " + std::to_string(s->wrapColour.getAlpha()) + "], ";
    
    contents += "\"startTime\": " + std::to_string(s->startTimeCode) + ", ";
    contents += "\"endTime\": " + std::to_string(s->endTimeCode);
    
    contents += "}";
    // ----------
    std::string err;
    return t_json::parse(contents,err);
    
}

void IRNodeObject::arrangeControllerChangedCallback(ChangeBroadcaster* source)
{
    using t = ArrangeController::ArrangeControllerStatus;
    
    switch (this->arrangeController->getStatus()) {
        case t::INPUTWIDTH:
            setObjectSize(this->arrangeController->getLabelWidth(), getHeight());
            break;
        case t::INPUTHEIGHT:
            setObjectSize(getWidth(), this->arrangeController->getLabelHeight());
            break;
        case t::INPUTX:
            setObjectBounds(this->arrangeController->getLabelX(), getY(),
            getWidth(), getHeight());
            break;
        case t::INPUTY:
            setObjectBounds(getX(), this->arrangeController->getLabelY(),
                            getWidth(), getHeight());
            break;
        case t::FRONTBUTTON:
            bringToFront();
            break;
        case t::BACKBUTTON:
            bringToBack();
            break;
        case t::ENCLOSEBUTTON:
            setEncloseMode(this->arrangeController->getEnclosedButtonStatus());
            break;
        case t::ENCLOSECOLOUR:
            setEncloseColour(this->arrangeController->getEncloseColour());
        default:
        case t::ACTIVATEBUTTON:
            setActive(this->arrangeController->getActivateStatus());
            break;
            break;
    }
    
    arrangeControllerChangedNotify();
}
// ==================================================


void IRNodeObject::mouseDownEvent(const MouseEvent& e)
{
    
}


void IRNodeObject::mouseUpEvent(const MouseEvent& e)
{
    
}

void IRNodeObject::mouseUpCompleted(const MouseEvent& e)
{
    notifyNodeObjectModification();
}


void IRNodeObject::mouseMoveEvent(const MouseEvent& e)
{
    
}


void IRNodeObject::mouseDoubleClickEvent(const MouseEvent& e)
{
    
}

void IRNodeObject::mouseDragEvent(const MouseEvent& e)
{
    
}



/*
 
 FD: FOR THOSE THREE METHODS NEED TO CHECK SCOPE RESOLUTION BEFORE REFACTORING - LEAVING IMPLEMENTATION IN .hpp FOR NOW
 
void IRNodeObject::addListener(IRNodeObject::Listener* newListener)
{
    this->listeners.add(newListener);
}


void IRNodeObject::removeListener(IRNodeObject::Listener* listener)
{
    this->listeners.remove(listener);
}


ListenerList<IRNodeObject::Listener>& IRNodeObject::getListenerList()
{
    return this->listeners;
}
 */



// BELOW DONE BY KEITARO ALREADY

void IRNodeObject::callDragOutNodeObjectFromParent()
{
    Component::BailOutChecker checker(this);
    
    setSelected(false); // IMPORTANT
    
    
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.dragoutNodeObjectFromParent(this); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->dragoutNodeObjectCompleted != nullptr) this->dragoutNodeObjectCompleted();
}

void IRNodeObject::callDropOutNodeObjectFromParent()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.dropoutNodeObjectFromParent(this); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->dropoutNodeObjectCompleted != nullptr) this->dropoutNodeObjectCompleted();

}

void IRNodeObject::callEditModeChangedInNodeObject()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.editModeChangedInNodeObject(isEditMode()); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->editModeChangedCompleted != nullptr) this->editModeChangedCompleted();
}

void IRNodeObject::callHeavyComponentCreated(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.heavyComponentCreated(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRNodeObject::callReorderZIndex()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.reorderZIndex(); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRNodeObject::callAddHeavyComponentToTopZOrder(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.addHeavyCopmonentToTopZOrder(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}
/*
void IRNodeObject::callLinkModeChangedInNodeObject()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.linkModeChangedInNodeObject(isLinkMode()); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->linkModeChangedCompleted != nullptr) this->linkModeChangedCompleted();
}

void IRNodeObject::receiveSelectedLinkMenuItemEvent()
{
    callGetSelectedLinkSystemFlag();
}*/


/*
void IRNodeObject::callAddObjectGlobal(IRObjectPtr obj, String id)
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    
    this->p_obj = obj;
    this->p_id = id;
    
    this->listeners.callChecked(checker, [this](Listener& l){ l.addObjectGlobal(p_obj, p_id); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->addOBjectGlobalCompleted != nullptr) this->addOBjectGlobalCompleted();
}

IRObjectPtr IRNodeObject::callGetObjectGlobal(String id)
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return nullptr;
    
    this->p_id = id;
    
    std::cout << "listener" <<  this->p_obj << std::endl;
    this->listeners.callChecked(checker, [this](Listener& l){ l.getObjectGlobal(this); });
    std::cout << "listener" <<  this->p_obj << std::endl;

    //check again
    if(checker.shouldBailOut()) return nullptr;
    //std::function
    if(this->getObjectGlobalCompleted != nullptr) this->getObjectGlobalCompleted();
    
    std::cout << " got pointer " <<  this->p_obj << std::endl;
    return this->p_obj;
}*/

void IRNodeObject::callSaveProject()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.saveProject(); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->saveProjectCompleted != nullptr) this->saveProjectCompleted();

}

void IRNodeObject::callSaveAsProject()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.saveAsProject(); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->saveAsProjectCompleted != nullptr) this->saveAsProjectCompleted();
    
}

void IRNodeObject::callCloseProject()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.closeProject(); });
    //check again
    if(checker.shouldBailOut()) return;
    if(this->closeProjectCompleted != nullptr) this->closeProjectCompleted();

}

void IRNodeObject::callOpenProject()
{
    
    std::cout << "IRNodeObject : openProject" << std::endl;
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.openProject(); });
    //check again
    if(checker.shouldBailOut()) return;
    if(this->openProjectCompleted != nullptr) this->openProjectCompleted();
    
}

void IRNodeObject::callCreateNewProject()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.createNewProject(); });
    //check again
    if(checker.shouldBailOut()) return;
    if(this->createNewProjectCompleted != nullptr) this->createNewProjectCompleted();
    
}

void IRNodeObject::callOpenFileInspecter()
{
    Component::BailOutChecker checker(this);
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.openFileInspecter(); });
    if(checker.shouldBailOut()) return;
}

void IRNodeObject::callOpenPreferenceWindow()
{
    Component::BailOutChecker checker(this);
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.openPreferenceWindow(); });
    if(checker.shouldBailOut()) return;
}

void IRNodeObject::notifyNodeObjectModification()
{
    Component::BailOutChecker checker(this);
    if(checker.shouldBailOut()) return;

    this->listeners.callChecked(checker, [this](Listener& l){ l.nodeObjectModifiedNotification(this); });
    if(checker.shouldBailOut()) return;
    
}
// ==================================================
/*
void IRNodeObject::callReceiveAudioLink(IRAudio* obj)
{
    printf("callReceiveAudioLink\n");
    
    if(obj == nullptr) return; // check empty object
    
    setAudioLink(obj);
    Component::BailOutChecker checker(this);
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.receiveAudioLink(this); });
    if(checker.shouldBailOut()) return;
}


void IRNodeObject::callReceiveTextLink(IRText* obj)
{
    if(obj == nullptr) return; // check empty object

    setTextLink(obj);
    Component::BailOutChecker checker(this);
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.receiveTextLink(this); });
    if(checker.shouldBailOut()) return;
}
void IRNodeObject::callReceiveImageLink(IRImage* obj)
{
    if(obj == nullptr) return; // check empty object

    setImageLink(obj);
    Component::BailOutChecker checker(this);
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.receiveImageLink(this); });
    if(checker.shouldBailOut()) return;
}
void IRNodeObject::callReceiveDataLink(IRData* obj)
{
    if(obj == nullptr) return; // check empty object

    setDataLink(obj);
    Component::BailOutChecker checker(this);
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.receiveDataLink(this); });
    if(checker.shouldBailOut()) return;
}
void IRNodeObject::callReceiveVideoLink(IRVideo* obj)
{
    if(obj == nullptr) return; // check empty object

    setVideoLink(obj);
    Component::BailOutChecker checker(this);
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.receiveVideoLink(this); });
    if(checker.shouldBailOut()) return;
}
*/
void IRNodeObject::callUpdateIRFileManager(IRFileManager* fileManager)
{
    
    //setIRFileManager(fileManager);
    Component::BailOutChecker checker(this);
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.updateIRFileManager(getFileManager()); });
    if(checker.shouldBailOut()) return;
}
// ==================================================

void IRNodeObject::saveObjectContents()
{
    
}


void IRNodeObject::loadObjectContents()
{
    
}

// ==================================================
// called from IRNodeComponent
void IRNodeObject::selectedChangeEvent()
{
    selectedChangedAction(isSelected());
    //call callNodeObjectSelectionChange() in Workspace
    callNodeObjectSelectionChange();
}

void IRNodeObject::thisObjectGetFocused()
{
    //std::cout << "IRNodeObject::thisObjectGetFocused\n";
    // for IRWorkspace Event
    callNodeObjectGetFocused();
}
/*
void IRNodeObject::setLinkActivationEvent()
{
    if(isLinkMode())
    {
        if(isLinkActivated()) openLinkMenu();
        else if(isLinkMode()) closeLinkMenu();
    }
    //repaint();
}
*/
void IRNodeObject::editModeChangedEvent()
{
    
}
/*
void IRNodeObject::linkModeChangedEvent()
{
    if(isLinkMode())
    {
        if(isLinkActivated()) openLinkMenu();
    }else{
        if(isLinkMenuOpened()) closeLinkMenu();
    }
    
    statusChangedWrapper(IRNodeComponentStatus::LinkModeStatus);
    repaint();
}*/


// ==================================================

void IRNodeObject::initialPaintOnWorkspace(Graphics& g, Component* workspace)
{

    //this->setLinkMenuCentrePosition(getX() + getWidth()/2,
                                    //getY() + getHeight()/2);
    paintOnWorkspace(g, workspace);
    workspace->repaint();
}

// ==================================================

void IRNodeObject::callNodeObjectSelectionChange()
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.nodeObjectSelectionChange(this); });
    //check again
    if(checker.shouldBailOut()) return;
}


void IRNodeObject::callNodeObjectGetFocused()
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.nodeObjectGetFocused(this); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRNodeObject::callNodeObjectMoveToFront()
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.nodeObjectMoveToFront(this); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRNodeObject::callNodeObjectMoveToBack()
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.nodeObjectMoveToBack(this); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRNodeObject::callNodeObjectPositionChanged()
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.nodeObjectPositionChanged(this); });
    //check again
    if(checker.shouldBailOut()) return;
}
// ==================================================

void IRNodeObject::changeListenerCallback (ChangeBroadcaster* source)
{
    // first check arrange controller if it exists
    if(this->arrangeController != nullptr)
    {
        if(source == this->arrangeController)
            arrangeControllerChangedCallback(source);
    }
    
    IRChangeListenerCallback(source);
}

void IRNodeObject::IRChangeListenerCallback(ChangeBroadcaster* source)
{
    
}
// ==================================================

// ==================================================
// move to Front

void IRNodeObject::moveThisToFrontZIndex()
{
    // to inform IRWorkspace to add this object in the top of Z-Index
       callNodeObjectMoveToFront();
}

void IRNodeObject::moveToFrontEvent(bool isRefreshHeavyComponent, bool registerZindex)
{
    // move to front z index
    if(registerZindex) moveThisToFrontZIndex();
    
    // virtual method to inform IRNodeObject
    moveToFrontAction();
    
    if(isRefreshHeavyComponent)
    {
        // to inform IRWindowComponent
        getStr()->projectOwner->rebindOpenGLContents();
        // to inform IRWorksapce
        //callHeavyComponentCreated(this);
    }
    
   
}

void IRNodeObject::moveThisToBackIndex()
{
    // inform to IRWorkspace
    callNodeObjectMoveToBack();
}

void IRNodeObject::moveToBackEvent()
{
    // move to back z index
    moveThisToBackIndex();
    
    // IRWorkspace will reorder objects according to the new ZIndex.
    callReorderZIndex();
    
    //to inform IRNodeObject
    moveToBackAction();

}

// ==================================================
// ==================================================
// STATUS //

void IRNodeObject::setOrdinaryBounds(Rectangle<int> bounds)
{
    
    //std::cout << "setOrdinaryBounds\n";
    auto str = getStatusStr();

    str->ordinaryBounds = bounds;
    this->ordinaryBounds = bounds;
    
    calcRelativeOrdinaryBounds();
    
}

void IRNodeObject::setEncloseBounds(Rectangle<int> bounds)
{
    auto str = getStatusStr();

    this->encloseBounds = bounds;
    str->encloseBounds = bounds;
    
    calcRelativeEncloseBounds();
}

void IRNodeObject::adjustOrdinaryBoundsToRelative()
{
    float w = this->parent->getWidth();
    float h = this->parent->getHeight();
        
    this->ordinaryBounds = Rectangle<int> (floor(w * this->ordinaryBoundsRelative.getX()),
                                           floor(h * this->ordinaryBoundsRelative.getY()),
                                           floor(w * this->ordinaryBoundsRelative.getWidth()),
                                           floor(h * this->ordinaryBoundsRelative.getHeight())
                                           );
    //setObjectBounds(this->ordinaryBounds);
}

void IRNodeObject::adjustEncloseBoundsToRelative()
{
    float w = this->parent->getWidth();
    float h = this->parent->getHeight();
            
    this->encloseBounds = Rectangle<int> (
                                          floor(w * this->encloseBoundsRelative.getX()),
                                          floor(h * this->encloseBoundsRelative.getY()),
                                          floor(w * this->encloseBoundsRelative.getWidth()),
                                          floor(h * this->encloseBoundsRelative.getHeight())
                                          );
    
    /*
    std::cout << "this->encloseBoundsRelative.getY() = " << this->encloseBoundsRelative.getY() << std::endl;
    std::cout << "adjustEncloseBoundsToRelative : " << this->encloseBounds.getX() << ", " << this->encloseBounds.getY() << " : "<<this->encloseBounds.getWidth() << ", " << this->encloseBounds.getHeight() << std::endl;
    
    std::cout << "encloseObject : " << this->enclosedObject.getBounds().getX() << ", " << this->enclosedObject.getBounds().getY() << " : "<<this->enclosedObject.getBounds().getWidth() << ", " << this->enclosedObject.getBounds().getHeight() << std::endl;
     */
}

void IRNodeObject::setStatus(IRNodeObjectStatus newStatus)
{
    this->status = newStatus;
    
    switch(newStatus)
    {
        case ORDINARY:
            transformStatusToOrdinary();
            break;
        case ENCLOSE:
            transformStatusEnclose();
            break;
        default:
            break;
    }
}

void IRNodeObject::transformStatusToOrdinary()
{

    this->resizingSquare.applyMouseListenerToIRNodeObject();
    this->resizingSquare.removeMouseListener(&this->enclosedObject);
    
    showEncloseObject(false);
    
    adjustOrdinaryBoundsToRelative();
    setObjectBounds(this->ordinaryBounds);
}

void IRNodeObject::transformStatusEnclose()
{
    std::cout << "transformStatusEnclose : bounds " << getBounds().getWidth() << ", " << getBounds().getHeight() << " : ordinary = " << this->ordinaryBounds.getWidth() << ", " << this->ordinaryBounds.getHeight() << std::endl;
    
    showEncloseObject(true);

    // then move this object to the enclosed object if encloseBounds is not empty
    if(hasEncloseAlreadyDefined())
    {
        std::cout << "hasEnclosedAlreadyDefined\n";
        adjustEncloseBoundsToRelative();
        setObjectBounds(this->encloseBounds);
        
        this->enclosedObject.setBounds(getLocalBounds());
        auto b = this->enclosedObject.getBounds();
        std::cout << "encloseObject bounds = " << b.getX() << " , " << b.getY() << " : " << b.getWidth() << ", " << b.getHeight() << std::endl;
        
        b = this->encloseBounds;
        std::cout << "encloseBounds = " << b.getX() << " , " << b.getY() << " : " << b.getWidth() << ", " << b.getHeight() << std::endl;

        
    }else{
        std::cout << "NOE YET enclose DEFINED\n";

        // first time set
        this->encloseBounds = getBounds();
        calcRelativeEncloseBounds();
        adjustEncloseBoundsToRelative();

        setEncloseAlreadyDefined(true);
        setObjectBounds(this->encloseBounds);
        this->enclosedObject.setBounds(getLocalBounds());

        
    }
    
    this->resizingSquare.bringThisToFront();
    
}

void IRNodeObject::createEnclosedObject()
{
    this->enclosedObject.onClick = [this]{ enclosedObjectClickedAction(); };
    addAndMakeVisible(this->enclosedObject);
    this->enclosedObject.addMouseListener(this, true);
    
    this->goBackToEncloseButton.reset( new EncloseButton(getStr()) );
    this->goBackToEncloseButton->setButtonText("close");
    this->goBackToEncloseButton->setColour(TextButton::ColourIds::buttonColourId, getStr()->SYSTEMCOLOUR.fundamental);
    this->goBackToEncloseButton->onClick = [this] { goBackToEncloseButtonClicked(); };
    this->parent->addAndMakeVisible(this->goBackToEncloseButton.get());
    this->goBackToEncloseButton->setVisible(false);
    
}

void IRNodeObject::showEncloseObject(bool flag)
{
    //std::cout << "showEncloseObject " << flag << std::endl;
    //update colour
    this->enclosedObject.setColour(this->arrangeController->getEncloseColour());
    getStatusStr()->wrapColour = this->arrangeController->getEncloseColour();
    
    
    if(flag) addAndMakeVisible(this->enclosedObject);
    else removeChildComponent(&this->enclosedObject);
    
    this->enclosedObject.setVisible(flag);
    
    if(flag) this->enclosedObject.toFront(true);
    else this->enclosedObject.toBack();
    
    //std::cout << "encloseObject visible = " << this->enclosedObject.isVisible() << std::endl;
}

void IRNodeObject::calcRelativeOrdinaryBounds()
{
    float w = (float) this->parent->getWidth();
    float h = (float) this->parent->getHeight();
    
    auto b = this->ordinaryBounds;
    this->ordinaryBoundsRelative = Rectangle<float> (
                                                     (float)b.getX() / w,
                                                     (float)b.getY() / h,
                                                     (float)b.getWidth() / w,
                                                     (float)b.getHeight() / h
                                                     );
    
    
    
    
    adjustRectangleFloatToAboveZero(this->ordinaryBoundsRelative);

    auto s = getStatusStr();
    s->ordinaryBoundsRelative = this->ordinaryBoundsRelative;
    
}

void IRNodeObject::calcRelativeEncloseBounds()
{
    float w = (float) this->parent->getWidth();
    float h = (float) this->parent->getHeight();
    
    auto b = this->encloseBounds;
    this->encloseBoundsRelative = Rectangle<float> (
                                                     (float)b.getX() / w,
                                                     (float)b.getY() / h,
                                                     (float)b.getWidth() / w,
                                                     (float)b.getHeight() / h
                                                     );
    
    adjustRectangleFloatToAboveZero(this->encloseBoundsRelative);
    auto s = getStatusStr();
    s->encloseBoundsRelative = this->encloseBoundsRelative;
 
}

void IRNodeObject::adjustRectangleFloatToAboveZero(Rectangle<float>& bounds)
{
    if(bounds.getX() < 0) bounds.setX(0);
    if(bounds.getY() < 0) bounds.setY(0);
    if(bounds.getWidth() < 0) bounds.setWidth(0);
    if(bounds.getHeight() < 0) bounds.setHeight(0);
}


void IRNodeObject::setEncloseColour(Colour colour)
{
    auto statusStr = getStatusStr();
    statusStr->wrapColour = colour;
    this->enclosedObject.setColour(colour);
}

void IRNodeObject::enclosedObjectClickedAction()
{
    std::cout << "enclosedObject clicked\n";
    if(isEditMode())
    {
        
    }else{
        enclosedObjectClickedInControlMode();
    }
}

void IRNodeObject::enclosedObjectClickedInControlMode()
{
    if(getStatus() == IRNodeObjectStatus::ENCLOSE)
    {
        createGoBackToEncloseButton(true);
        setEncloseMode(false);
        getArrangeController()->setEncloseToggle(false, dontSendNotification);
        
    }else if(getStatus() == IRNodeObjectStatus::ORDINARY)
    {
        createGoBackToEncloseButton(false);

        setEncloseMode(true);
        getArrangeController()->setEncloseToggle(true, dontSendNotification);
    }
}

void IRNodeObject::goBackToEncloseButtonClicked()
{
    enclosedObjectClickedInControlMode();
}

void IRNodeObject::createGoBackToEncloseButton(bool createOrDelete)
{
    
    std::cout <<"createGoBackToEncloseButton : " << createOrDelete << std::endl;
    if(createOrDelete)
    {
        std::cout <<"createGoBackToEncloseButton visible \n";
        this->goBackToEncloseButton->setVisible(true);
        this->goBackToEncloseButton->bringThisToFront();
        setBoundsGoBackToEncloseButton();
    }else{
        this->goBackToEncloseButton->setVisible(false);
    }
    
}

void IRNodeObject::setBoundsGoBackToEncloseButton()
{
    
    if(!this->goBackToEncloseButton->isVisible()) return;
    
    std::cout << "setBoundsGoBackToEncloseButton\n";
    auto b = getBounds();
    auto p = this->parent->getBounds();
    int w = 50;
    int h = 20;
    if(b.getX() < p.getWidth()/2)
    {
        this->goBackToEncloseButton->setBounds(b.getX() + b.getWidth(),
                                               b.getY(),
                                               w, h);
    }else{
        this->goBackToEncloseButton->setBounds(b.getX() - w,
                                               b.getY(),
                                               w, h);
    }
    
    
  
}



void IRNodeObject::setEncloseMode(bool flag)
{
    
    std::cout << "setEncloseMode " << flag << std::endl;
    // data
    auto statusStr = getStatusStr();
    statusStr->wrap = flag;
    
    
    if(flag) setStatus(IRNodeObjectStatus::ENCLOSE);
    else setStatus(IRNodeObjectStatus::ORDINARY);
}

void IRNodeObject::setEncloseAlreadyDefined(bool flag)
{
    this->hasEncloseObjectAlreadyDefined = flag;
    getStatusStr()->hasEncloseObjectAlreadyDefined = flag;
}

// ==================================================

void IRNodeObject::ObjectPositionChanged4IRNodeObject(int x, int y)
{
    encloseObjectPositionChangedAction(x, y);
    arrangeControllerPositionChangedAction(x, y);
}
void IRNodeObject::ObjectBoundsChanged4IRNodeObject(Rectangle<int> bounds)
{
    
    encloseObjectBoundsChangedAction(bounds);
    arrangeControllerBoundsChangedAction(bounds);
    //IRNodeObject::Listener
    callNodeObjectPositionChanged();
}

void IRNodeObject::encloseObjectPositionChangedAction(int x, int y)
{
    if(this->status == IRNodeObjectStatus::ENCLOSE)
    {
        this->enclosedObject.setBounds(getLocalBounds());
        setEncloseBounds(getBounds());
    }else if(this->status == IRNodeObjectStatus::ORDINARY){
        setOrdinaryBounds(getBounds());
    }
}

void IRNodeObject::encloseObjectBoundsChangedAction(Rectangle<int> bounds)
{
    if(this->status == IRNodeObjectStatus::ENCLOSE)
    {
        this->enclosedObject.setBounds(getLocalBounds());
        setEncloseBounds(getBounds());
    }else if(this->status == IRNodeObjectStatus::ORDINARY){
        setOrdinaryBounds(getBounds());
    }
}
                                           
                                           
void IRNodeObject::arrangeControllerPositionChangedAction(int x, int y)
{
    // update arrangeController Labels
    if(this->arrangeController != nullptr)
    {
        this->arrangeController->setLabelX(x);
        this->arrangeController->setLabelY(y);
    }
}
                                           
void IRNodeObject::arrangeControllerBoundsChangedAction(Rectangle<int> bounds)
{
    // set bounds info without emitting notification
    if(this->arrangeController != nullptr)
        this->arrangeController->setRectangle(getBounds());
 }

void IRNodeObject::loadArrangeControllerSaveData(t_json arrangeCtl, IRNodeComponentBoundsType type)
{
    auto b = arrangeCtl["bounds"].array_items();
    auto rb = arrangeCtl["relativeBounds"].array_items();
    
    auto ob = arrangeCtl["ordinaryBounds"].array_items();
    auto obr = arrangeCtl["ordinaryBoundsRelative"].array_items();

    auto eb = arrangeCtl["encloseBounds"].array_items();
    auto ebr = arrangeCtl["encloseBoundsRelative"].array_items();
    
    // we need to determin boundtype each time of calling loadArrangeControllerSaveData.
    setBoundType(type);

    std::cout << "IRNodeObject arrangeController load , boundType = " << type << std::endl;

    // ====== WRAP ======
    //wrap colour
    auto wrapColour = arrangeCtl["wrapColour"].array_items();
    Colour wc = Colour((uint8)wrapColour[0].int_value(),
                       (uint8)wrapColour[1].int_value(),
                       (uint8)wrapColour[2].int_value(),
                       (uint8)wrapColour[3].int_value()
                       );
    
    std::cout << "wrapColour = " << wc.getRed() << ", " << wc.getFloatAlpha() << std::endl;
    setEncloseColour(wc);
    auto a = getArrangeController();
    if(a != nullptr)
    {
        a->setEncloseColour(wc);
    }


    this->ordinaryBounds = Rectangle<int>(ob[0].int_value(), ob[1].int_value(),
                                          ob[2].int_value(), ob[3].int_value());
    this->encloseBounds = Rectangle<int>(eb[0].int_value(), eb[1].int_value(),
                                         eb[2].int_value(), eb[3].int_value());
    
    std::cout << "load arrangeCtl bounds enclose " << this->encloseBounds.getX() << " ; ordinary " << this->ordinaryBounds.getX() << std::endl;
    
    this->ordinaryBoundsRelative = Rectangle<float>(obr[0].number_value(), obr[1].number_value(),
                                                    obr[2].number_value(), obr[3].number_value());
    
    this->encloseBoundsRelative = Rectangle<float>(ebr[0].number_value(), ebr[1].number_value(),
                                                   ebr[2].number_value(), ebr[3].number_value());
    
    std::cout << "load arrangeCtl bounds relative " << this->encloseBoundsRelative.getX() << " ; ordinary " << this->ordinaryBoundsRelative.getX() << std::endl;

    int wrap = arrangeCtl["wrap"].int_value();
    int hasWrap = arrangeCtl["hasWrap"].int_value();
    setEncloseAlreadyDefined((bool)hasWrap);
    
    // give wrap only when wrap is TRUE because the oridinary bounds of the enclosed mode does not have the initial bounds. It gets the bounds only when it transfers to enclose mode.
    
    if(wrap == 1)
    {
        
        // adjust bounds based in the relative ratio to the parent size
        //adjustEncloseBoundsToRelative();
        getArrangeController()->setEncloseToggle(true, sendNotification);
        setEncloseMode((bool)wrap);
            
        // adjust ordinary bounds in accordance with the current parent component bounds
        //adjustOrdinaryBoundsToRelative();

    }else{
        // adjust enclose bounds in accordance with the current parent component bounds
        //adjustEncloseBoundsToRelative();
    }
    
    // ========== BOUNDS ===========
    // AFTER ALL SETUP, SET BOUNDS FINALLY!
    // set bounds and give appripriate bounds from ordinary, enclose, relative etc...
    setObjectBoundsRelative(rb[0].number_value(), rb[1].number_value(),
                            rb[2].number_value(), rb[3].number_value());
    setObjectBounds(b[0].int_value(), b[1].int_value(),
                    b[2].int_value(), b[3].int_value());
    
    

    // ====== TimeCode ======
    float startTime = arrangeCtl["startTime"].number_value();
    float endTime = arrangeCtl["endTime"].number_value();
    
    setStartTimeSec(startTime);
    setEndTimeSec(endTime);
    
    std::cout << "loadArranceControllerSaveData completed\n";

}

// ==================================================
//##### Animation #####
// ==================================================
/*
void IRNodeObject::StatusUpdated()
{
    
}
*/
