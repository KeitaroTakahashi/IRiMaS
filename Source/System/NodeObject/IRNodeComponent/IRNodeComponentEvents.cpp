
#include "IRNodeComponent.hpp"




void IRNodeComponent::mouseDownNodeEvent(const MouseEvent& e)
{
    //fire message
    sendChangeMessage();
    
    //std::cout << this->name << " : mouseDown\n";
    if (e.mods.isCtrlDown())
    {
        if(isEditMode())
        {
            // show popup menu
            defaultPopupMenuEvents();
        }

    }
    else
    {
        //check if a mouse position is the area of resize trigger
        if (e.getMouseDownX() > (getWidth() - this->resizingArea.x) &&
           e.getMouseDownY() > (getHeight() - this->resizingArea.y) &&
           this->isResizable() && this->isEditMode())
        {
            // Resize this Node Object
            this->isMovableFlag = false;
            this->resizingFlag = true;
            // initialize previous width and height
            this->previousWidth = getWidth();
            this->previousHeight = getHeight();
            
            sendChangeMessage();
            
        }
        else if (this->isMovable())
        {
            // Move this Node Object
            //prepares our dragger to drag this component
            this->dragger.startDraggingComponent(this, e);
            this->movingFlag = true;
        }
    }
    //toFront(true);
}


void IRNodeComponent::mouseMoveNodeEvent(const MouseEvent& e)
{
    
    //std::cout << "mouse moving : position = " << e.getPosition().x << ", " << e.getPosition().y << std::endl;
    //std::cout << "mouse moving : down position = " << e.getMouseDownX() << ", " << e.getMouseDownY() << std::endl;
}


void IRNodeComponent::mouseDoubleClickNodeEvent(const MouseEvent& e)
{
    
}


void IRNodeComponent::mouseDragNodeEvent(const MouseEvent& e)
{
    if (this->isMovable())
    {
        moveThisComponentEvent(e);
        
    }
    else if (this->isResizable())
    {
        resizeThisComponentEvent(e);
    }
}


void IRNodeComponent::mouseUpNodeEvent(const MouseEvent& e)
{
    recoverEventStatus();
    
}


void IRNodeComponent::moveThisComponentEvent(const MouseEvent& e)
{
    if (isSelected())
        this->dragger.dragComponent(this, e, &constrainer);
}


void IRNodeComponent::resizeThisComponentEvent(const MouseEvent& e)
{
    float deltaX = e.getScreenX() - e.getMouseDownScreenX();
    float deltaY = e.getScreenY() - e.getMouseDownScreenY();

    float newWidth = this->previousWidth + deltaX;
    float newHeight = this->previousHeight + deltaY;

    setSize(newWidth, newHeight);
    
}


void IRNodeComponent::recoverEventStatus()
{
    
    if(this->isResizing())
    {
        this->resizingFlag = false;
        this->isMovableFlag = true; // recover movableFlag
    }
    if(this->isMoving()) this->movingFlag = false;
}


void IRNodeComponent::copyThisComponentEvent(const MouseEvent& e)
{
    if(this->isCopiable())
    {
        
    }
}


void IRNodeComponent::pasteThisComponentEvent(const MouseEvent& e)
{
  
}


void IRNodeComponent::deleteThisComponentEvent(const MouseEvent& e)
{
    
}


void IRNodeComponent::setSelected(bool flag)
{
    this->selectedFlag = flag;
    statusChangedCallback(IRNodeComponentStatus::SelectableStatus); // call back
    
    selectedChangeEvent();
}


void IRNodeComponent::defaultPopupMenuEvents()
{
    int result = this->menu.show();
    
    // 999 is reserved for the object name
    switch (result)
    {
        case 1:
            toFront(true);
            break;
        case 2:
            toBack();
            break;
        case 3:
            
            break;
            
        default:
            break;
    }
    
    // user defined events
    popupMenuEvents();
}





