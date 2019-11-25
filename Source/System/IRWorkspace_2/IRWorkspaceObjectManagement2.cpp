//
//  IRWorkspaceObjectManagement.cpp
//  IRiMaS
//
//  Created by Keitaro on 18/09/2019.
//

#include "IRWorkspace2.hpp"

void IRWorkspace::copySelectedObjects()
{
    std::cout << "copied\n";
    this->copiedObjects.clear();
    
    for (auto obj : this->selector->getSelectedObjectList())
    {
        copyObject(obj, false);
    }
}
// ------------------------------------------------------------

void IRWorkspace::pasteSelectedObjects()
{
    std::cout << "paste\n";
    
    for (auto obj: this->copiedObjects)
    {
        pasteObject(obj,false);
    }
    this->selector->addSelectedObjects();
    copySelectedObjects();
}
// ------------------------------------------------------------

void IRWorkspace::deleteSelectedObjects()
{
    std::cout << "delete selected objects\n";
    auto list = this->selector->getSelectedObjectList();
    
    for(auto obj : list)
    {
        deleteObject(obj);
    }
    
    // initialize the focuses on any NodeObject
    nodeObjectGetFocused(nullptr);
    
    repaint();
}
// ------------------------------------------------------------

void IRWorkspace::duplicateSelectedObjects()
{
    copySelectedObjects();
    pasteSelectedObjects();
}


void IRWorkspace::createObject(IRNodeObject *obj)
{
    std::cout << "==================================================\n ";
    std::cout << "creating " << obj->getName() << std::endl;
    
    obj->setEditMode(isEditMode());
    obj->setLinkMode(isLinkMode());
    
    // make uniqueID
    KeRandomStringGenerator a;
    std::string id = a.createStrings(10);
    // generate uniqueID
    obj->setUniqueID(id);
    addAndMakeVisible(obj);
    
    //obj->addToDesktop(ComponentPeer::windowIsTemporary);
    obj->addChangeListener(this);
    obj->addListener(this); // IRNodeObjectListener
    obj->addKeyListener(this); // key listener
    obj->setPreferenceWindow(this->preferenceWindow); // set preference window
    this->objects.add(obj);
    
    // use this function in order to also update file manger of all related UIs etc.
    obj->updateFileManager(this->ir_str->FILEMANAGER);
    obj->callUpdateIRFileManager(&this->ir_str->FILEMANAGER);
    
    //audiosource
    if (obj->isContainAudioSource()){
        this->mixer.addAudioSource(obj->getAudioSource());
    }
    
    // setup openGL Context if it has. This should be called adter addAndMakeVisible
    obj->initOpenGLContext();
    std::cout << "initOpenGLContext done\n" << std::endl;
    
    // check if the created object is Heavy-weight Component
    if(obj->getObjectType().componentType == IRNodeComponentType::heavyWeightComponent ||
       obj->getObjectType().componentType == IRNodeComponentType::orginaryIRComponent)
    {
        callHeavyObjectCreated(obj);
    }
    
    //request updating the workspaceList
    if(requestWorkspaceListUpdate != nullptr) requestWorkspaceListUpdate();
    
    // register this object to the first place of ZOrder list
    insertObjectAtTopZOrder(obj);
    
    std::cout << "createObject done\n" << std::endl;
    
   

    repaint();
}
// ------------------------------------------------------------
void IRWorkspace::copyObject(IRNodeObject *obj, bool clearCopied)
{
    if(clearCopied) this->copiedObjects.clear();
    this->copiedObjects.add(obj);
}

void IRWorkspace::pasteObject(IRNodeObject *obj, bool addToSelected)
{
    IRNodeObject* newObj = obj->copyThis();
    newObj->setBounds(obj->getPosition().x-20, obj->getPosition().y-20,
                      obj->getWidth(), obj->getHeight());
    createObject(newObj);
    
    obj->setSelected(false);
    this->selector->removeSelectedObject(obj);
    newObj->setSelected(true);
    
    if(addToSelected)
    {
        this->selector->addSelectedObjects();
        copySelectedObjects();
    }
}
void IRWorkspace::duplicateObject(IRNodeObject *obj)
{
    copyObject(obj, true);
    pasteObject(obj, true);
}
void IRWorkspace::deleteObject(IRNodeObject *obj)
{
    if(this->selector->removeSelectedObject(obj))
    {
        
        removeObjectFromZOrder(obj);
        removeChildComponent(obj);
        
        int index = this->objects.indexOf(obj);
        
        // DUFEU ADDON
        if (obj->isContainAudioSource())
            this->mixer.removeSource(obj->getAudioSource());
        // END DUFEU ADDON
        
        if(index >= 0){
            this->objects.remove(index);
            delete obj;
        }
    }
    
    //request updating the workspaceList
    if(requestWorkspaceListUpdate != nullptr) requestWorkspaceListUpdate();
}

void IRWorkspace::manageHeavyWeightComponents(bool flag)
{
    // check if any components contain HEAVY weight components
    
    // to save objects, we need to reverse the order of ObjectZorder
    // The top object is stored the begining of the vector but it needs to be at the end in order to be created at last.
    std::vector<IRNodeObject*> reversedZorder = this->ObjectZorder;
    std::reverse(std::begin(reversedZorder), std::end(reversedZorder));
    
    
    for(auto obj: reversedZorder)
    {
        auto objType = obj->getObjectType();
        // check if this object contains any HEAVY weights
        if(objType.componentType == heavyWeightComponent || objType.componentType == orginaryIRComponent)
        {
            if(flag){
                addAndMakeVisible(obj);
                obj->heavyComponentRefreshed();
            }else{
                removeChildComponent(obj);
            }
        }
    }
}
// ------------------------------------------------------------
void IRWorkspace::openObjectListMenu(Point<int>Pos)
{
    
    ObjectListMenu* c = this->objectMenuComponent;
    
    int c_w = c->getWidth();
    int c_h = c->getHeight();
    int x = Pos.getX();
    int y = Pos.getY();
    
    this->objMenuwindow.reset(new ObjectMenuWindow("Menu", Rectangle<int>(0, 0, c_w, c_h), c));
    
    
    // adjust position
    if (x <= c_w)
    { // if the menu is around left edge of the window
        if (y <= c_h)
        {
            this->objMenuwindow->setCentrePosition(x + c_w / 2, Pos.getY() + c_h / 2);
        }
        else if (y >= (getHeight()-c_h))
        {
            this->objMenuwindow->setCentrePosition(x + c_w / 2, Pos.getY() - c_h / 2);
        }
        else
        {
            this->objMenuwindow->setCentrePosition(x + c_w / 2, y);
        }
    }
    else if (x >= (getWidth() - c_w)) // if the menu is around right edge of the window
    {
        if (y <= c_h)
        {
            this->objMenuwindow->setCentrePosition(x - c_w / 2, Pos.getY() + c_h / 2);
            
        }
        else if (y >= (getHeight()-c_h))
        {
            this->objMenuwindow->setCentrePosition(x- c_w / 2, Pos.getY() - c_h / 2);
        }
        else
        {
            this->objMenuwindow->setCentrePosition(x - c_w / 2, y);
        }
    }
    else
    {
        this->objMenuwindow->setCentrePosition(x, y);
    }
    
}
void IRWorkspace::closeObjectListMenu()
{
    //removeChildComponent(this->objectMenuComponent);
    
    // free menu window here.
    this->objMenuwindow = nullptr;
}

void IRWorkspace::itemSelectionAction(ObjectListMenu* menu)
{
    
}
void IRWorkspace::itemHasSelectedAction(ObjectListMenu* menu)
{
    std::cout << "item has selected action " << menu->getSelectedIndex() << std::endl;
    std::cout << "item has selected action " << menu->getSelectedId() << std::endl;
    
    auto* obj = IRFactory.createObject(menu->getSelectedId(), this, nullptr);
    obj->setCentrePosition(this->currentMousePosition.getX(),
                           this->currentMousePosition.getY());
    createObject(obj);
    removeChildComponent(this->objectMenuComponent);
    
    //destroy window
    this->objMenuwindow = nullptr;
    
}
// ------------------------------------------------------------
/*
 void IRWorkspace::createObject(std::string objName)
 {
 
 }
 */
// ------------------------------------------------------------
void IRWorkspace::dragoutNodeObjectFromParent(IRNodeObject* obj)
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
    o->setCentrePosition(this->currentMousePosition.getX(),
                         this->currentMousePosition.getY());
    
    // set other parameters
    o->setSelected(true);
    this->selector->addSelectedObjects();
    o->repaint();
    
    this->dummy.add(o);
    
    //obj->addToDesktop(0);
}
// ------------------------------------------------------------
void IRWorkspace::dropoutNodeObjectFromParent(IRNodeObject* obj)
{
    std::cout << "workspace : dropOut" << std::endl;
    
}
// ------------------------------------------------------------
void IRWorkspace::editModeChangedInNodeObject(bool editMode)
{
    setEditMode(editMode);
    
    // notify it to IRProject
    if(this->notifyEditModeChanged != nullptr)
    {
        this->notifyEditModeChanged();
    }
}

void IRWorkspace::linkModeChangedInNodeObject(bool linkMode)
{
    setLinkMode(linkMode);
    // notify it to IRProject
    if(this->notifyLinkModeChanged != nullptr)
    {
        this->notifyLinkModeChanged();
    }
}
// ------------------------------------------------------------
void IRWorkspace::saveProject()
{
    if(this->requestSaveProject != nullptr) requestSaveProject();
}
void IRWorkspace::saveAsProject()
{
    if(this->requestSaveAsProject != nullptr) requestSaveAsProject();
}
void IRWorkspace::closeProject()
{
    if(this->requestCloseProject != nullptr) requestCloseProject();
}
void IRWorkspace::openProject()
{
    if(this->requestOpenProject != nullptr) requestOpenProject();
}
void IRWorkspace::createNewProject()
{
    if(this->requestNewProject != nullptr) requestNewProject();
}
void IRWorkspace::openFileInspecter()
{
    if(this->requestOpenFileInspecter != nullptr) requestOpenFileInspecter();
}
void IRWorkspace::openPreferenceWindow()
{
    if(this->requestOpenPreferenceWindow != nullptr) requestOpenPreferenceWindow();
}
// ============================================================
void IRWorkspace::addObjectGlobal(IRObjectPtr ptr, String id)
{
    std::cout << "addObjectGlobal" << std::endl;
    
    this->p_obj.insert(std::make_pair(id, ptr));
    
    std::cout << this->p_obj.at(id) << std::endl;
}
// ------------------------------------------------------------
void IRWorkspace::getObjectGlobal(IRNodeObject* obj)
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

void IRWorkspace::nodeObjectModifiedNotification(IRNodeObject* obj)
{
    
    if(this->notifyNodeObjectModification != nullptr)
        this->notifyNodeObjectModification(obj);

}

void IRWorkspace::nodeObjectSelectionChange(IRNodeObject* obj)
{
    //std::cout << "IRWorkspace nodeObjectSelectionChange " << obj << std::endl;
    callNodeObjectSelectionChange(obj);
    
}

void IRWorkspace::nodeObjectGetFocused(IRNodeObject* obj)
{
    //std::cout << "IRWorkspace nodeObjectSelectionChange " << obj << std::endl;
    callNodeObjectGetFocused(obj);
    
}

void IRWorkspace::nodeObjectMoveToFront(IRNodeObject* obj)
{
    // bring obj to the top front Z order
    insertObjectAtTopZOrder(obj);
}

void IRWorkspace::heavyComponentCreated(IRNodeObject* obj)
{
    callHeavyObjectCreated(obj);
}

void IRWorkspace::addHeavyCopmonentToTopZOrder(IRNodeObject* obj)
{
    insertObjectAtTopZOrder(obj);
    
}

void IRWorkspace::callNodeObjectSelectionChange(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.nodeObjectSelectionChange(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRWorkspace::callNodeObjectGetFocused(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.nodeObjectGetFocused(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRWorkspace::callEditModeChanged()
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.editModeChanged(this); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRWorkspace::callHeavyObjectCreated(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.heavyObjectCreated(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}


// ============================================================

void IRWorkspace::insertObjectAtTopZOrder(IRNodeObject* obj)
{
    std::cout << "insertObjectAtTopZOrder : " << obj << std::endl;
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
        std::cout << this->ObjectZorder[i]->name << " : " << this->ObjectZorder[i]->sortIndex << std::endl;
    }
}

void IRWorkspace::removeObjectFromZOrder(IRNodeObject* obj)
{
    
    std::cout << "removeObjectFromZOrder : " << obj << std::endl;

    auto it = std::find(this->ObjectZorder.begin(), this->ObjectZorder.end(), obj);
    if(it != this->ObjectZorder.end())
    {
        this->ObjectZorder.erase(it);
    }
    
    for(int i = 0; i < this->ObjectZorder.size(); i ++)
       {
           this->ObjectZorder[i]->sortIndex = i;
           std::cout << this->ObjectZorder[i]->name << " : " << this->ObjectZorder[i]->sortIndex << std::endl;
       }
}

