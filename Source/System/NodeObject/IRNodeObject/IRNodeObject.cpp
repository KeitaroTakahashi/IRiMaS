
#include "IRNodeObject.hpp"

IRNodeObject::IRNodeObject(Component* parent,
                           String name,
                           IRStr* str,
                           NodeObjectType objectType) :
IRNodeComponent(parent, name, str, objectType)
//IRNodeObjectAnimation(this)
{
    this->parent = parent;
}


IRNodeObject::~IRNodeObject()
{
    this->enclosedObject.reset();
    //std::cout << "IRNodeObject deconstrucing" << std::endl;

    //notify any modification
    notifyNodeObjectModification();
   // std::cout << "IRNodeObject deoconstructing done" << std::endl;
}
// ==================================================

//copy constructor
IRNodeObject* IRNodeObject::copyThis()
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
}

t_json IRNodeObject::getArrangeControllerSaveData()
{
    
    auto s = getStatusStr();
    
    std::string contents = "";

    // ----------
    contents += "{";
    
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
    
    contents += "\"encloseBounds\": [" + std::to_string(s->encloseBounds.getX()) +
    ", " + std::to_string(s->encloseBounds.getY()) +
    ", " + std::to_string(s->encloseBounds.getWidth()) +
    ", " + std::to_string(s->encloseBounds.getHeight()) + "], ";
    
    contents += "\"wrap\": " + std::to_string(s->wrap) + ", ";

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
    // first check arrange controller if it has
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
    this->ordinaryBounds = bounds;
}

void IRNodeObject::setEncloseBounds(Rectangle<int> bounds)
{
    this->encloseBounds = bounds;
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

    std::cout << "transformStatusToOrdinary\n";
    this->resizingSquare.applyMouseListenerToIRNodeObject();
    this->resizingSquare.removeMouseListener(this->enclosedObject.get());
    removeChildComponent(this->enclosedObject.get());
    this->enclosedObject.reset();
    setObjectBounds(this->ordinaryBounds);
}

void IRNodeObject::transformStatusEnclose()
{
    // first store data
    auto str = getStatusStr();
    str->ordinaryBounds = getBounds();
    this->ordinaryBounds = getBounds();
    
    createEnclosedObject();
    this->resizingSquare.bringThisToFront();
    
    // then move this object to the enclosed object if encloseBounds is not empty
    if(this->isEncloseObjectAlreadyDefined)
    {
        setObjectBounds(this->encloseBounds);

    }else{
        this->enclosedObject->setBounds(getLocalBounds());
        this->encloseBounds = getBounds();
        str->encloseBounds = getBounds();
        this->isEncloseObjectAlreadyDefined = true;
    }
}

void IRNodeObject::createEnclosedObject()
{
    this->enclosedObject.reset(new IREnclosedObject());
    this->enclosedObject->onClick = [this]{ enclosedObjectClickedAction(); };
    addAndMakeVisible(this->enclosedObject.get());
    this->enclosedObject->setColour(this->arrangeController->getEncloseColour());
    this->enclosedObject->addMouseListener(this, true);
}


void IRNodeObject::setEncloseColour(Colour colour)
{
    std::cout<< "setEncloseColour\n";
    auto statusStr = getStatusStr();
    statusStr->wrapColour = colour;
    
    if(this->enclosedObject.get() != nullptr)
        this->enclosedObject->setColour(colour);
}

void IRNodeObject::enclosedObjectClickedAction()
{
    std::cout << "enclosedObject clicked\n";
}

void IRNodeObject::setEncloseMode(bool flag)
{
    // data
    auto statusStr = getStatusStr();
    statusStr->wrap = flag;
    
    
    if(flag) setStatus(IRNodeObjectStatus::ENCLOSE);
    else setStatus(IRNodeObjectStatus::ORDINARY);
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
    if(this->enclosedObject.get() != nullptr)
    {
        this->enclosedObject->setBounds(getLocalBounds());
        this->encloseBounds = getBounds();
    }
}

void IRNodeObject::encloseObjectBoundsChangedAction(Rectangle<int> bounds)
{
    if(this->enclosedObject.get() != nullptr)
    {
        this->enclosedObject->setBounds(getLocalBounds());
        this->encloseBounds = getBounds();
        

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

void IRNodeObject::loadArrangeControllerSaveData(t_json arrangeCtl)
{
    auto b = arrangeCtl["bounds"].array_items();
    auto rb = arrangeCtl["relativeBounds"].array_items();
    
    // relative first
    setObjectBoundsRelative(rb[0].number_value(), rb[1].number_value(),
                            rb[2].number_value(), rb[3].number_value());
    
    // absolute second
    setObjectBounds(b[0].int_value(), b[1].int_value(),
                    b[2].int_value(), b[3].int_value());
        
    auto wrap = arrangeCtl["wrap"].int_value();
    // give wrap only when wrap is TRUE because the oridinary bounds of the enclosed mode does not have the initial bounds. It gets the bounds only when it transfers to enclose mode.
    if(wrap == 1) setEncloseMode(true);
    
    auto wrapColour = arrangeCtl["wrapColour"].array_items();
    setEncloseColour(Colour((uint8)wrapColour[0].int_value(),
                            (uint8)wrapColour[1].int_value(),
                            (uint8)wrapColour[2].int_value(),
                            (uint8)wrapColour[3].int_value()));
}

// ==================================================
//##### Animation #####
// ==================================================
/*
void IRNodeObject::StatusUpdated()
{
    
}
*/
