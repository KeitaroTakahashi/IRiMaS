//
//  AutomationVertex.cpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 01/05/2019.
//

#include "AutomationVertex.hpp"

using namespace IRAutomation;

vertex::vertex(Component* parent)
{
    
    // set how this vertex can be moved by users inputs
    // no move in horizontal way
    setMovable(true, true, false);
}

vertex::~vertex()
{
    
}

void vertex::resized()
{
    
    this->constrainer.setMinimumOnscreenAmounts(getHeight(),
                                                getWidth(),
                                                getHeight(),
                                                getWidth());
    
    
    
    
    
}

void vertex::parentResized()
{
    auto newBounds = getBoundsInParent();
    this->constrainer.checkBounds(newBounds, getBoundsInParent(), newBounds, false, false, false, false);
    // this->constrainer.setMinimumOnscreenAmounts(200,200,200,200);
    this->constrainer.setMinimumOnscreenAmounts(getHeight(),
                                                getWidth(),
                                                getHeight(),
                                                getWidth());
}

// ==================================================

void vertex::paint(Graphics& g)
{
    if(this->isPointFlag)
    {
        if(getCondition() == uiSelected) g.setColour(Colours::red);
        else g.setColour(Colours::orange);
    
        g.fillEllipse(0, 0, getWidth(), getHeight());
    }
}

// ==================================================
// interaction
void vertex::mouseDown(const MouseEvent& e)
{
    //mouseDownNodeEvent(e);
}

void vertex::mouseMove(const MouseEvent& e)
{
    //mouseUpNodeEvent(e);
}

void vertex::mouseUp(const MouseEvent& e)
{
    //mouseUpNodeEvent(e);
}

void vertex::mouseDoubleClick(const MouseEvent& e)
{
   // mouseDoubleClickNodeEvent(e);
}

void vertex::mouseDrag(const MouseEvent& e)
{
    //mouseDragNodeEvent(e); // defines fundamental behavior
}

// ==================================================

void vertex::mouseDownNodeEvent(const MouseEvent& e)
{

    if (this->isMovable())
    {
        // Move this Node Object
        //prepares our dragger to drag this component
        this->dragger.startDraggingComponent(this, e);
        this->movingFlag = true;
    }
    //toFront(true);
}

void vertex::mouseMoveNodeEvent(const MouseEvent& e)
{
    
    //std::cout << "mouse moving : position = " << e.getPosition().x << ", " << e.getPosition().y << std::endl;
    //std::cout << "mouse moving : down position = " << e.getMouseDownX() << ", " << e.getMouseDownY() << std::endl;
}

void vertex::mouseDoubleClickNodeEvent(const MouseEvent& e)
{
    
}

void vertex::mouseDragNodeEvent(const MouseEvent& e)
{
    if (this->isMovable())
    {
        moveThisComponentEvent(e);
    }
}
void vertex::mouseUpNodeEvent(const MouseEvent& e)
{
    recoverEventStatus();
    
}

void vertex::recoverEventStatus()
{
    if(this->isMoving()) this->movingFlag = false;
}

// ==================================================

bool vertex::isMovable() const
{
    return this->isMovableFlag;
}


bool vertex::isVerticalMovable() const
{
    return this->isVerticalMovableFlag;
}


bool vertex::isHorizontalMovable() const
{
    return this->isHorizontalMovableFlag;
}


bool vertex::isMoving() const
{
    return this->movingFlag;
}


bool vertex::isDragging() const
{
    return this->draggingFlag;
}

// all setter
void vertex::setMovable(bool movable, bool verticalMovable, bool horizontalMovable)
{
    this->isMovableFlag = movable;
    this->isVerticalMovableFlag = verticalMovable;
    this->isHorizontalMovableFlag = horizontalMovable;
}


void vertex::moveThisComponentEvent(const MouseEvent& e)
{
    if(isSelected()) this->dragger.dragComponent(this, e, &constrainer);
}
// ==================================================

bool vertex::hitTest(Rectangle<float> area) const
{
    float x = getX() + getWidth() / 2.0;
    float y = getY() + getHeight() / 2.0;
    
    float powX = (x - area.getX()) * (x - area.getX());
    float powY = (y - area.getY()) * (y - area.getY());
    float dist = sqrt (powX + powY);
    if(dist <= (getWidth()/2 + area.getWidth()/2) || dist <= (getHeight()/2 + area.getHeight()/2))
        return true;
    else return false;
    
}

// ==================================================

void vertex::setSelected(bool flag)
{
    if(flag){
        this->condition = uiSelected;
        Point<int> c = getBounds().getCentre();
        setSize(this->selectedSize, this->selectedSize);
        setCentrePosition(c);
    }
    else
    {
        this->condition = uiOrdinary;
        Point<int> c = getBounds().getCentre();
        setSize(this->ordinarySize, this->ordinarySize);
        setCentrePosition(c);
    }
}
bool vertex::isSelected() { if(this->condition == uiSelected) return true; else return false;}

void vertex::setMouseDownPos(Point<int> pos) { this->mouseDownPos = pos; }
Point<int> vertex::getMouseDownPos() const { return this->mouseDownPos; }

// ==================================================

void vertex::setPosition(Point<float>pos)
{
    this->position = pos;
    setCentrePosition(pos.roundToInt());
}

void vertex::setPosition(Point<int>pos)
{
    this->position = pos.toFloat();
    setCentrePosition(pos);
}

void vertex::setPositionEx(Point<float>pos)
{
    this->position = pos;
    setTopLeftPosition(pos.roundToInt());
}



Point<float> vertex::getPosition()
{
    return this->position;
}

// ==================================================
