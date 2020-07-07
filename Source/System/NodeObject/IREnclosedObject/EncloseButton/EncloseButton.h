//
//  EncloseButton.h
//  IRiMaS
//
//  Created by Keitaro on 07/07/2020.
//

#ifndef EncloseButton_h
#define EncloseButton_h
#include "IRHeavyWeightComponent.h"
#include "IRStrComponent.hpp"
class EncloseButton : public TextButton,
public IRHeavyWeightComponent,
public IRStrComponent
{
public:
    EncloseButton(IRStr* str) : IRHeavyWeightComponent(this, "EncloseButton"),
    IRStrComponent(str)
    {
        
    }
    
    ~EncloseButton()
    {
        
    }
    
    
private:
    
};
#endif /* EncloseButton_h */
