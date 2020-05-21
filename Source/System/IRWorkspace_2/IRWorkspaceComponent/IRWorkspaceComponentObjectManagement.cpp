//
//  IRWorkspaceComponentObjectManagement.cpp
//  IRiMaS
//
//  Created by Keitaro on 18/09/2019.
//

#include "IRWorkspaceComponent.hpp"

void IRWorkspaceComponent::copySelectedObjects()
{
    this->copiedObjects.clear();
    
    for (auto obj : this->selector->getSelectedObjectList())
    {
        copyObject(obj, false);
    }
}
// ------------------------------------------------------------

void IRWorkspaceComponent::pasteSelectedObjects()
{
    bench2.start();
    
    for (auto obj: this->copiedObjects)
    {
        pasteObject(obj,false);
    }
    this->selector->addSelectedObjects();
    copySelectedObjects();
    resortHeavyObject();
}
// ------------------------------------------------------------

void IRWorkspaceComponent::deselectAllObjects()
{
    auto list = this->selector->getSelectedObjectList();

    for(auto obj : list )
    {
        obj->setSelected(false);
    }
}

void IRWorkspaceComponent::deleteSelectedObjects()
{
    auto list = this->selector->getSelectedObjectList();
    
    for(auto obj : list)
    {
       // std::cout << "IRWorkspaceComponent : delete obj " << obj << std::endl;
        deleteObject(obj);
    }
    
    // initialize the focuses on any NodeObject
    nodeObjectGetFocused(nullptr);
    
    repaint();
}

// ------------------------------------------------------------

void IRWorkspaceComponent::duplicateSelectedObjects()
{
    copySelectedObjects();
    pasteSelectedObjects();
    
    //resortHeavyObject();
}


void IRWorkspaceComponent::createObject(IRNodeObject *obj, bool shouldSort)
{
    std::cout << "==================================================\n ";
    
    obj->setEditMode(isEditMode());
    
    obj->setResizableMargin(this->draggableMargin);
    // make uniqueID
    //KeRandomStringGenerator a;
    //std::string id = a.createStrings(10);
    // generate uniqueID
    //obj->setUniqueID("id-aa");
    addAndMakeVisible(obj);
    obj->toFront(true);
    obj->addChangeListener(this);
    obj->addListener(this); // IRNodeObjectListener
    obj->addKeyListener(this); // key listener
    this->objects.add(obj);
    
    // use this function in order to also update file manger of all related UIs etc.
    obj->updateFileManager(this->ir_str->FILEMANAGER);
    obj->callUpdateIRFileManager(&this->ir_str->FILEMANAGER);
    
    //audiosource
    if (obj->isContainAudioSource())
    {
        this->mixer.addAudioSource(obj->getAudioSource());
    }
    
    // setup openGL Context if it has. This should be called adter addAndMakeVisible
    obj->initOpenGLContext();
   
    // check if the created object is Heavy-weight Component
    if(shouldSort)
    {
        if(obj->getObjectType().componentType == IRNodeComponentType::heavyWeightComponent ||
           obj->getObjectType().componentType == IRNodeComponentType::ordinaryIRComponent)
        {
            
            std::cout << "IRWorkspaceComponent::createObject callHeavyObjectCreated\n";
            callHeavyObjectCreated(obj);
        }
    }
    //request updating the workspaceList
    if(requestWorkspaceListUpdate != nullptr) requestWorkspaceListUpdate();
    
    // register this object to the first place of ZOrder list
    insertObjectAtTopZOrder(obj);
    repaint();
}

void IRWorkspaceComponent::createParentObject(IRNodeObject* obj, bool shouldSort)
{
    std::cout << "==================================================\n ";
    
    obj->setEditMode(isEditMode());
    
    obj->setResizableMargin(this->draggableMargin);
    //obj->setEnableResizingSquare(false);
    //obj->setMovable(false, false, false);
    // make uniqueID
    //KeRandomStringGenerator a;
    //std::string id = a.createStrings(10);
    // generate uniqueID
    //obj->setUniqueID("id-aa");
    addAndMakeVisible(obj);
    obj->toFront(true);
    obj->addChangeListener(this);
    obj->addListener(this); // IRNodeObjectListener
    obj->addKeyListener(this); // key listener
    //this->objects.add(obj);
    
    // add as a Parent node object
    setParentNodeObject(obj);
    
    // use this function in order to also update file manger of all related UIs etc.
    obj->updateFileManager(this->ir_str->FILEMANAGER);
    obj->callUpdateIRFileManager(&this->ir_str->FILEMANAGER);
    
    //audiosource
    if (obj->isContainAudioSource())
    {
        this->mixer.addAudioSource(obj->getAudioSource());
    }
    
    // setup openGL Context if it has. This should be called adter addAndMakeVisible
    obj->initOpenGLContext();
   
    // check if the created object is Heavy-weight Component
    if(shouldSort)
    {
        if(obj->getObjectType().componentType == IRNodeComponentType::heavyWeightComponent ||
           obj->getObjectType().componentType == IRNodeComponentType::ordinaryIRComponent)
        {
            
            std::cout << "IRWorkspaceComponent::createObject callHeavyObjectCreated\n";
            callHeavyObjectCreated(obj);
        }
    }
    //request updating the workspaceList
    if(requestWorkspaceListUpdate != nullptr) requestWorkspaceListUpdate();
    
    // register this object to the first place of ZOrder list
    //insertObjectAtTopZOrder(obj);
    repaint();
}

void IRWorkspaceComponent::resortHeavyObject()
{
    
    std::cout << "IRWorkspaceComponent::resortHeavyObject\n";
    callHeavyObjectCreated(nullptr);

}
// ------------------------------------------------------------
void IRWorkspaceComponent::copyObject(IRNodeObject *obj, bool clearCopied)
{
    if(clearCopied) this->copiedObjects.clear();
    this->copiedObjects.add(obj);
}

void IRWorkspaceComponent::pasteObject(IRNodeObject *obj, bool addToSelected)
{
    
    bench.start();
    IRNodeObject* newObj = obj->copyThis();
    
    
    newObj->setObjectBounds(Rectangle<int>(obj->getPosition().x-20, obj->getPosition().y-20,
                            obj->getWidth(), obj->getHeight()));
   
    bench.result("copy object");
    // do not initialize heavy weight component here but later
    bench.start();
    createObject(newObj, false);
    bench.result("createObject");

    obj->setSelected(false);
    this->selector->removeSelectedObject(obj);
    newObj->setSelected(true);
    
    if(addToSelected)
    {
        this->selector->addSelectedObjects();
        copySelectedObjects();
    }
    
    callNodeObjectPasted(newObj);
}
void IRWorkspaceComponent::duplicateObject(IRNodeObject *obj)
{
    copyObject(obj, true);
    pasteObject(obj, true);
}
void IRWorkspaceComponent::deleteObject(IRNodeObject *obj)
{
    if(this->selector->removeSelectedObject(obj))
    {
        
        removeObjectFromZOrder(obj);
        removeChildComponent(obj);
        
        int index = this->objects.indexOf(obj);
        
        if (obj->isContainAudioSource())
            this->mixer.removeSource(obj->getAudioSource());
        
        if(index >= 0){
            
            callNodeObjectWillDeleted(obj);
            
            this->objects.remove(index);
            delete obj;
        }
    }
    
    //request updating the workspaceList
    if(requestWorkspaceListUpdate != nullptr) requestWorkspaceListUpdate();
}

void IRWorkspaceComponent::manageHeavyWeightComponents(bool flag)
{
    // check if any components contain HEAVY weight components
    
    // to save objects, we need to reverse the order of ObjectZorder
    // The top object is stored the begining of the vector but it needs to be at the end in order to be created at last.
    std::cout << "IRWorkspaceComponent::manageHeavyWeightComponents\n";
    std::vector<IRNodeObject*> reversedZorder = this->ObjectZorder;
    std::reverse(std::begin(reversedZorder), std::end(reversedZorder));
    
    // ##############
    //first bring cover object to front
    // second bring parent nodeObject to front
    bringParentNodeObjectToFront();
    
    bringCoverToFront();

    // these two objects should be always placed in this z-order, and then we coordinate other objects
    // ##############

    for(auto obj: reversedZorder)
    {
        if(flag)
        {
            if(obj->isActive())
            {
                // do not refresh heacy component z- order until all objects are set up
                obj->bringThisToFront(false);
            }else{
                // if inAvtivated, then the object is behind the cover object
                obj->bringThisToBack();
            }
        }
    }
    // refresh heacy component z order
    callHeavyObjectCreated(nullptr);
}
// ------------------------------------------------------------

void IRWorkspaceComponent::createCover()
{
    this->cover.reset(new IRWorkspaceCover(getStr()));
    
    addAndMakeVisible(this->cover.get());
    this->cover->addMouseListener(this, true);
    this->cover->addMouseListener(getStr()->projectOwner, false);
    this->cover->addKeyListener(this);
    
    
}

void IRWorkspaceComponent::bringCoverToFront()
{
    if(this->cover.get() != nullptr)
           this->cover->bringThisToFront();
}

void IRWorkspaceComponent::setCoverEditMode(bool editMode)
{
    if(this->cover.get() != nullptr)
    {
        this->cover->setEditMode(editMode);
    }
}


// ------------------------------------------------------------

void IRWorkspaceComponent::bringParentNodeObjectToFront()
{
    if(this->parentNodeObject != nullptr)
        this->parentNodeObject->bringThisToFront();
}

void IRWorkspaceComponent::setParentNodeObject(IRNodeObject* newParentNodeObject)
{
    this->parentNodeObject = newParentNodeObject;
    this->hasParentNodeObjectFlag = true;
    this->parentNodeObject->setEnableResizingSquare(false);
}

void IRWorkspaceComponent::removeParentNodeObject()
{
    this->parentNodeObject = nullptr;
    this->hasParentNodeObjectFlag = false;
}

// ------------------------------------------------------------
void IRWorkspaceComponent::dragoutNodeObjectFromParent(IRNodeObject* obj)
{
    std::cout << "workspace : dragoutNodeObject" << std::endl;
    
    // change edit mode and notify its change
    // because dragoutNodeObjectFromParent is called from IRNodeObject and not the up down order.
    setEditMode(true, true);
    // notify the change of editMode to IRProject
    if (this->notifyEditModeChanged != nullptr)
    {
        this->notifyEditModeChanged();
    }
    
    // create new object
    IRNodeObject* o = obj->copyThis();
    
    // create eobject first!!
    createObject(o);
    
    // copy contents here.
    o = obj->copyDragDropContents(o);
    
    // set Position
    o->setObjectCentredPosition(this->currentMousePosition.getX(),
                                this->currentMousePosition.getY());
    
    // set other parameters
    o->setSelected(true);
    this->selector->addSelectedObjects();
    o->repaint();
    
    this->dummy.add(o);
    
    //obj->addToDesktop(0);
}
// ------------------------------------------------------------
void IRWorkspaceComponent::dropoutNodeObjectFromParent(IRNodeObject* obj)
{
    std::cout << "workspace : dropOut" << std::endl;
    
}
// ------------------------------------------------------------
void IRWorkspaceComponent::editModeChangedInNodeObject(bool editMode)
{
    setEditMode(editMode);
    
    // notify it to IRProject
    if(this->notifyEditModeChanged != nullptr)
    {
        this->notifyEditModeChanged();
    }
}

// ------------------------------------------------------------
void IRWorkspaceComponent::saveProject()
{
    if(this->requestSaveProject != nullptr) requestSaveProject();
}
void IRWorkspaceComponent::saveAsProject()
{
    if(this->requestSaveAsProject != nullptr) requestSaveAsProject();
}
void IRWorkspaceComponent::closeProject()
{
    if(this->requestCloseProject != nullptr) requestCloseProject();
}
void IRWorkspaceComponent::openProject()
{
    if(this->requestOpenProject != nullptr) requestOpenProject();
}
void IRWorkspaceComponent::createNewProject()
{
    if(this->requestNewProject != nullptr) requestNewProject();
}
void IRWorkspaceComponent::openFileInspecter()
{
    if(this->requestOpenFileInspecter != nullptr) requestOpenFileInspecter();
}

// ============================================================

void IRWorkspaceComponent::addObjectGlobal(IRObjectPtr ptr, String id)
{
    std::cout << "addObjectGlobal" << std::endl;
    
    this->p_obj.insert(std::make_pair(id, ptr));
    
    std::cout << this->p_obj.at(id) << std::endl;
}
// ------------------------------------------------------------
void IRWorkspaceComponent::getObjectGlobal(IRNodeObject* obj)
{
    String id = obj->getGlobalObjectID();
    
    std::cout << "getObjectGlobal of " << id << std::endl;
    
    if(this->p_obj.find(id) != this->p_obj.end()){
        std::cout << this->p_obj.at(id) << std::endl;
        obj->setGlobalObject(this->p_obj.at(id));
    }else{
        std::cout << "could not find "<< id << std::endl;
    }
}
// ------------------------------------------------------------

void IRWorkspaceComponent::nodeObjectModifiedNotification(IRNodeObject* obj)
{
    
    if(this->notifyNodeObjectModification != nullptr)
        this->notifyNodeObjectModification(obj);

}

void IRWorkspaceComponent::nodeObjectSelectionChange(IRNodeObject* obj)
{
    std::cout << "IRWorkspaceComponent nodeObjectSelectionChange " << obj << std::endl;
    
    // add
    this->selector->addSelectedObjects();

    callNodeObjectSelectionChange(obj);
    
}

void IRWorkspaceComponent::nodeObjectGetFocused(IRNodeObject* obj)
{
    //std::cout << "IRWorkspaceComponent nodeObjectSelectionChange " << obj << std::endl;
    callNodeObjectGetFocused(obj);
    
}

void IRWorkspaceComponent::nodeObjectMoveToFront(IRNodeObject* obj)
{
    // bring obj to the top front Z order
    insertObjectAtTopZOrder(obj);
}

void IRWorkspaceComponent::nodeObjectMoveToBack(IRNodeObject* obj)
{
    // bring obj to the top front Z order
    insertObjectAtEndZOrder(obj);
    
    //re-order the objects
    manageHeavyWeightComponents(true);
}

void IRWorkspaceComponent::heavyComponentCreated(IRNodeObject* obj)
{
    callHeavyObjectCreated(obj);
}

void IRWorkspaceComponent::addHeavyCopmonentToTopZOrder(IRNodeObject* obj)
{
    insertObjectAtTopZOrder(obj);
    
}

void IRWorkspaceComponent::callNodeObjectSelectionChange(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.nodeObjectSelectionChange(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRWorkspaceComponent::callNodeObjectGetFocused(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.nodeObjectGetFocused(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRWorkspaceComponent::callEditModeChanged()
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.editModeChanged(this); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRWorkspaceComponent::callHeavyObjectCreated(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.heavyObjectCreated(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}


void IRWorkspaceComponent::callNothingSelected()
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.nothingSelected(); });
    //check again
    if(checker.shouldBailOut()) return;
}

// ============================================================

void IRWorkspaceComponent::callNodeObjectCopied(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.nodeObjectCopied(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}
void IRWorkspaceComponent::callNodeObjectPasted(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.nodeObjectPasted(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}
void IRWorkspaceComponent::callNodeObjectWillDeleted(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.nodeObjectWillDeleted(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}

// ============================================================

void IRWorkspaceComponent::insertObjectAtTopZOrder(IRNodeObject* obj)
{
    //std::cout << "insertObjectAtTopZOrder : " << obj << std::endl;
    // check if inserted obj is already registered and remove it.
    auto it = std::find(this->ObjectZorder.begin(), this->ObjectZorder.end(), obj);
    if(it != this->ObjectZorder.end())
    {
        this->ObjectZorder.erase(it);
    }
    
    it = this->ObjectZorder.begin();
    this->ObjectZorder.insert(it, obj);
    
    for(int i = 0; i < this->ObjectZorder.size(); i ++)
    {
        this->ObjectZorder[i]->sortIndex = i;
        //std::cout << this->ObjectZorder[i]->name << " : " << this->ObjectZorder[i]->sortIndex << std::endl;
    }
}

// ============================================================


void IRWorkspaceComponent::insertObjectAtEndZOrder(IRNodeObject* obj)
{
    // check if inserted obj is already registered and remove it.
    auto it = std::find(this->ObjectZorder.begin(), this->ObjectZorder.end(), obj);
    if(it != this->ObjectZorder.end())
    {
        this->ObjectZorder.erase(it);
    }
    
    it = this->ObjectZorder.end();
    this->ObjectZorder.insert(it, obj);
    
    for(int i = 0; i < this->ObjectZorder.size(); i ++)
    {
        this->ObjectZorder[i]->sortIndex = i;
        //std::cout << this->ObjectZorder[i]->name << " : " << this->ObjectZorder[i]->sortIndex << std::endl;
    }
}

void IRWorkspaceComponent::removeObjectFromZOrder(IRNodeObject* obj)
{
    
    //std::cout << "removeObjectFromZOrder : " << obj << std::endl;

    auto it = std::find(this->ObjectZorder.begin(), this->ObjectZorder.end(), obj);
    if(it != this->ObjectZorder.end())
    {
        this->ObjectZorder.erase(it);
    }
    
    for(int i = 0; i < this->ObjectZorder.size(); i ++)
       {
           this->ObjectZorder[i]->sortIndex = i;
           //std::cout << this->ObjectZorder[i]->name << " : " << this->ObjectZorder[i]->sortIndex << std::endl;
       }
}

