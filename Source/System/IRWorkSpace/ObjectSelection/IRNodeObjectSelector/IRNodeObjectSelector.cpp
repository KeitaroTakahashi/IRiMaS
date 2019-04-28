
#include "IRNodeObjectSelector.hpp"

IRNodeObjectSelector::IRNodeObjectSelector(Array<IRNodeObject* > *list, bool* linkModeFlag)
{
    this->objectList = list;
    this->linkModeFlag = linkModeFlag;
}


IRNodeObjectSelector::~IRNodeObjectSelector()
{
    
}


void IRNodeObjectSelector::mouseDownHandler(const MouseEvent& e)
{
    IRNodeObject* nodeObj = dynamic_cast<IRNodeObject* >(e.originalComponent);
    if (nodeObj != nullptr)
    {
        
        //std::cout << "nodeOBj size = "<< nodeObj->getWidth() << "; " << nodeObj->getHeight() << std::endl;
        if (e.mods.isShiftDown() || e.mods.isCommandDown())
        {
            nodeObj->setSelected(! nodeObj->isSelected());
            
        }
        else if (! nodeObj->isSelected()){ // if the object is not yet selected.
            
            // if not linkMode, then clear all objects otherwise, add newly selected objects
            deselectAllObjects(); // CLEAR
            
            nodeObj->setSelected(true);
        }
        
        // linkMode behavior
        if(*this->linkModeFlag){
            if(nodeObj->isLinkActivated()) nodeObj->setLinkActivation(false);
            else nodeObj->setLinkActivation(true);
        }
        
        std::cout << "*this->linkModeFlag = " << *this->linkModeFlag << " : " << nodeObj->isLinkActivated() << std::endl;
        
        //repaint obj graphics
        nodeObj->repaint();
        
        //add all selected nodeObj to a list
        addSelectedObjects();
        addActivatedObjects();
        
        //if this object is selected
        if (nodeObj->isSelected())
        {
            setDragging(true);
            // set this object to be dragged.
            this->setBeingDraggedObject(nodeObj);
            this->mouseDownWithinTarget = e.getEventRelativeTo(nodeObj).getMouseDownPosition();//e.originalComponent->getPosition();//
            this->totalDragDelta = {0,0};
            this->constrainedDirection = noConstraint;
        }
    }
    else
    {
        std::cout << "workSpace mouseDowned "<< "*this->linkModeFlag = " << *this->linkModeFlag << " : " << std::endl;

        if (! e.mods.isShiftDown() && ! e.mods.isCommandDown())
        {
            //if background clicked
            deselectAllObjects();
        }
        // linkMode behavior when background clicked
        //deactivateAllLinkingObjects();

        beginSelection(e);
        this->multiSelectionFlag = true;
    }
}


void IRNodeObjectSelector::mouseDragHandler(const MouseEvent& e)
{
    if (this->multiSelectionFlag)
    {
        dragSelection(e);
    }
    else
    {
        if (! isDragging()) return;
        
        Point<int> delta = e.getEventRelativeTo(getBeingDraggedObject()).getPosition() - this->mouseDownWithinTarget;

        for (auto comp: this->selectedObjectList)
        {
            if (comp != nullptr)
            {
                if (comp != getBeingDraggedObject())
                {
                    Rectangle<int> bounds (comp->getBounds());
                    bounds += delta;
                    comp->setBounds(bounds);
                }
            }
        }
        
        this->totalDragDelta += delta;
    }
    

}


void IRNodeObjectSelector::mouseUpHandler(const MouseEvent& e)
{
    // finish dragging action
    if (isDragging()) setDragging(false);
    
    if (this->multiSelectionFlag)
    {
        endSelection(e);
        this->multiSelectionFlag = false;
    }
    repaintAllSelectedObjects();
}


void IRNodeObjectSelector::deselectAllObjects()
{
    this->selectedObjectList.clear();
    for (auto obj : *this->objectList)
    {
        if(obj->isSelected()) obj->setSelected(false);
        obj->repaint();
    }
}

void IRNodeObjectSelector::deactivateAllLinkingObjects()
{
    this->activatedLinkingObjectList.clear();
    for (auto obj : *this->objectList)
    {
        obj->setLinkActivation(false);
    }
}

void IRNodeObjectSelector::addSelectedObjects()
{
    this->selectedObjectList.clear();
    for (auto obj : *this->objectList)
    {
        if (obj->isSelected())
        {
            this->selectedObjectList.add(obj);
        }
    }
}

void IRNodeObjectSelector::addActivatedObjects()
{
    this->activatedLinkingObjectList.clear();
    for (auto obj : *this->objectList)
    {
        if (obj->isLinkActivated())
        {
            this->activatedLinkingObjectList.add(obj);
        }
    }
}

bool IRNodeObjectSelector::removeSelectedObject(IRNodeObject* removeObj)
{
    int index = this->selectedObjectList.indexOf(removeObj);
    if (index >= 0)
    {
        this->selectedObjectList.remove(index);
        return true;
    }
    return false;
}

bool IRNodeObjectSelector::removeActivatedObject(IRNodeObject* removeObj)
{
    int index = this->activatedLinkingObjectList.indexOf(removeObj);
    if (index >= 0)
    {
        this->activatedLinkingObjectList.remove(index);
        return true;
    }
    return false;
}
void IRNodeObjectSelector::repaintAllSelectedObjects()
{
    for(auto obj : this->selectedObjectList)
    {
        obj->repaint();
    }
    
    for(auto obj : this->activatedLinkingObjectList)
    {
        obj->repaint();
    }
}


void IRNodeObjectSelector::judgeSelection(const Rectangle<int>& area, const MouseEvent& e)
{
    for (auto item : *this->objectList)
    {
        IRNodeObject* obj = static_cast<IRNodeObject*>(item);
        
        if (area.intersects(obj->getBounds()))
        {
            if(e.mods.isShiftDown() || e.mods.isCommandDown())
            {
                obj->setSelected(! obj->isSelected());
            }
            else
            {
                obj->setSelected(true);
            }
        }
    }
    
    addSelectedObjects();
}


Array<IRNodeObject*> IRNodeObjectSelector::getSelectedObjectList() const
{
    return this->selectedObjectList;
}

Array<IRNodeObject*> IRNodeObjectSelector::getActivatedLinkingObjectList() const
{
    return this->activatedLinkingObjectList;
}


// **** **** PRIVATE METHODS **** **** //

Rectangle<int> IRNodeObjectSelector::getAreaOfSelectedObj()
{
    if (this->selectedObjectList.size() == 0)
        return Rectangle<int> (0,0,0,0);
    
    Rectangle<int> a = this->selectedObjectList[0]->getBounds();
    
    for (auto obj : this->selectedObjectList)
        if (obj) a = a.getUnion(obj->getBounds());
    
    return a;
}
