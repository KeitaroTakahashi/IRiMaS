
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

void IRNodeComponent::resizeTopLeftComponentEvent(const MouseEvent& e)
{
    float deltaX = e.getScreenX() - e.getMouseDownScreenX();
    float deltaY = e.getScreenY() - e.getMouseDownScreenY();

    float newWidth = this->previousWidth - deltaX;
    float newHeight = this->previousHeight - deltaY;
    
    float newX = this->previousX + deltaX;
    float newY = this->previousY + deltaY;
    setObjectBounds(Rectangle<int> (newX,
                                    newY,
                                    newWidth,
                                    newHeight));
}
void IRNodeComponent::resizeTopRightComponentEvent(const MouseEvent& e)
{
    float deltaX = e.getScreenX() - e.getMouseDownScreenX();
    float deltaY = e.getScreenY() - e.getMouseDownScreenY();

    float newWidth = this->previousWidth + deltaX;
    float newHeight = this->previousHeight - deltaY;
    
    float newY = this->previousY + deltaY;
    setObjectBounds(Rectangle<int> (this->previousX,
                                    newY,
                                    newWidth,
                                    newHeight));
}
void IRNodeComponent::resizeBottomLeftComponentEvent(const MouseEvent& e)
{
    float deltaX = e.getScreenX() - e.getMouseDownScreenX();
    float deltaY = e.getScreenY() - e.getMouseDownScreenY();

    float newWidth = this->previousWidth - deltaX;
    float newHeight = this->previousHeight + deltaY;
    
    float newX = this->previousX + deltaX;
    setObjectBounds(Rectangle<int> (newX,
                                    this->previousY,
                                    newWidth,
                                    newHeight));
}
void IRNodeComponent::resizeBottomRightComponentEvent(const MouseEvent& e)
{
    float deltaX = e.getScreenX() - e.getMouseDownScreenX();
    float deltaY = e.getScreenY() - e.getMouseDownScreenY();

    float newWidth = this->previousWidth + deltaX;
    float newHeight = this->previousHeight + deltaY;
    
    setObjectBounds(Rectangle<int> (this->previousX,
                                    this->previousY,
                                    newWidth,
                                    newHeight));
}

void IRNodeComponent::resizeLeftComponentEvent(const MouseEvent& e)
{
    float deltaX = e.getScreenX() - e.getMouseDownScreenX();

    float newWidth = this->previousWidth - deltaX;
    float newX = this->previousX + deltaX;
    setObjectBounds(Rectangle<int> (newX,
                                    this->previousY,
                                    newWidth,
                                    this->previousHeight));
}
void IRNodeComponent::resizeRightComponentEvent(const MouseEvent& e)
{
    float deltaX = e.getScreenX() - e.getMouseDownScreenX();
    float deltaY = e.getScreenY() - e.getMouseDownScreenY();

    float newWidth = this->previousWidth + deltaX;

    setObjectBounds(Rectangle<int> (this->previousX,
                                    this->previousY,
                                    newWidth,
                                    this->previousHeight));
}
void IRNodeComponent::resizeTopComponentEvent(const MouseEvent& e)
{
    float deltaY = e.getScreenY() - e.getMouseDownScreenY();

    float newHeight = this->previousHeight - deltaY;
    
    float newY = this->previousY + deltaY;
    setObjectBounds(Rectangle<int> (this->previousX,
                                    newY,
                                    this->previousWidth,
                                    newHeight));
}
void IRNodeComponent::resizeBottomComponentEvent(const MouseEvent& e)
{
    float deltaY = e.getScreenY() - e.getMouseDownScreenY();

    float newHeight = this->previousHeight + deltaY;
    
    setObjectBounds(Rectangle<int> (this->previousX,
                                    this->previousY,
                                    this->previousWidth,
                                    newHeight));
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
    std::cout << "setSelected\n";
    this->selectedFlag = flag;
    
    if(flag)
    {
        this->parent->addAndMakeVisible(this->resizingSquare);
        this->resizingSquare.enableSquare(true);
        heavyComponentCreatedFunc();
        //this->resizingSquare.toFront(true);
    }else{
        this->parent->removeChildComponent(&this->resizingSquare);
        this->resizingSquare.enableSquare(false);
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
                bringThisToFront();
            }else{
                toFront(true);
            }
            
            break;
        case 2:
            bringThisToBack();
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

void IRNodeComponent::initResizingSquare()
{
    
   
}

void IRNodeComponent::showResizingSquare()
{
    
}
void IRNodeComponent::hideResizingSquare()
{
    
}

void IRNodeComponent::resizingObjectSizeByResizingSquare()
{
    
}

// ==================================================

void IRNodeComponent::resizingSquareClicked(IRResizeSquare2::MovableDirection direction)
{
    std::cout << "resizingSquareClicked : " << direction << std::endl;
    resizingObjectFunc(direction);
}

void IRNodeComponent::resizingSquareReleased(IRResizeSquare2::MovableDirection direction)
{
    std::cout << "resizingSquareReleased : " << direction << std::endl;

    
    //reset
    resizingObjectFunc(IRResizeSquare2::MovableDirection::None);
}

void IRNodeComponent::resizingSquareDragged(MouseEvent e)
{
    std::cout <<"resizingSquareDragged : dragged!!\n";
    resizeThisComponentEvent(e);
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




