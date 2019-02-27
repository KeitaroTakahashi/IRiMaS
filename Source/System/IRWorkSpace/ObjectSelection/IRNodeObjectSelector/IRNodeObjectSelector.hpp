//
//  IRNodeObjectSelector.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 22/08/2018.
//

#ifndef IRNodeObjectSelector_hpp
#define IRNodeObjectSelector_hpp

#include "IRObjectSelection.hpp"
#include "IRNodeObject.hpp"





class IRNodeObjectSelector : public IRObjectSelection<IRNodeObject* >
{
    
public:
    
    IRNodeObjectSelector(Array<IRNodeObject* > *list);
    ~IRNodeObjectSelector();

    void mouseDownHandler(const MouseEvent& e) override;
    void mouseDragHandler(const MouseEvent& e) override;
    void mouseUpHandler(const MouseEvent& e) override;

    void deselectAllObjects();
    void addSelectedObjects();
    bool removeSelectedObject(IRNodeObject* removeObj);
    
    void repaintAllSelectedObjects();
    
    void judgeSelection(const Rectangle<int>& area, const MouseEvent& e) override;

    Array<IRNodeObject* > getSelectedObjectList() const;
    
    
private:
    
    Rectangle<int> getAreaOfSelectedObj();
    
    Array<IRNodeObject* > selectedObjectList;

    Array<IRNodeObject* > *objectList;
    
    bool multiSelectionFlag = false;

};




#endif /* IRNodeObjectSelector_hpp */




