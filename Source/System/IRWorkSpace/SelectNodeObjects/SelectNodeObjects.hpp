//
//  selectComponents.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 20/08/2018.
//  source codes referring Jim Credland
//


// FD NOTE: THIS WHOLE CLASS IS IN FACT UNUSED AND PROBABLY OBSOLETE - SEE NOW WITH OBJECTSELECTION CLASSES



#ifndef SelectNodeObjects_hpp
#define SelectNodeObjects_hpp

#include "JuceHeader.h"
#include "IRNodeObject.hpp"




class SelectNodeObjects
{
    
public:
    
    SelectNodeObjects();
    ~SelectNodeObjects();
    
    void setSelected(IRNodeObject *obj, bool flag);
    void toggleSelection(IRNodeObject* obj);
    
    // this is called when the user clicks on the backgroun of the parent component
    void deSelectedAll();

    bool isSelected(IRNodeObject* obj) const;
    void removeSelectedObj(IRNodeObject* obj);
    void setConstrainBoundsToParent(bool constrainToParentSize,
                                    BorderSize<int> amountPermittedOffscreen);
    void setShiftConstrainsDirection(bool constrainDirection);
    
    void handleMouseDown(IRNodeObject* obj, const MouseEvent& e);
    void handleMouseUp(IRNodeObject* obj, const MouseEvent& e);
    void handleMouseDrag(const MouseEvent& e);
    
    
private:
    
    Rectangle<int> getAreaOfSelectedObj();
    
    void applyingDirectionConstraints(const MouseEvent& e, Point<int> &delta);
    
    enum
    {
        noConstraint,
        xOnly,
        yOnly
    } constrainedDirection;
    
    const int minimumMovementToStartDrag = 10;
    
    std::vector<IRNodeObject*> selectedObj;
    
    bool constrainToParent = true;
    bool shiftConstrainsDirection = false;
    
    bool didJustSelect {false};
    bool didStartDragging {false};
    
    Point<int> mouseDownWithinTarget;
    Point<int> totalDragDelta;
    
    IRNodeObject* objBeingDragged { nullptr };
    
    BorderSize<int> amountPermittedOffscreen;
};

#endif /* SelectNodeObjects_hpp */




