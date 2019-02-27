//
//  IRNodeObjectSelector.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 22/08/2018.
//

#include "IRNodeObjectSelector.hpp"

void IRNodeObjectSelector::mouseDownHandler(const MouseEvent& e)
{
    IRNodeObject* nodeObj = dynamic_cast<IRNodeObject* >(e.originalComponent);
    if(nodeObj != nullptr)
    {
        
        //std::cout << "nodeOBj size = "<< nodeObj->getWidth() << "; " << nodeObj->getHeight() << std::endl;
        if(e.mods.isShiftDown() || e.mods.isCommandDown())
        {
            nodeObj->setSelected(! nodeObj->isSelected());
            
        }else if(! nodeObj->isSelected()){ // if the object is not yet selected.
            deselectAllObjects(); // CLEAR
            nodeObj->setSelected(true);
        }
        //repaint obj graphics
        nodeObj->repaint();
        
        //add all selected nodeObj to a list
        addSelectedObjects();
        
        //if this object is selected
        if(nodeObj->isSelected())
        {
            setDragging(true);
            // set this object to be dragged.
            this->setBeingDraggedObject(nodeObj);
            this->mouseDownWithinTarget = e.getEventRelativeTo(nodeObj).getMouseDownPosition();//e.originalComponent->getPosition();//
            this->totalDragDelta = {0,0};
            this->constrainedDirection = noConstraint;
        }
        
        
    }else{
        
        if(! e.mods.isShiftDown() && ! e.mods.isCommandDown())
        {
            //if background clicked, clear all selected status
            deselectAllObjects();
        }
        beginSelection(e);
        this->multiSelectionFlag = true;
        
    }

}
void IRNodeObjectSelector::mouseDragHandler(const MouseEvent& e)
{
    if(this->multiSelectionFlag)
    {
        dragSelection(e);
    }else{
        
        if(! isDragging()) return;
        Point<int> delta = e.getEventRelativeTo(getBeingDraggedObject()).getPosition() - this->mouseDownWithinTarget;

        for(auto comp: this->selectedObjectList)
        {
            if(comp != nullptr)
            {
                if(comp != getBeingDraggedObject())
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
    if(isDragging()) setDragging(false);
    if(this->multiSelectionFlag){
        endSelection(e);
        this->multiSelectionFlag = false;
    }
    repaintAllSelectedObjects();
    
}
