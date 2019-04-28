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
    
    IRNodeObjectSelector(Array<IRNodeObject* > *list, bool* linkModeFlag);
    ~IRNodeObjectSelector();

    void mouseDownHandler(const MouseEvent& e) override;
    void mouseDragHandler(const MouseEvent& e) override;
    void mouseUpHandler(const MouseEvent& e) override;

    void deselectAllObjects();
    void deactivateAllLinkingObjects();
    void addSelectedObjects();
    void addActivatedObjects();
    bool removeSelectedObject(IRNodeObject* removeObj);
    bool removeActivatedObject(IRNodeObject* removeObj);

    
    void repaintAllSelectedObjects();
    
    void judgeSelection(const Rectangle<int>& area, const MouseEvent& e) override;

    Array<IRNodeObject* > getSelectedObjectList() const;
    Array<IRNodeObject*> getActivatedLinkingObjectList() const;
    
    
private:
    
    Rectangle<int> getAreaOfSelectedObj();
    
    Array<IRNodeObject* > selectedObjectList;

    Array<IRNodeObject*> activatedLinkingObjectList;
    
    Array<IRNodeObject* > *objectList;
    
    bool multiSelectionFlag = false;
    
    bool* linkModeFlag = nullptr;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRNodeObjectSelector)
    
};




#endif /* IRNodeObjectSelector_hpp */




