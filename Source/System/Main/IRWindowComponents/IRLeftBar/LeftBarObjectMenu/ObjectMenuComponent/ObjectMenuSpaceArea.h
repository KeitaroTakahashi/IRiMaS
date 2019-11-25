//
//  ObjectMenuSpaceArea.h
//  IRiMaS
//
//  Created by Keitaro on 20/11/2019.
//

#ifndef ObjectMenuSpaceArea_h
#define ObjectMenuSpaceArea_h
#include "IRStrComponent.hpp"
#include "IRHeavyWeightComponent.h"
#include "ObjectMenuComponent.hpp"

class ObjectMenuSpaceArea : public Component,
public IRStrComponent,
public IRHeavyWeightComponent
{
public:
    ObjectMenuSpaceArea(IRStr * str) : IRStrComponent(str),
    IRHeavyWeightComponent(this, "ObjectMenuSpaceArea")
    {
        
    }
    
    ~ObjectMenuSpaceArea()
    {
        
    }
    // ==================================================

    void resized() override
    {
        if(this->menuObj != nullptr)
            this->menuObj->setBounds(0, 0, getWidth(), 600);
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    }
    // ==================================================

    void setMenuComponent(ObjectMenuComponent* obj)
    {
        if(this->menuObj != nullptr)
            removeChildComponent(this->menuObj);
        
        this->menuObj = obj;
        addAndMakeVisible(this->menuObj);
        resized();

    }
    
    void removeMenuComponent()
    {
        if(this->menuObj != nullptr)
            removeChildComponent(this->menuObj);
        
        this->menuObj = nullptr;
        resized();

    }
    // ==================================================

    void setHidden(bool flag) { this->isHiddenFlag = flag; }
    bool isHidden() const { return this->isHiddenFlag; }
    
    
    // ==================================================

private:
    ObjectMenuComponent* menuObj = nullptr;
    
    // ==================================================
    void bringToFrontCompleted() override
    {
        if(this->menuObj != nullptr)
            this->menuObj->bringThisToFront();
    }
    // ==================================================

    bool isHiddenFlag = true;
    
};
#endif /* ObjectMenuSpaceArea_h */
