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
    // ============================================================
public:
    IRNodeObjectSelector(Array<IRNodeObject* > *list)
    {
        this->objectList = list;
    }
    // ------------------------------------------------------------
    ~IRNodeObjectSelector() {}
    // ------------------------------------------------------------

    void mouseDownHandler(const MouseEvent& e) override;
    void mouseDragHandler(const MouseEvent& e) override;
    void mouseUpHandler(const MouseEvent& e) override;

    // ------------------------------------------------------------
    void deselectAllObjects()
    {
        this->selectedObjectList.clear();
        for(auto obj : *this->objectList)
        {
            if(obj->isSelected()) obj->setSelected(false);
            obj->repaint();
        }
    }
    // ------------------------------------------------------------
    void addSelectedObjects()
    {
        this->selectedObjectList.clear();
        for(auto obj : *this->objectList)
        {
            if(obj->isSelected())
            {
                this->selectedObjectList.add(obj);
            }
        }
    }
    
    bool removeSelectedObject(IRNodeObject* removeObj)
    {
        int index = this->selectedObjectList.indexOf(removeObj);
        if(index >= 0){
            this->selectedObjectList.remove(index);
            return true;
        }
        return false;
    }
    // ------------------------------------------------------------
    void repaintAllSelectedObjects()
    {
        for(auto obj : this->selectedObjectList)
        {
            obj->repaint();
        }
    }
    // ------------------------------------------------------------
    // ============================================================
    void judgeSelection(const Rectangle<int>& area, const MouseEvent& e) override
    {
        for(auto item : *this->objectList)
        {
            IRNodeObject* obj = static_cast<IRNodeObject*>(item);
            
            if(area.intersects(obj->getBounds()))
            {
                if(e.mods.isShiftDown() || e.mods.isCommandDown())
                {
                    obj->setSelected(! obj->isSelected());
                }else{
                    obj->setSelected(true);
                }
            }
        }
        
        addSelectedObjects();
    }
    // ============================================================

    
    Array<IRNodeObject* > getSelectedObjectList() const { return this->selectedObjectList; }
    
    
    // ------------------------------------------------------------

    // ============================================================
private:
    
    
    Rectangle<int> getAreaOfSelectedObj()
    {
        if(this->selectedObjectList.size() == 0)
            return Rectangle<int> (0,0,0,0);
        
        Rectangle<int> a = this->selectedObjectList[0]->getBounds();
        
        for(auto obj : this->selectedObjectList)
            if(obj) a = a.getUnion(obj->getBounds());
        return a;
    }
    
    Array<IRNodeObject* > selectedObjectList;

    Array<IRNodeObject* > *objectList;
    
    bool multiSelectionFlag = false;
    
    
};

#endif /* IRNodeObjectSelector_hpp */
