//
//  VertecesSelector.hpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 30/04/2019.
//

#ifndef VertecesSelector_hpp
#define VertecesSelector_hpp

#include "/Users/keitaro/program/JUCE/IRiMaS/Source/System/IRWorkSpace/ObjectSelection/IRObjectSelection/IRObjectSelection.hpp"

#include "AutomationVertex.hpp"

using namespace IRAutomation;

class VertecesSelector : public IRObjectSelection<vertex* >
{
    
public:
    
    VertecesSelector(Array<vertex* > *list, Component* parent);
    ~VertecesSelector();
    
    void mouseDownHandler(const MouseEvent& e) override;
    void mouseDragHandler(const MouseEvent& e) override;
    void mouseUpHandler(const MouseEvent& e) override;
    
    void deselectAllObjects();
    void addSelectedObjects();
    bool removeSelectedObject(vertex* removeObj);
    
    
    
    void judgeSelection(const Rectangle<int>& area, const MouseEvent& e) override;
    
    Array<vertex* > getSelectedObjectList() const;
    
    void setMovableDirection(bool vertical, bool horizontal);
    
private:
    
    Rectangle<int> getAreaOfSelectedObj();
    
    Array<vertex* > selectedObjectList;
    
    Array<vertex* > *objectList;
    
    bool multiSelectionFlag = false;
    
    bool isVerticalMovable = true;
    bool isHorizontalMovable = true;
    
    Component* parent;
    
    Rectangle<int> boundsConstrainAboveZero(Rectangle<int> rect);
    Rectangle<int> boundsConstrainWithinParent(Rectangle<int> rect);

        
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VertecesSelector)
    
};
#endif /* VertecesSelector_hpp */
