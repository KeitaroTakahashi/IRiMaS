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

class ObjectMenuSpaceArea : public Component,
public IRStrComponent,
public IRHeavyWeightComponent
{
public:
    ObjectMenuSpaceArea(IRStr * str) : IRStrComponent(str),
    IRHeavyWeightComponent(this)
    {
        
    }
    
    ~ObjectMenuSpaceArea()
    {
        
    }
    
    void resized() override
    {
        
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    }
    
    
};
#endif /* ObjectMenuSpaceArea_h */
