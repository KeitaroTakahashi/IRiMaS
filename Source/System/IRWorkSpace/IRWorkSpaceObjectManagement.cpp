
#include "IRWorkSpace.hpp"





void IRWorkSpace::copySelectedObjects()
{
    std::cout << "copied\n";
    this->copiedObjects.clear();
    
    for(auto obj : this->selector->getSelectedObjectList())
    {
        copyObject(obj, false);
    }
}
// ------------------------------------------------------------

void IRWorkSpace::pasteSelectedObjects()
{
    std::cout << "paste\n";
    
    for(auto obj: this->copiedObjects)
    {
        pasteObject(obj,false);
    }
    this->selector->addSelectedObjects();
    copySelectedObjects();
}
// ------------------------------------------------------------

void IRWorkSpace::deleteSelectedObjects()
{
    std::cout << "delete selected objects\n";
    auto list = this->selector->getSelectedObjectList();
    
    for(auto obj : list)
    {
        deleteObject(obj);
    }
}
// ------------------------------------------------------------

void IRWorkSpace::duplicateSelectedObjects()
{
    copySelectedObjects();
    pasteSelectedObjects();
}


void IRWorkSpace::createObject(IRNodeObject *obj)
{
    obj->setEditMode(isEditMode());
    
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
    
    std::cout << "object created count = " << this->objects.size() << std::endl;
    
    //audiosource
    if(obj->isContainAudioSource())
        this->mixer.addAudioSource(obj->getAudioSource());
    
    //request updating the workspaceList
    if(requestWorkspaceListUpdate != nullptr) requestWorkspaceListUpdate();
}
// ------------------------------------------------------------
void IRWorkSpace::copyObject(IRNodeObject *obj, bool clearCopied)
{
    if(clearCopied) this->copiedObjects.clear();
    this->copiedObjects.add(obj);
}
void IRWorkSpace::pasteObject(IRNodeObject *obj, bool addToSelected)
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
void IRWorkSpace::duplicateObject(IRNodeObject *obj)
{
    copyObject(obj, true);
    pasteObject(obj, true);
}
void IRWorkSpace::deleteObject(IRNodeObject *obj)
{
    if(this->selector->removeSelectedObject(obj))
    {
        removeChildComponent(obj);
        
        int index = this->objects.indexOf(obj);
        if(index >= 0)
            this->objects.remove(index);
    }
    
    //request updating the workspaceList
    if(requestWorkspaceListUpdate != nullptr) requestWorkspaceListUpdate();
}

void IRWorkSpace::manageHeavyWeightComponents(bool flag)
{
    // check if any components contain HEAVY weight components
    for(auto obj: this->objects)
    {
        auto objType = obj->getObjectType();
        // check if this object contains any HEAVY weights
        if(objType.componentType == heavyWeightComponent)
        {
            if(flag){
                addAndMakeVisible(obj);
            }else{
                removeChildComponent(obj);
            }
        }
    }
}
// ------------------------------------------------------------
void IRWorkSpace::openObjectListMenu(Point<int>Pos)
{
    
    ObjectListMenu* c = this->ObjectMenuComponent;
    
    int c_w = c->getWidth();
    int c_h = c->getHeight();
    int x = Pos.getX();
    int y = Pos.getY();
    
    this->objMenuwindow.reset(new ObjectMenuWindow("Menu",Rectangle<int>(0,0,c_w,c_h), c));
    
    
    // adjust position
    if(x <= c_w){ // if the menu is around left edge of the window
        if(y <= c_h){
            this->objMenuwindow->setCentrePosition(x+c_w/2,
                                                   Pos.getY()+c_h/2);
            
        }else if(y >= (getHeight()-c_h)){
            this->objMenuwindow->setCentrePosition(x+c_w/2,
                                                   Pos.getY()-c_h/2);
        }else{
            this->objMenuwindow->setCentrePosition(x+c_w/2,y);
        }
    }else if(x >= (getWidth()-c_w)){ // if the menu is around right edge of the window
        if(y <= c_h){
            this->objMenuwindow->setCentrePosition(x-c_w/2,
                                                   Pos.getY()+c_h/2);
            
        }else if(y >= (getHeight()-c_h)){
            this->objMenuwindow->setCentrePosition(x-c_w/2,
                                                   Pos.getY()-c_h/2);
        }else{
            this->objMenuwindow->setCentrePosition(x-c_w/2,y);
        }
    }else{
        this->objMenuwindow->setCentrePosition(x,y);
    }
    
}
void IRWorkSpace::closeObjectListMenu()
{
    //removeChildComponent(this->ObjectMenuComponent);
    
    // free menu window here.
    this->objMenuwindow = nullptr;
}

void IRWorkSpace::itemSelectionAction(ObjectListMenu* menu)
{
    
}
void IRWorkSpace::itemHasSelectedAction(ObjectListMenu* menu)
{
    std::cout << "item has selected action " << menu->getSelectedIndex() << std::endl;
    std::cout << "item has selected action " << menu->getSelectedId() << std::endl;
    
    auto* obj = IRFactory.createObject(menu->getSelectedId(), this);
    obj->setCentrePosition(this->currentMousePosition.getX(),
                           this->currentMousePosition.getY());
    createObject(obj);
    removeChildComponent(this->ObjectMenuComponent);
    
    //destroy window
    this->objMenuwindow = nullptr;
    
}
// ------------------------------------------------------------
void IRWorkSpace::createObject(std::string objName)
{
    
}
// ------------------------------------------------------------
void IRWorkSpace::dragoutNodeObjectFromParent(IRNodeObject* obj)
{
    std::cout << "workspace : dragoutNodeObject\n";
    setEditMode(true);
    this->dummy.add(obj);
}
// ------------------------------------------------------------
void IRWorkSpace::dropoutNodeObjectFromParent(IRNodeObject* obj)
{
    std::cout << "workspace : dropOut\n";
}
// ------------------------------------------------------------
void IRWorkSpace::editModeChangedInNodeObject(bool editMode)
{
    //setEditMode(editMode);
    
    // DO NOT change edit mode here, but IRProject must change the status of this object

    // notify it to IRProject
    if(this->notifyEditModeChanged != nullptr)
    {
        this->notifyEditModeChanged();
    }
}
// ------------------------------------------------------------
void IRWorkSpace::saveProject()
{
    if(this->requestSaveProject != nullptr) requestSaveProject();
}
// ============================================================
void IRWorkSpace::addObjectGlobal(IRObjectPtr ptr, String id)
{
    std::cout << "addObjectGlobal\n";
    this->p_obj.insert(std::make_pair(id, ptr));
    
    std::cout << this->p_obj.at(id) << std::endl;
}
// ------------------------------------------------------------
IRObjectPtr IRWorkSpace::getObjectGlobal(String id)
{
    std::cout << "getObjectGlobal of " << id << "\n";
    
    if(this->p_obj.find(id) != this->p_obj.end()){
        std::cout << this->p_obj.at(id) << std::endl;
        return this->p_obj.at(id);
    }else{
        return nullptr;
    }
    
}


// ------------------------------------------------------------
