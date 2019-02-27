
#include "NodeObjectControlManager.hpp"




NodeObjectControlManager::NodeObjectControlManager()
{
    
}


NodeObjectControlManager::~NodeObjectControlManager()
{
    
}


void NodeObjectControlManager::setSelected(IRNodeObject *obj, bool flag)
{
    if (isSelected(obj) && !flag) // remove
        removeSelectedObj(obj);
    
    if (! isSelected(obj) && flag) // add
        this->selectedObj.push_back(obj);
}


void NodeObjectControlManager::toggleSelection(IRNodeObject* obj)
{
    setSelected(obj, ! isSelected(obj)); // set the opposite status
}


// this is called when the user clicks on the backgroun of the parent component
void NodeObjectControlManager::deSelectedAll()
{
    for (auto c: this->selectedObj)
        if (c) c->repaint();
    
    this->selectedObj.clear();
}


bool NodeObjectControlManager::isSelected(IRNodeObject* obj) const
{
    return std::find(this->selectedObj.begin(),
                     this->selectedObj.end(),
                     obj) != this->selectedObj.end();
}


void NodeObjectControlManager::removeSelectedObj(IRNodeObject* obj)
{
    this->selectedObj.erase(std::remove(this->selectedObj.begin(),
                                        this->selectedObj.end(),
                                        obj), this->selectedObj.end());
}


void NodeObjectControlManager::setConstrainBoundsToParent(bool constrainToParentSize,
                                BorderSize<int> amountPermittedOffscreen)
{
    this->constrainToParent = constrainToParentSize;
    this->amountPermittedOffscreen = amountPermittedOffscreen;
}


void NodeObjectControlManager::setShiftConstrainsDirection(bool constrainDirection)
{
    this->shiftConstrainsDirection = constrainDirection;
}


void NodeObjectControlManager::handleMouseDown(IRNodeObject* obj, const MouseEvent& e)
{
    jassert(obj != nullptr);
    
    if(! isSelected(obj))
    {
        if(! e.mods.isShiftDown() || ! e.mods.isCommandDown())
        {
            deSelectedAll();
        }
        
        setSelected(obj, true);
        didJustSelect = true;
    }
    
    this->mouseDownWithinTarget = e.getEventRelativeTo(obj).getMouseDownPosition();
    this->objBeingDragged = obj;
    this->constrainedDirection = noConstraint;
    obj->repaint();
}


void NodeObjectControlManager::handleMouseUp(IRNodeObject* obj, const MouseEvent& e)
{
    if(this->didStartDragging) this->didStartDragging = false;
    else if(! this->didJustSelect && isSelected(obj))
        setSelected(obj,false); // deselect obj
    
    this->didJustSelect = false;
    obj->repaint();
}


void NodeObjectControlManager::handleMouseDrag(const MouseEvent& e)
{
    jassert(e.mods.isAnyMouseButtonDown());
    
    if (! this->didStartDragging &&
       e.getDistanceFromDragStart() < this->minimumMovementToStartDrag)
        return;
    this->didStartDragging = true;
    
    Point<int> delta = e.getEventRelativeTo(this->objBeingDragged).getPosition() - this->mouseDownWithinTarget;
    
    if (this->constrainToParent){
        auto targetArea = getAreaOfSelectedObj() + delta;
        //get a size of the parent component
        auto limit = this->objBeingDragged->getParentComponent()->getBounds();
        
        this->amountPermittedOffscreen.subtractFrom(targetArea);
        
        if (targetArea.getX() < 0) delta.x -= targetArea.getY();
        if (targetArea.getY() < 0) delta.y -= targetArea.getY();
        if (targetArea.getBottom() > limit.getBottom()) delta.y -= targetArea.getBottom() - limit.getBottom();
        if (targetArea.getRight() > limit.getRight()) delta.x -= targetArea.getRight() - limit.getRight();
    }
    
    
}



// **** **** PRIVATE METHODS **** **** //


Rectangle<int> NodeObjectControlManager::getAreaOfSelectedObj()
{
    if(this->selectedObj.size() == 0)
        return Rectangle<int> (0,0,0,0);
    
    Rectangle<int> a = this->selectedObj[0]->getBounds();
    
    for(auto obj : this->selectedObj)
        if(obj) a = a.getUnion(obj->getBounds());
    return a;
}

void NodeObjectControlManager::applyingDirectionConstraints(const MouseEvent& e, Point<int> &delta)
{
    if (this->shiftConstrainsDirection && e.mods.isShiftDown())
    {
        /* xy > 0 == movement mainly X direction, xy < 0 == movement mainly Y direction. */
        int xy = abs(this->totalDragDelta.x + delta.x) - abs(this->totalDragDelta.y + delta.y);
        
        /* big movements remove the lock to a particular axis */
        
        if (xy > this->minimumMovementToStartDrag)
            this->constrainedDirection = xOnly;
        
        if (xy < -this->minimumMovementToStartDrag)
            this->constrainedDirection = yOnly;
        
        if ((xy > 0 && this->constrainedDirection != yOnly) ||
           (this->constrainedDirection == xOnly))
        {
            delta.y = -this->totalDragDelta.y;
            this->constrainedDirection = xOnly;
        }
        else if (xy <= 0 && this->constrainedDirection != xOnly)
        {
            delta.x = -this->totalDragDelta.x;
            this->constrainedDirection = yOnly;
        }
        else
        {
            delta = {0,0};
        }
    }
    else
    {
        this->constrainedDirection = noConstraint;
    }
}




