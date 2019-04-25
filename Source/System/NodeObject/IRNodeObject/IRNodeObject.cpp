
#include "IRNodeObject.hpp"

IRNodeObject::IRNodeObject(Component* parent, String name, NodeObjectType objectType)
: IRNodeComponent(parent, name, objectType)
{
    this->parent = parent;
}


IRNodeObject::~IRNodeObject()
{
    //notify any modification
    notifyNodeObjectModification();
    std::cout << "~IRNODEOBJECT DESTRUCTOR CALLED" << std::endl;
}


//copy constructor
IRNodeObject* IRNodeObject::copyThis()
{
    return new IRNodeObject(this->parent, "IRNodeObject", NodeObjectType());
}


// copy constructor with contents
IRNodeObject* IRNodeObject::copyContents(IRNodeObject* object)
{
    return new IRNodeObject(this->parent, "IRNodeObject", NodeObjectType());
}

IRNodeObject* IRNodeObject::copyDragDropContents(IRNodeObject* object)
{
    return new IRNodeObject(this->parent, "IRNodeObject", NodeObjectType());
}


t_json IRNodeObject::saveThisToSaveData()
{
    t_json a;
    return a;
}


// load data contents from save data
void IRNodeObject::loadThisFromSaveData(t_json saveData)
{
    
}


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

void IRNodeObject::callGetSelectedLinkSystemFlag()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.getSelectedLinkSystemFlag(this); });
    //check again
    if(checker.shouldBailOut()) return;
}

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
    
    this->listeners.callChecked(checker, [this](Listener& l){ p_obj = l.getObjectGlobal(p_id); });
    //check again
    if(checker.shouldBailOut()) return nullptr;
    //std::function
    if(this->getObjectGlobalCompleted != nullptr) this->getObjectGlobalCompleted();
    
    return this->p_obj;
}

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

void IRNodeObject::notifyNodeObjectModification()
{
    
    Component::BailOutChecker checker(this);
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.nodeObjectModifiedNotification(this); });
    if(checker.shouldBailOut()) return;
    
}


void IRNodeObject::saveObjectContents()
{
    
}


void IRNodeObject::loadObjectContents()
{
    
}

// ==================================================

void IRNodeObject::selectedChangeEvent()
{
    if(isSelected())
    {
        if(isLinkMode()) openLinkMenu();
    }
    else
    {
        if(isLinkMode()) closeLinkMenu();
    }

}
void IRNodeObject::editModeChangedEvent()
{
    
}

void IRNodeObject::linkModeChangedEvent()
{
    if(isLinkMode())
    {
        if(isSelected()) openLinkMenu();
    }else{
        if(isLinkMenuOpened()) closeLinkMenu();
    }
}

// ==================================================
// link system
void IRNodeObject::addLinkParam(IRLinkSystemFlag flag)
{
    this->linkFlags.push_back(flag);
}
// --------------------------------------------------
void IRNodeObject::revemoLinkParam(IRLinkSystemFlag flag)
{
    int index = 0;
    for(auto f : this->linkFlags)
    {
        if(f == flag)
            this->linkFlags.erase(this->linkFlags.begin() + index);
        index++;
    }
}
// --------------------------------------------------
void IRNodeObject::clearLinkParam()
{
    this->linkFlags.clear();
}
// --------------------------------------------------
void IRNodeObject::createLinkMenu()
{
    this->linkMenu = std::make_shared<IRLinkMenuObject>(this->parent, this->linkFlags);
    this->linkMenu.get()->setCentrePosition(getX() + getWidth()/2,
                                            getY() + getHeight()/2);
    this->linkMenu.get()->setSize(this->linkMenuSize * (int) this->linkFlags.size(),
                                  this->linkMenuSize);
    this->linkMenu.get()->notifySelectedItem = [this] (IRLinkSystemFlag flag) { receiveSelectedLinkMenuItem(flag); };
}
// --------------------------------------------------
void IRNodeObject::receiveSelectedLinkMenuItem(IRLinkSystemFlag flag)
{
    this->selectedLinkSystemFlag = flag;    
    callGetSelectedLinkSystemFlag();
}
// --------------------------------------------------
IRLinkMenuObject* IRNodeObject::getLinkMenu()
{
    if(this->linkMenu)
    {
        return this->linkMenu.get();
    }else{
        createLinkMenu();
        return this->linkMenu.get();
    }
}
// --------------------------------------------------
void IRNodeObject::openLinkMenu()
{
    if(this->linkMenu)
    {
        if(!this->linkMenu.get()->isShowing()){
            
            this->parent->addAndMakeVisible(this->linkMenu.get());
            this->linkMenu.get()->toFront(true);
        }
    }else{

        createLinkMenu();
        this->parent->addAndMakeVisible(this->linkMenu.get());
        this->linkMenu.get()->toFront(true);

    }
    this->linkMenuOpenedFlag = true;
}
// --------------------------------------------------
void IRNodeObject::closeLinkMenu()
{
    if(this->linkMenu)
    {
        if(this->linkMenu.get()->isShowing())
        {
            this->linkMenu.get()->deSelectAll();
            this->parent->removeChildComponent(this->linkMenu.get());
            this->linkMenuOpenedFlag = false;
        }
    }
}
// --------------------------------------------------
bool IRNodeObject::isLinkMenuOpened() const
{
    return this->linkMenuOpenedFlag;
}
// --------------------------------------------------
// ==================================================

void IRNodeObject::initialPaintOnWorkspace(Graphics& g, Component* workspace)
{
    // draw link menu
    if(isLinkMenuOpened())
    {
        this->linkMenu.get()->setCentrePosition(getX() + getWidth()/2,
                                                getY() + getHeight()/2);
    }
    paintOnWorkspace(g, workspace);
    workspace->repaint();
}
