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
    obj->callUpdateIRFileManager(getFileManager());
    std::cout << " IRFileManager in WorkSpace = " << getFileManager() << std::endl;
    
    //audiosource
    if (obj->isContainAudioSource())
        this->mixer.addAudioSource(obj->getAudioSource());
    
    //request updating the workspaceList
    if(requestWorkspaceListUpdate != nullptr) requestWorkspaceListUpdate();
    
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
    for(auto obj: this->objects)
    {
        auto objType = obj->getObjectType();
        // check if this object contains any HEAVY weights
        if(objType.componentType == heavyWeightComponent)
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
    
    auto* obj = IRFactory.createObject(menu->getSelectedId(), this);
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
    setEditMode(true);
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
// ============================================================

