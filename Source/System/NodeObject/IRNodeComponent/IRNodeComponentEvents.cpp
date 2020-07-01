
#include "IRNodeComponent.hpp"


void IRNodeComponent::mouseDownNodeEvent(const MouseEvent& e)
{
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
            
            std::cout << "resize\n";

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
           // this->dragger.startDraggingComponent(this, e);
            
            
            this->movingFlag = true;
        }
    }
    
    // call virtual function
    thisObjectGetFocused();
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
    if(isEditMode())
    {
    
        if (this->isMovable())
        {            
            moveThisComponentEvent(e);
            
        } else if (this->isResizable())
        {
            resizeThisComponentEvent(e);
        }
    }
}


void IRNodeComponent::mouseUpNodeEvent(const MouseEvent& e)
{
    recoverEventStatus();
    


}


void IRNodeComponent::moveThisComponentEvent(const MouseEvent& e)
{
    if (isSelected()){
       // this->dragger.dragComponent(this, e, &constrainer);
        
        
        // also move resizingSquare if visible
        if(this->resizingSquare.isVisible())
        {
            updateResizingSquare();
            //std::cout << "resizing square wh = " << this->resizingSquare.getWidth() << ", " << this->resizingSquare.getHeight() << std::endl;

        }
        
        
        // call event
        ObjectPositionChanged(getBounds().getX(), getBounds().getY());
        ObjectPositionChanged4IRNodeObject(getBounds().getX(), getBounds().getY());
        

    }
}

// ==================================================
void IRNodeComponent::resizeThisComponentEvent(const MouseEvent& e)
{
    using t = IRResizeSquare2::MovableDirection;
    switch(this->resizeDirection)
    {
        case t::TopLeft:
            resizeTopLeftComponentEvent(e);
            break;
        case t::TopRight:
            resizeTopRightComponentEvent(e);
            break;
        case t::Top:
            resizeTopComponentEvent(e);
            break;
        case t::Left:
            resizeLeftComponentEvent(e);
            break;
        case t::Right:
            resizeRightComponentEvent(e);
            break;
        case t::Bottom:
            resizeBottomComponentEvent(e);
            break;
        case t::BottomLeft:
            resizeBottomLeftComponentEvent(e);
            break;
        case t::BottomRight:
            resizeBottomRightComponentEvent(e);
            break;
        default:
            break;
    }
}

void IRNodeComponent::setResizableMargin(Rectangle<int> margin)
{
    this->resizableMargin = Rectangle<int>(margin.getX(),
                                         margin.getY(),
                                         margin.getWidth(),
                                         margin.getHeight());
}

void IRNodeComponent::adjustSizeToResizableArea(Rectangle<int>& bounds, const MouseEvent& e)
{
    

    
}

void IRNodeComponent::adjustCurrentXYToResizableArea(int& currentX, int& currentY, const MouseEvent& e)
{
    currentX = e.getScreenX();
    if(currentX <= (this->parent->getScreenX() + this->resizableMargin.getX()))
        currentX = (this->parent->getScreenX() + this->resizableMargin.getX());
    else if(currentX > (this->parent->getScreenX() + this->parent->getWidth()) + this->resizableMargin.getWidth())
        currentX = (this->parent->getScreenX() + this->parent->getWidth()) + this->resizableMargin.getWidth();
    
    currentY = e.getScreenY();
    if(currentY < (this->parent->getScreenY() + this->resizableMargin.getY())) currentY = (this->parent->getScreenY() + this->resizableMargin.getY());
    else if(currentY >= (this->parent->getScreenY() + this->parent->getHeight() + this->resizableMargin.getHeight()))
        currentY = (this->parent->getScreenY() + this->parent->getHeight() + this->resizableMargin.getHeight());
}


void IRNodeComponent::resizeTopLeftComponentEvent(const MouseEvent& e)
{
    int currentX = 0;
    int currentY = 0;
    adjustCurrentXYToResizableArea(currentX, currentY, e);
    
    float deltaX = currentX - e.getMouseDownScreenX();
    float deltaY = currentY - e.getMouseDownScreenY();

    float newWidth = this->previousWidth - deltaX;
    float newHeight = this->previousHeight - deltaY;
    float newX = this->previousX + deltaX;
    float newY = this->previousY + deltaY;
    
    if(newWidth < this->minWidth)
    {
        newX = this->previousX + (this->previousWidth - this->minWidth);
        newWidth = this->minWidth;
    }
    if(newHeight < this->minHeight)
    {
        newY = this->previousY + (this->previousHeight - this->minHeight);
        newHeight = this->minHeight;
    }
    setObjectBounds(Rectangle<int>(newX, newY, newWidth, newHeight));
}

void IRNodeComponent::resizeTopRightComponentEvent(const MouseEvent& e)
{
    int currentX = 0;
    int currentY = 0;
    adjustCurrentXYToResizableArea(currentX, currentY, e);
       
    float deltaX = currentX - e.getMouseDownScreenX();
    float deltaY = currentY - e.getMouseDownScreenY();

    float newWidth = this->previousWidth + deltaX;
    float newHeight = this->previousHeight - deltaY;
    float newY = this->previousY + deltaY;
    
    if(newWidth < this->minWidth)
    {
        newWidth = this->minWidth;
    }
    if(newHeight < this->minHeight)
    {
        newY = this->previousY + (this->previousHeight - this->minHeight);
        newHeight = this->minHeight;
    }
    
    Rectangle<int> b2(this->previousX, newY, newWidth, newHeight);
    setObjectBounds(b2);
}
void IRNodeComponent::resizeBottomLeftComponentEvent(const MouseEvent& e)
{
    int currentX = 0;
    int currentY = 0;
    adjustCurrentXYToResizableArea(currentX, currentY, e);
    
    float deltaX = currentX - e.getMouseDownScreenX();
    float deltaY = currentY - e.getMouseDownScreenY();

    float newWidth = this->previousWidth - deltaX;
    float newHeight = this->previousHeight + deltaY;
    float newX = this->previousX + deltaX;
    
    if(newWidth < this->minWidth)
    {
        newX = this->previousX + (this->previousWidth - this->minWidth);
        newWidth = this->minWidth;
    }
    if(newHeight < this->minHeight)
    {
        newHeight = this->minHeight;
    }
    setObjectBounds(Rectangle<int>(newX, this->previousY, newWidth, newHeight));

}
void IRNodeComponent::resizeBottomRightComponentEvent(const MouseEvent& e)
{
     int currentX = 0;
       int currentY = 0;
       adjustCurrentXYToResizableArea(currentX, currentY, e);
       
       float deltaX = currentX - e.getMouseDownScreenX();
       float deltaY = currentY - e.getMouseDownScreenY();

       float newWidth = this->previousWidth + deltaX;
       float newHeight = this->previousHeight + deltaY;
       
       if(newWidth < this->minWidth)
       {
           newWidth = this->minWidth;
       }
       if(newHeight < this->minHeight)
       {
           newHeight = this->minHeight;
       }
       setObjectBounds(Rectangle<int>(this->previousX, this->previousY, newWidth, newHeight));
    
}

void IRNodeComponent::resizeLeftComponentEvent(const MouseEvent& e)
{
    
    int currentX = 0;
    int currentY = 0;
    adjustCurrentXYToResizableArea(currentX, currentY, e);
    
    float deltaX = currentX - e.getMouseDownScreenX();

    float newWidth = this->previousWidth - deltaX;
    float newX = this->previousX + deltaX;
    
    if(newWidth < this->minWidth)
    {
        newX = this->previousX + (this->previousWidth - this->minWidth);
        newWidth = this->minWidth;
    }
    
    setObjectBounds(Rectangle<int>(newX, this->previousY, newWidth, this->previousHeight));
    
}
void IRNodeComponent::resizeRightComponentEvent(const MouseEvent& e)
{

    int currentX = 0;
    int currentY = 0;
    adjustCurrentXYToResizableArea(currentX, currentY, e);
    
    float deltaX = currentX - e.getMouseDownScreenX();

    float newWidth = this->previousWidth + deltaX;
    
    if(newWidth < this->minWidth)
    {
        newWidth = this->minWidth;
    }
    
    setObjectBounds(Rectangle<int>(this->previousX, this->previousY,
                                   newWidth, this->previousHeight));
    
    
}
void IRNodeComponent::resizeTopComponentEvent(const MouseEvent& e)
{
    int currentX = 0;
    int currentY = 0;
    adjustCurrentXYToResizableArea(currentX, currentY, e);
    
    float deltaY = currentY - e.getMouseDownScreenY();

    float newHeight = this->previousHeight - deltaY;
    float newY = this->previousY + deltaY;
    
    if(newHeight < this->minHeight)
    {
        newY = this->previousY + (this->previousHeight - this->minHeight);
        newHeight = this->minHeight;
    }
    setObjectBounds(Rectangle<int>(this->previousX, newY,
                                   this->previousWidth, newHeight));
    
}
void IRNodeComponent::resizeBottomComponentEvent(const MouseEvent& e)
{
    int currentX = 0;
    int currentY = 0;
    adjustCurrentXYToResizableArea(currentX, currentY, e);
    
    float deltaY = currentY - e.getMouseDownScreenY();

    float newHeight = this->previousHeight + deltaY;
    
    if(newHeight < this->minHeight)
    {
        newHeight = this->minHeight;
    }
    setObjectBounds(Rectangle<int>(this->previousX, this->previousY,
                                   this->previousWidth, newHeight));
    
}


// ==================================================

void IRNodeComponent::recoverEventStatus()
{
    
    if(this->isResizing())
    {
        this->resizingFlag = false;
        this->isMovableFlag = true; // recover movableFlag
    }
    if(this->isMoving()) this->movingFlag = false;
    
    // important to call bringThisToFront of resizingSquare, so that the square is now on the IRNodeComponent and catch the mouseEvent. The mouseEvent is sent to IRNodeComponent by addMouseListener.
    // Without bringThisToFront, mouseEvent is stolen after enableSquare() at setSelected method and mouseDrag event is not called anymore.
    if(this->isResizingSquareEnabled() && isEditMode())
    {
        this->resizingSquare.bringThisToFront();
    }else{
        
        this->resizingSquare.toBack();
        this->resizingSquare.showSquare(false);
    }
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
    //std::cout << "setSelected " << flag << " of " << name << " : " << this << std::endl;
    // if this object is not visible, then do nothing.
    
    if(!isActive())
    {
        this->selectedFlag = false;
        this->resizingSquare.showSquare(false);
        
        statusChangedCallback(IRNodeComponentStatus::SelectableStatus); // call back
        selectedChangeEvent();
        
        return;

    }
    
    if(!isVisible()) return;
    
    if(this->selectedFlag == flag) return;
    
    this->selectedFlag = flag;

    if(flag)
    {
        //this->parent->addAndMakeVisible(this->resizingSquare);
        if(this->isResizingSquareEnabled() && isEditMode()){
            this->resizingSquare.showSquare(true);

            std::cout << "square enabled : " << &this->resizingSquare << std::endl;
        }
        //heavyComponentCreatedFunc();
        //this->resizingSquare.toFront(true);
    }else{
        this->resizingSquare.showSquare(false);
    }
    
    
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
            
            if(this->objectType.componentType == ordinaryIRComponent ||
               this->objectType.componentType == heavyWeightComponent)
            {
                //bringThisToFront();
                bringToFront();
            }else{
                toFront(true);
            }
            
            break;
        case 2:
            bringToBack();
            break;
        case 3:
            break;
            
        default:
            break;
    }
    
    // user defined events
    popupMenuEvents();
}

// ==================================================

void IRNodeComponent::showThisObject(bool flag)
{
    
    setVisible(flag);
    
    if(flag)
    {
        if(isEditMode())
        {
            if(this->isResizingSquareEnabled())
                this->resizingSquare.showSquare(flag);
            this->selectedFlag = flag;
        }else{
            this->resizingSquare.showSquare(false);
            this->selectedFlag = false;
        }
        
        /*
        if(!this->parent->isParentOf(this))
        {
            this->parent->addChildComponent(this);
        }*/
        
        
        if(!this->parent->isParentOf(&this->resizingSquare))
        {
            this->parent->addChildComponent(this->resizingSquare);
        }
    }else{
        this->parent->removeChildComponent(&this->resizingSquare);
        //this->parent->removeChildComponent(this);
    }
    
    //IROpenGLManager manager(&this->openGLContext, this->name);
    //manager.setOpenGLContextAlpha(0);

}

// ==================================================

void IRNodeComponent::resizingSquareClicked(IRResizeSquare2::MovableDirection direction)
{
    resizingObjectFunc(direction);
    resizingSquareClickedAction(direction);

}

void IRNodeComponent::resizingSquareReleased(IRResizeSquare2::MovableDirection direction)
{
    //std::cout << "resizingSquareReleased : " << direction << std::endl;

    //reset
    resizingObjectFunc(IRResizeSquare2::MovableDirection::None);
    
    resizingSquareReleasedAction(direction);
}

void IRNodeComponent::resizingSquareDragged(MouseEvent e)
{
    //std::cout <<"resizingSquareDragged : dragged!!\n";
    resizeThisComponentEvent(e);
    resizingSquareDraggedAction(e);

}

void IRNodeComponent::resizingObjectFunc(IRResizeSquare2::MovableDirection direction)
{
    using t = IRResizeSquare2::MovableDirection;
    switch (direction)
    {
        case t::TopLeft:
            setResizable(true, true, false);
            setResizingStart(true);
            this->resizeDirection = t::TopLeft;
            break;

        case t::TopRight:
            setResizable(true, true, false);
            setResizingStart(true);
            this->resizeDirection = t::TopRight;
            break;
            
        case t::Top:
            setResizable(true, false, true);
            setResizingStart(true);
            this->resizeDirection = t::Top;
            break;
        case t::Left:
            setResizable(true, false, true);
            setResizingStart(true);
            this->resizeDirection = t::Left;
            break;
        case t::Right:
            setResizable(true, false, true);
            setResizingStart(true);
            this->resizeDirection = t::Right;
            break;
        case t::Bottom:
            setResizable(true, true, true);
            setResizingStart(true);
            this->resizeDirection = t::Bottom;
            break;
        case t::BottomLeft:
            setResizable(true, true, true);
            setResizingStart(true);
            this->resizeDirection = t::BottomLeft;
            break;
        case t::BottomRight:
            setResizable(true, true, true);
            setResizingStart(true);
            this->resizeDirection = t::BottomRight;
            break;
        default:
            setResizable(false, false, false);
            setResizingStart(false);
            break;
    }
}


// ==================================================




